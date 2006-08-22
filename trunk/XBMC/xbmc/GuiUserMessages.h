//  GUI messages outside GuiLib
//
#pragma once
#include "guimessage.h"

//  Bookmark related messages
#define GUI_MSG_REMOVED_MEDIA           GUI_MSG_USER + 1
#define GUI_MSG_UPDATE_BOOKMARKS        GUI_MSG_USER + 2

//  General playlist items changed
#define GUI_MSG_PLAYLIST_CHANGED        GUI_MSG_USER + 3

//  Start Slideshow in my pictures lpVoid = CStdString
//  Param lpVoid: CStdString* that points to the Directory
//  to start the slideshow in.
#define GUI_MSG_START_SLIDESHOW         GUI_MSG_USER + 4

#define GUI_MSG_PLAYBACK_STARTED        GUI_MSG_USER + 5
#define GUI_MSG_PLAYBACK_ENDED          GUI_MSG_USER + 6

//  Playback stopped by user
#define GUI_MSG_PLAYBACK_STOPPED        GUI_MSG_USER + 7

//  Message is send by the playlistplayer when it starts a playlist
//  Parameter:
//  dwParam1 = Current Playlist, can be PLAYLIST_MUSIC, PLAYLIST_TEMP_MUSIC, PLAYLIST_VIDEO or PLAYLIST_TEMP_VIDEO
//  dwParam2 = Item started in the playlist
//  lpVoid = Playlistitem started playing
#define GUI_MSG_PLAYLISTPLAYER_STARTED  GUI_MSG_USER + 8

//  Message is send by playlistplayer when next/previous item is started
//  Parameter:
//  dwParam1 = Current Playlist, can be PLAYLIST_MUSIC, PLAYLIST_TEMP_MUSIC, PLAYLIST_VIDEO or PLAYLIST_TEMP_VIDEO
//  dwParam2 = LOWORD Position of the current playlistitem
//             HIWORD Position of the previous playlistitem
//  lpVoid = Current Playlistitem
#define GUI_MSG_PLAYLISTPLAYER_CHANGED  GUI_MSG_USER + 9

//  Message is send by the playlistplayer when the last item to play ended
//  Parameter:
//  dwParam1 = Current Playlist, can be PLAYLIST_MUSIC, PLAYLIST_TEMP_MUSIC, PLAYLIST_VIDEO or PLAYLIST_TEMP_VIDEO
//  dwParam2 = Playlistitem played when stopping
#define GUI_MSG_PLAYLISTPLAYER_STOPPED  GUI_MSG_USER + 10

#define GUI_MSG_LOAD_SKIN               GUI_MSG_USER + 11

//  Message is send by the dialog scan music
//  Parameter:
//  StringParam = Directory last scanned
#define GUI_MSG_DIRECTORY_SCANNED       GUI_MSG_USER + 12

#define GUI_MSG_SCAN_FINISHED           GUI_MSG_USER + 13

//  Mute activated by the user
#define GUI_MSG_MUTE_ON                 GUI_MSG_USER + 14
#define GUI_MSG_MUTE_OFF                GUI_MSG_USER + 15

//  Player has requested the next item for caching purposes (PAPlayer)
#define GUI_MSG_QUEUE_NEXT_ITEM         GUI_MSG_USER + 16

// Visualisation messages when loading/unloading
#define GUI_MSG_VISUALISATION_UNLOADING GUI_MSG_USER + 17 // sent by vis
#define GUI_MSG_VISUALISATION_LOADED    GUI_MSG_USER + 18 // sent by vis
#define GUI_MSG_GET_VISUALISATION       GUI_MSG_USER + 19 // request to vis for the visualisation object
#define GUI_MSG_VISUALISATION_ACTION    GUI_MSG_USER + 20 // request the vis perform an action

#define GUI_MSG_VIDEO_MENU_STARTED      GUI_MSG_USER + 21 // sent by dvdplayer on entry to the menu

//  Message is sent by built-in function to alert the playlist window
//  that the user has initiated Random playback
//  dwParam1 = Current Playlist (PLAYLIST_MUSIC, PLAYLIST_TEMP_MUSIC, PLAYLIST_VIDEO or PLAYLIST_TEMP_VIDEO)
//  dwParam2 = 0 or 1 (Enabled or Disabled)
#define GUI_MSG_PLAYLISTPLAYER_RANDOM   GUI_MSG_USER + 22

//  Message is sent by built-in function to alert the playlist window
//  that the user has initiated Repeat playback
//  dwParam1 = Current Playlist (PLAYLIST_MUSIC, PLAYLIST_TEMP_MUSIC, PLAYLIST_VIDEO or PLAYLIST_TEMP_VIDEO)
//  dwParam2 = 0 or 1 or 2 (Off, Repeat All, Repeat One)
#define GUI_MSG_PLAYLISTPLAYER_REPEAT   GUI_MSG_USER + 23

// Message is sent by the background info loader when it is finished with fetching a weather location.
#define GUI_MSG_WEATHER_FETCHED       GUI_MSG_USER + 24

