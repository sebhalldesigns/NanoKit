/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  unix_window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-02-16 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Unix Window Header
**
***************************************************************/

#ifndef UNIX_WINDOW_H
#define UNIX_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <X11/Xlib.h>

#include <extern/glad/glad.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <extern/nanovg/src/nanovg.h>

#include "platform_window.h"


/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct nkUnixWindow
{
    Window window;
    Atom deleteMessage;
    GLXContext glContext;
    NVGcontext *nvg;
    
    const char *title;
    size_t width;
    size_t height;

    void *data;
} nkUnixWindow;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

bool GetWindowFromHandle(Window hwnd, nkUnixWindow **window);
size_t GetNumberOfWindows();

Display *GetDisplay();


#endif /* UNIX_WINDOW_H */