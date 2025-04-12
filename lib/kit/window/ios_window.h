/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  ios_window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-04-12 (YYYY-MM-DD)
** License      :  MIT
** Description  :  iOS Window Header
**
***************************************************************/

#ifndef IOS_WINDOW_H
#define IOS_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

//#include <extern/nanovg/src/nanovg.h>

#include <stdbool.h>

#include "platform_window.h"
#include "../app/ios_app.h"

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t PlatformWindowHandle;

typedef struct
{
    UIWindow *uiWindow;
    GLKView *glkView;
    EAGLContext *GLContext;
    bool hasRendered;

    NVGcontext *Nvg;

    const char *Title;
    size_t Width;
    size_t Height;

    void *Data;
} nkiOSWindow;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromUIWindow(UIWindow *uiWindow, nkiOSWindow **window);

#endif /* IOS_WINDOW_H */