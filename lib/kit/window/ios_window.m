/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  ios_window.m
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

#include "ios_window.h"

#define NANOVG_GLES2_IMPLEMENTATION
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

//static WindowDelegate *windowDelegate = NULL;

static bool initialized = false;

static size_t windowCount = 0;
static nkiOSWindow **windows = NULL;

static EAGLContext *currentGLContext = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/


/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

PlatformWindowHandle InitPlatformWindow(const char *title, size_t width, size_t height, void *data)
{


   
    printf("InitPlatformWindow: %s\n", title);


    return (PlatformWindowHandle)0;
}

void FreePlatformWindow(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkiOSWindow *win32Window = (nkiOSWindow *)window;

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
    windows = (nkiOSWindow **)realloc(windows, windowCount * sizeof(nkiOSWindow));

}

void *GetPlatformWindowData(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkiOSWindow *win32Window = (nkiOSWindow *)window;
    return win32Window->Data;
}

void BeginPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

   
    
}

void EndPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

   

}

NVGcontext *GetNanoVGContext(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkiOSWindow *win32Window = (nkiOSWindow *)window;
    return win32Window->Nvg;
}

nkSize GetWindowSize(PlatformWindowHandle window)
{
    if (!window)
    {
        return (nkSize){0, 0};
    }

    nkiOSWindow *win32Window = (nkiOSWindow *)window;
    return (nkSize){win32Window->Width, win32Window->Height};
}

bool GetWindowFromUIWindow(UIWindow *uiWindow, nkiOSWindow **window)
{
    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i]->uiWindow == uiWindow)
        {
            *window = windows[i];
            return true;
        }
    }

    return false;
}

size_t GetNumberOfWindows()
{
    return windowCount;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/