/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  win32_window.h
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-15 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Window Header
**
***************************************************************/

#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>

#include <extern/glad/glad.h>

#include <extern/nanovg/src/nanovg.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct Win32Window
{
    HWND hwnd;
    HINSTANCE hinstance;
    HDC hdc;
    HGLRC hglrc;
    PAINTSTRUCT ps;
    NVGcontext *nvg;

    const char *title;
    size_t width;
    size_t height;
} Win32Window;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromHwnd(HWND hwnd, Win32Window **window);
size_t GetNumberOfWindows();


#endif /* WIN32_WINDOW_H */