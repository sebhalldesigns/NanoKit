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

#ifndef MACOS_WINDOW_H
#define MACOS_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <extern/nanovg/src/nanovg.h>

#include <stdbool.h>

#include "platform_window.h"
#include "../app/macos_app.h"

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t PlatformWindowHandle;

typedef struct
{
    NSWindow *nsWindow;
    NSOpenGLView *nsOpenGLView;
    NSOpenGLContext *GLContext;
    bool hasRendered;

    NVGcontext *Nvg;

    const char *Title;
    size_t Width;
    size_t Height;

    void *Data;
} nkMacOSWindow;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromNSWindow(NSWindow *nsWindow, nkMacOSWindow **window);

#endif /* MACOS_WINDOW_H */