#include "SDLMouse.h"
#include "../include.h"
#include "../Key.h"

CMouse g_Mouse; // global

CMouse::CMouse()
{
  m_dwExclusiveWindowID = WINDOW_INVALID;
  m_dwExclusiveControlID = WINDOW_INVALID;
  m_dwState = MOUSE_STATE_NORMAL;
  m_bActive = false;
}

CMouse::~CMouse()
{
}

void CMouse::Initialize(HWND hWnd)
{
//  Acquire();
  // Set the default resolution (PAL)
  SetResolution(720, 576, 1, 1);
}

void CMouse::Update()
{
  bool bMouseMoved(false);
  int x, y;
  Uint8 mouseState = SDL_GetRelativeMouseState(&x, &y);

  cMickeyX = (char)x;
  cMickeyY = (char)y;
  //cWheel = (char)mouseState.lZ; TODO LINUX: how do we mke the scroll wheel to work?
  posX += ((float)cMickeyX * m_fSpeedX); if (posX < 0) posX = 0; if (posX > m_iMaxX) posX = (float)m_iMaxX;
  posY += ((float)cMickeyY * m_fSpeedY); if (posY < 0) posY = 0; if (posY > m_iMaxY) posY = (float)m_iMaxY;

  // reset our activation timer
  m_bActive = true;
  dwLastActiveTime = timeGetTime();

  // Check if we have an update...
  if (bMouseMoved)
  {
    m_bActive = true;
    dwLastActiveTime = timeGetTime();
  }
  else
  {
    cMickeyX = 0;
    cMickeyY = 0;
    cWheel = 0;
    // check how long we've been inactive
    if (timeGetTime() - dwLastActiveTime > MOUSE_ACTIVE_LENGTH) m_bActive = false;
  }
  // Fill in the public members
  bDown[MOUSE_LEFT_BUTTON] = mouseState & SDL_BUTTON(1);
  bDown[MOUSE_RIGHT_BUTTON] = mouseState & SDL_BUTTON(3);
  bDown[MOUSE_MIDDLE_BUTTON] = mouseState & SDL_BUTTON(2);
  bDown[MOUSE_EXTRA_BUTTON1] = mouseState & SDL_BUTTON(4);
  bDown[MOUSE_EXTRA_BUTTON2] = mouseState & SDL_BUTTON(5);
  // Perform the click mapping (for single + double click detection)
  bool bNothingDown = true;
  for (int i = 0; i < 5; i++)
  {
    bClick[i] = false;
    bDoubleClick[i] = false;
    bHold[i] = false;
    if (bDown[i])
    {
      bNothingDown = false;
      if (bLastDown[i])
      { // start of hold
        bHold[i] = true;
      }
      else
      {
        if (timeGetTime() - dwLastClickTime[i] < MOUSE_DOUBLE_CLICK_LENGTH)
        { // Double click
          bDoubleClick[i] = true;
        }
        else
        { // Mouse down
        }
      }
    }
    else
    {
      if (bLastDown[i])
      { // Mouse up
        bNothingDown = false;
        bClick[i] = true;
        dwLastClickTime[i] = timeGetTime();
      }
      else
      { // no change
      }
    }
    bLastDown[i] = bDown[i];
  }
  
  if (bNothingDown)
  { // reset mouse pointer
    SetState(MOUSE_STATE_NORMAL);
  }
}

void CMouse::SetResolution(int iXmax, int iYmax, float fXspeed, float fYspeed)
{
  m_iMaxX = iXmax;
  m_iMaxY = iYmax;
  m_fSpeedX = fXspeed;
  m_fSpeedY = fYspeed;
  // reset the coordinates
  posX = m_iMaxX * 0.5f;
  posY = m_iMaxY * 0.5f;
}

// IsActive - returns true if we have been active in the last MOUSE_ACTIVE_LENGTH period
bool CMouse::IsActive() const
{
  return m_bActive;
}

// turns off mouse activation
void CMouse::SetInactive()
{
  m_bActive = false;
}

bool CMouse::HasMoved() const
{
  return (cMickeyX && cMickeyY);
}

void CMouse::SetExclusiveAccess(DWORD dwControlID, DWORD dwWindowID)
{
  m_dwExclusiveControlID = dwControlID;
  m_dwExclusiveWindowID = dwWindowID;
}

void CMouse::EndExclusiveAccess(DWORD dwControlID, DWORD dwWindowID)
{
  if (m_dwExclusiveControlID == dwControlID && m_dwExclusiveWindowID == dwWindowID)
    SetExclusiveAccess(WINDOW_INVALID, WINDOW_INVALID);
}

void CMouse::Acquire()
{
}
