/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  win32_window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Window Header
**
***************************************************************/

#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

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

#include <stdbool.h>

#include "platform_window.h"


/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t PlatformWindowHandle;

typedef struct Win32Window
{
    HWND HWnd;
    HINSTANCE HInstance;
    HDC HDc;
    HGLRC HGlrc;
    PAINTSTRUCT PaintStruct;
    NVGcontext *Nvg;

    const char *Title;
    size_t Width;
    size_t Height;

    void *Data;
} nkWin32Window;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromHwnd(HWND hwnd, nkWin32Window **window);

#endif /* WINDOW_H */