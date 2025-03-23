/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  window.c
** Module       :  unix
** Author       :  SH
** Created      :  2025-02-16 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Unix window API
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <pal/api/window/window.h>
#include <pal/unix/x11_window.h>

#include <kit/log/log.h>

#include <stdlib.h>
#include <stdio.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <extern/nanovg/src/nanovg_gl.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/* OpenGL 3.3 context attributes */
const static int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 3,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
};

/* OpenGL 3.3 visual attributes */
static int visual_attribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    GLX_DOUBLEBUFFER, True,
    None
};

//extern const char resources_shaders_shader_frag[];



/***************************************************************
** MARK: TYPEDEFS
***************************************************************/


/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/


static Display *display = NULL;
static Window root = 0;
static XVisualInfo *visualInfo = NULL;
GLXFBConfig bestFbc;
XSetWindowAttributes windowAttributes;

static bool initialized = false;

static size_t windowCount = 0;
static X11Window **windows = NULL;

static GLXContext currentGlrc = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

PlatformWindowHandle InitPlatformWindow(const char *title, size_t width, size_t height, void *data)
{

    if (!initialized)
    {

        //printf("loaded shader frag: %s\n", resources_shaders_shader_frag);
        display = XOpenDisplay(NULL);
        if (!display)
        {
            LogError("Failed to open X display");
            return 0;
        }

        root = DefaultRootWindow(display);

        /* try to chose a framebuffer */
        int fbcount;
        GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
        if (!fbc) {
            LogError("Failed to retrieve a framebuffer config");
            return 0;
        }

        /* Pick the first matching FB config */
        bestFbc = fbc[0];
        XFree(fbc);

        /* get a visual */
        visualInfo = glXGetVisualFromFBConfig(display, bestFbc);
        if (!visualInfo) {
            LogError("Failed to get a visual");
            return 0;
        }

        /* Create a colormap */
        Colormap colormap = XCreateColormap(display, RootWindow(display, visualInfo->screen), visualInfo->visual, AllocNone);
        windowAttributes.colormap = colormap;
        windowAttributes.event_mask = ExposureMask | KeyPressMask;

        initialized = true;
    }

    /* Create an Xorg window */

    Window xWnd = XCreateWindow(
        display, root,    /* parent */ 
        0, 0,                               /* position */ 
        width, height,                      /* size */ 
        0,                                  /* border size */  
        visualInfo->depth,         /* depth */ 
        InputOutput,                        /* class */ 
        visualInfo->visual,        /* visual */ 
        CWEventMask | CWColormap,           
        &windowAttributes
    );


    /* Select input events to listen to */
    XSelectInput(display, xWnd, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | PointerMotionMask);

    Atom deleteAtom = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, xWnd, &deleteAtom, 1);
    
    XStoreName(display, xWnd, title);
    Xutf8SetWMProperties(display, xWnd, title, NULL, NULL, 0, NULL, NULL, NULL);
    XMapWindow(display, xWnd);

    /* Create an OpenGL 3.3 context */

    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = NULL;
    glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

    if (!glXCreateContextAttribsARB) {
        LogError("glXCreateContextAttribsARB not found");
        return 0;
    }

    GLXContext context = glXCreateContextAttribsARB(display, bestFbc, NULL, True, context_attribs);
    if (!context) {
        LogError("Failed to create OpenGL context");
        return 0;
    }


    glXMakeCurrent(display, xWnd, context);

    gladLoadGL();

    LogInfo("Initialised OpenGL %s", glGetString(GL_VERSION));

    currentGlrc = context;

    X11Window *window = (X11Window *)malloc(sizeof(X11Window));
    window->window = xWnd;
    window->deleteMessage = deleteAtom;
    window->glContext = context;
    window->title = title;
    window->width = width;
    window->height = height;
    window->nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    window->data = data;

    windowCount++;
    windows = (X11Window **)realloc(windows, windowCount * sizeof(X11Window));
    windows[windowCount - 1] = window;

    return (PlatformWindowHandle)window;
}

void FreePlatformWindow(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }


    if (currentGlrc == ((X11Window *)window)->glContext)
    {
        glXMakeCurrent(display, None, NULL);

        currentGlrc = NULL;
    }

    X11Window *x11Window = (X11Window *)window;

    XDestroyWindow(display, x11Window->window);
    free(x11Window);

    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i] == x11Window)
        {
            for (size_t j = i; j < windowCount - 1; j++)
            {
                windows[j] = windows[j + 1];
            }
            break;
        }
    }

    windowCount--;
    windows = (X11Window **)realloc(windows, windowCount * sizeof(X11Window));

}

void *GetPlatformWindowData(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    X11Window *x11Window = (X11Window *)window;
    return x11Window->data;
}

void BeginPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    X11Window *x11Window = (X11Window *)window;
    
    if (currentGlrc != x11Window->glContext)
    {
        glXMakeCurrent(display, x11Window->window, x11Window->glContext);
        currentGlrc = x11Window->glContext;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glViewport(0, 0, x11Window->width, x11Window->height);

}

void EndPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    X11Window *x11Window = (X11Window *)window;

    glXSwapBuffers(display, x11Window->window);

}

NVGcontext *GetNanoVGContext(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    X11Window *x11Window = (X11Window *)window;
    return x11Window->nvg;
}

Size GetWindowSize(PlatformWindowHandle window)
{
    if (!window)
    {
        return (Size){0, 0};
    }

    X11Window *x11Window = (X11Window *)window;
    return (Size){x11Window->width, x11Window->height};
}

bool GetWindowFromHandle(Window xWnd, X11Window **window)
{
    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i]->window == xWnd)
        {
            *window = windows[i];
            return true;
        }
    }

    printf("Window not found\n");

    return false;
}

size_t GetNumberOfWindows()
{
    return windowCount;
}

Display *GetDisplay()
{
    return display;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/
