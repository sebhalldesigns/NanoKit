/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  macos_window.m
** Module       :  window
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  macOS Window Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <kit/log/log.h>

#include <stdio.h>

#include "macos_window.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <extern/nanovg/src/nanovg_gl.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/


/***************************************************************
** MARK: TYPEDEFS
***************************************************************/


/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/


static bool initialized = false;

static size_t windowCount = 0;
static nkWin32Window **windows = NULL;

//static HGLRC currentGlrc = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/


/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

PlatformWindowHandle InitPlatformWindow(const char *title, size_t width, size_t height, void *data)
{

    nkWin32Window *window = (nkWin32Window *)malloc(sizeof(nkWin32Window));
    window->Title = title;
    window->Width = width;
    
    window->Nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    window->Data = data;
    return (PlatformWindowHandle)window;
}

void FreePlatformWindow(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }


   

    nkWin32Window *win32Window = (nkWin32Window *)window;

    free(win32Window);

    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i] == win32Window)
        {
            for (size_t j = i; j < windowCount - 1; j++)
            {
                windows[j] = windows[j + 1];
            }
            break;
        }
    }

    windowCount--;
    windows = (nkWin32Window **)realloc(windows, windowCount * sizeof(nkWin32Window));

}

void *GetPlatformWindowData(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkWin32Window *win32Window = (nkWin32Window *)window;
    return win32Window->Data;
}

void BeginPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkWin32Window *win32Window = (nkWin32Window *)window;
    


}

void EndPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkWin32Window *win32Window = (nkWin32Window *)window;

}

NVGcontext *GetNanoVGContext(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkWin32Window *win32Window = (nkWin32Window *)window;
    return win32Window->Nvg;
}

nkSize GetWindowSize(PlatformWindowHandle window)
{
    if (!window)
    {
        return (nkSize){0, 0};
    }

    nkWin32Window *win32Window = (nkWin32Window *)window;
    return (nkSize){win32Window->Width, win32Window->Height};
}

//bool GetWindowFromHwnd(HWND hwnd, nkWin32Window **window)
//{
//    for (size_t i = 0; i < windowCount; i++)
//    {
        /*if (windows[i]->HWnd == hwnd)
        {
            *window = windows[i];
            return true;
        }*/
//    }

//    return false;/
//}

size_t GetNumberOfWindows()
{
    return windowCount;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

