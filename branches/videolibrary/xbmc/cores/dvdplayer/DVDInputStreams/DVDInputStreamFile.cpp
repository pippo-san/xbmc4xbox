
#include "../../../stdafx.h"
#include "DVDInputStreamFile.h"

#include "..\..\..\util.h"


CDVDInputStreamFile::CDVDInputStreamFile() : CDVDInputStream(DVDSTREAM_TYPE_FILE)
{
  m_pFile = NULL;
}

CDVDInputStreamFile::~CDVDInputStreamFile()
{
  Close();
}

bool CDVDInputStreamFile::IsEOF()
{
  if(m_pFile)
  {
    __int64 size = m_pFile->GetLength();
    if( size > 0 && m_pFile->GetPosition() >= size )
      return true;

    return false;
  }

  return true;
}

bool CDVDInputStreamFile::Open(const char* strFile, const std::string& content)
{
  if (!CDVDInputStream::Open(strFile, content)) return false;

  m_pFile = new CFile();
  if (!m_pFile) return false;

  // open file in binary mode
  if (!m_pFile->Open(strFile, true))
  {
    delete m_pFile;
    m_pFile = NULL;
    return false;
  }

  return true;
}

// close file and reset everyting
void CDVDInputStreamFile::Close()
{
  if (m_pFile)
  {
    m_pFile->Close();
    delete m_pFile;
  }

  CDVDInputStream::Close();
  m_pFile = NULL;  
}

int CDVDInputStreamFile::Read(BYTE* buf, int buf_size)
{
  int ret = 0;
  if (m_pFile) ret = m_pFile->Read(buf, buf_size, READ_TRUNCATED);
  else return -1;

  /* on error close file */
  if( ret < 0 ) Close();

  return (int)(ret & 0xFFFFFFFF);
}

__int64 CDVDInputStreamFile::Seek(__int64 offset, int whence)
{
  __int64 ret = 0;
  if (m_pFile) ret = m_pFile->Seek(offset, whence);
  else return -1;

  return ret;
}

__int64 CDVDInputStreamFile::GetLength()
{
  if (m_pFile)
    return m_pFile->GetLength();
  return 0;
}
