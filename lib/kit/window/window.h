/***************************************************************
**
** NanoKit Library Header File
**
** File         :  window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit Window API
**
***************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>

#include "../common/geometry.h"

#include "../view/view.h"

#include "platform_window.h"

#ifdef _WIN32
    /* Redefine symbols to prevent user32 linker confict */
    #define CreateWindow NanoKit_CreateWindow
    #define DestroyWindow NanoKit_DestroyWindow
    #define UpdateWindow NanoKit_UpdateWindow
#endif


/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void (*WindowInitCallback)(void *view);
typedef void (*WindowResizeCallback)(void *view, nkSize newSize);
typedef void (*WindowRenderCallback)(void *window);

typedef struct 
{
    const char *Title;
    nkSize Size;

    nkView *Content;
    
    WindowResizeCallback ResizeCallback;
    WindowRenderCallback RenderCallback;

    PlatformWindowHandle PlatformHandle;

    size_t DataSize;
    void *DataHandle;
} nkWindow;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

nkWindow *CreateWindow(const char *title, int width, int height);
void UpdateWindow(); /* push changes */
void DestroyWindow(nkWindow *window);

#endif /* WINDOW_H */