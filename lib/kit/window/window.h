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
#include "../pal/api/window/window.h"

#ifdef _WIN32

    /* Redefine symbols to prevent user32 linker confict */
    #define CreateWindow NanoKit_CreateWindow
    #define DestroyWindow NanoKit_DestroyWindow
#endif

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void (*WindowInitCallback)(void *view);
typedef void (*WindowResizeCallback)(void *view, Size newSize);

typedef struct WindowClass
{
    const char *name;
    size_t dataSize;    
    WindowResizeCallback resizeCallback;
} WindowClass;

typedef struct 
{
    
    WindowClass *windowClass;

    const char *title;
    Size size;

    View *rootView;
    
    PlatformWindowHandle platformHandle;

    /*void *data;*/
} Window;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

void RegisterWindowClass(WindowClass *windowClass);
WindowClass *GetWindowClassByName(const char *name);

Window *CreateWindow(WindowClass *windowClass, const char *title, size_t width, size_t height);
void DestroyWindow(Window *window);

#endif /* WINDOW_H */