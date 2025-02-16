/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  x11_window.h
** Module       :  unix
** Author       :  SH
** Created      :  2025-02-16 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Unix Window Header
**
***************************************************************/

#ifndef X11_WINDOW_H
#define X11_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <X11/Xlib.h>

#include <extern/glad/glad.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <extern/nanovg/src/nanovg.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct X11Window
{
    Window window;
    Atom deleteMessage;
    GLXContext glContext;
    NVGcontext *nvg;
    
    const char *title;
    size_t width;
    size_t height;
} X11Window;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromHandle(Window hwnd, X11Window **window);
size_t GetNumberOfWindows();

Display *GetDisplay();


#endif /* X11_WINDOW_H */