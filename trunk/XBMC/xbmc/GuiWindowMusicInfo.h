#pragma once
#include "GUIDialog.h"
#include "guiwindowmanager.h"
#include "utils/MusicInfoScraper.h"
using namespace MUSIC_GRABBER;

#include "stdstring.h"

class CGUIWindowMusicInfo :
  public CGUIDialog
{
public:
  CGUIWindowMusicInfo(void);
  virtual ~CGUIWindowMusicInfo(void);
  virtual bool    OnMessage(CGUIMessage& message);
  virtual void    OnKey(const CKey& key);
  virtual void    Render();
	void						SetAlbum(CMusicAlbumInfo& album);
protected:
	void										Refresh();
	void										Update();
	void										SetLabel(int iControl, const CStdString& strLabel);
	IDirect3DTexture8* 			m_pTexture;
	CMusicAlbumInfo*				m_pAlbum;
	int											m_iTextureWidth;
	int											m_iTextureHeight;
	bool										m_bViewReview;
};
