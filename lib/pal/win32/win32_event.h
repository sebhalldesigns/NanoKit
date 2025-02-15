/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  win32_event.h
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-15 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Window Header
**
***************************************************************/

#ifndef WIN32_EVENT_H
#define WIN32_EVENT_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/


/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif /* WIN32_WINDOW_H */