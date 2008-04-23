/*
 *      Copyright (C) 2005-2007 Team XboxMediaCenter
 *      http://www.xboxmediacenter.com
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "stdafx.h"
#include "BackgroundInfoLoader.h"
#include "FileItem.h"

#ifdef _XBOX
#define ITEMS_PER_THREAD 10
#define MAX_THREAD_COUNT 2
#else
#define ITEMS_PER_THREAD 5
#define MAX_THREAD_COUNT 5
#endif

CBackgroundInfoLoader::CBackgroundInfoLoader(int nThreads)
{
  m_bRunning = false;
  m_bStop = true;
  m_pObserver=NULL;
  m_pProgressCallback=NULL;
  m_pVecItems = NULL;
  m_nRequestedThreads = nThreads;
  m_bStartCalled = false;
  m_nActiveThreads = 0;
}

CBackgroundInfoLoader::~CBackgroundInfoLoader()
{
  StopThread();
}

void CBackgroundInfoLoader::SetNumOfWorkers(int nThreads)
{
  m_nRequestedThreads = nThreads;
}

void CBackgroundInfoLoader::Run()
{
  try
  {
    if (m_vecItems.size() > 0)
    {
      {
        CSingleLock lock(m_lock);
        if (!m_bStartCalled)
        {
          OnLoaderStart();
          m_bStartCalled = true;
        }
      }

      while (!m_bStop)
      {
        CSingleLock lock(m_lock);
        CFileItem *pItem = NULL;
        std::vector<CFileItem*>::iterator iter = m_vecItems.begin();
        if (iter != m_vecItems.end())
        {
          pItem = *iter;
          m_vecItems.erase(iter);
        }

        if (pItem == NULL)
          break;

        // Ask the callback if we should abort
        if (m_pProgressCallback && m_pProgressCallback->Abort())
          m_bStop=true;

        lock.Leave();
        try
        {
          if (!m_bStop && LoadItem(pItem) && m_pObserver)
            m_pObserver->OnItemLoaded(pItem);
        }
        catch (...)
        {
          CLog::Log(LOGERROR, "%s::LoadItem - Unhandled exception for item %s", __FUNCTION__, pItem->m_strPath.c_str());
        }
      }
    }

    CSingleLock lock(m_lock);
    if (m_nActiveThreads == 1)
      OnLoaderFinish();
    m_nActiveThreads--;

  }
  catch (...)
  {
    m_nActiveThreads--;
    CLog::Log(LOGERROR, "%s - Unhandled exception", __FUNCTION__);
  }
}

void CBackgroundInfoLoader::Load(CFileItemList& items)
{
  StopThread();

  if (items.Size() == 0)
    return;
  
  EnterCriticalSection(m_lock);

  for (int nItem=0; nItem < items.Size(); nItem++)
    m_vecItems.push_back(items[nItem]);

  m_pVecItems = &items;
  m_bRunning = true;
  m_bStop = false;
  m_bStartCalled = false;

  int nThreads = m_nRequestedThreads;
  if (nThreads == -1)
    nThreads = (m_vecItems.size() / (ITEMS_PER_THREAD+1)) + 1;

  if (nThreads > MAX_THREAD_COUNT)
    nThreads = MAX_THREAD_COUNT;

  m_nActiveThreads = nThreads;
  for (int i=0; i < nThreads; i++)
  {
    CThread *pThread = new CThread(this); 
    pThread->Create();
#ifndef _LINUX
    pThread->SetPriority(THREAD_PRIORITY_BELOW_NORMAL);
#endif
    pThread->SetName("Background Loader");
    m_workers.push_back(pThread);
  }
      
  LeaveCriticalSection(m_lock);
}

void CBackgroundInfoLoader::StopThread()
{
  m_bStop = true;
  EnterCriticalSection(m_lock);
  m_vecItems.clear();
  LeaveCriticalSection(m_lock);

  for (int i=0; i<(int)m_workers.size(); i++)
  {
    m_workers[i]->StopThread();
    delete m_workers[i];
  }

  m_workers.clear();

  m_pVecItems = NULL;
  m_bRunning = false;
  m_nActiveThreads = 0;
}

bool CBackgroundInfoLoader::IsLoading()
{
  return m_nActiveThreads > 0;
}

void CBackgroundInfoLoader::SetObserver(IBackgroundLoaderObserver* pObserver)
{
  m_pObserver = pObserver;
}

void CBackgroundInfoLoader::SetProgressCallback(IProgressCallback* pCallback)
{
  m_pProgressCallback = pCallback;
}

