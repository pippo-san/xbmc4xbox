#ifndef XBOX_VIDEO_RENDERER
#define XBOX_VIDEO_RENDERER
#include <xtl.h>

#include "video.h"
#include "../../settings.h"
#include "XBoxRenderer.h"

class CXBoxRenderManager
{
public:
	CXBoxRenderManager();
	~CXBoxRenderManager();

	void ChangeRenderers();

	// Functions called from the GUI
	void					GetRects(RECT &rs, RECT &rd) { if (!m_bChanging && m_pRenderer) m_pRenderer->GetRects(rs, rd); };
	float					GetAR() { if (!m_bChanging && m_pRenderer) return m_pRenderer->GetAR(); else return 1.0f; };
	void					Update(bool bPauseDrawing) { m_bPauseDrawing = bPauseDrawing; if (!m_bChanging && m_pRenderer) m_pRenderer->Update(); };
	void					RenderUpdate() { if (!m_bChanging && m_pRenderer) m_pRenderer->RenderUpdate(); };
	void					CheckScreenSaver() { if (!m_bChanging && m_pRenderer) m_pRenderer->CheckScreenSaver(); };

	// Functions called from mplayer
	void					WaitForFlip();
	unsigned int	QueryFormat(unsigned int format);
	void					Configure(unsigned int width, unsigned int height, unsigned int d_width, unsigned int d_height);
	unsigned int	GetImage(mp_image_t *mpi);
	unsigned int	PutImage(mp_image_t *mpi);
	unsigned int	DrawFrame();
	unsigned int	DrawSlice(unsigned char *src[], int stride[], int w,int h,int x,int y);
	void					FlipPage();
	unsigned int	PreInit();
	void					UnInit();
	void					DrawAlpha(int x0, int y0, int w, int h, unsigned char *src,unsigned char *srca, int stride);

	int						GetOSDWidth() { return m_iSourceWidth; };
	int						GetOSDHeight() { return m_iSourceHeight; };
	bool					Paused() { return m_bPauseDrawing; };

	CXBoxRenderer *m_pRenderer;
protected:
	float					m_fSourceFrameRatio;	// the frame aspect ratio of the source (corrected for pixel ratio)
	unsigned int	m_iSourceWidth;				// width
	unsigned int	m_iSourceHeight;			// height
	bool					m_bPauseDrawing;			// true if we should pause rendering

	bool					m_bChanging;					// true when we are changing renderers
};

extern CXBoxRenderManager g_renderManager;


#endif