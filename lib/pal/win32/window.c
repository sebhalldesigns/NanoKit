/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  window.c
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Cross-platform event callback
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <pal/api/window/window.h>
#include <pal/win32/win32_window.h>
#include <pal/win32/win32_event.h>

#include <log/log.h>

#include <stdio.h>

#include <windows.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <extern/nanovg/src/nanovg_gl.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/
#define WGL_CONTEXT_MAJOR_VERSION_ARB       (0x2091U)
#define WGL_CONTEXT_MINOR_VERSION_ARB       (0x2092U)
#define WGL_CONTEXT_PROFILE_MASK_ARB        (0x9126U)
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB    (0x0001U)
#define WGL_DRAW_TO_WINDOW_ARB              (0x2001U)
#define WGL_ACCELERATION_ARB                (0x2003U)
#define WGL_SUPPORT_OPENGL_ARB              (0x2010U)
#define WGL_DOUBLE_BUFFER_ARB               (0x2011U)
#define WGL_PIXEL_TYPE_ARB                  (0x2013U)
#define WGL_COLOR_BITS_ARB                  (0x2014U)
#define WGL_DEPTH_BITS_ARB                  (0x2022U)
#define WGL_STENCIL_BITS_ARB                (0x2023U)
#define WGL_FULL_ACCELERATION_ARB           (0x2027U)
#define WGL_TYPE_RGBA_ARB                   (0x202BU)

typedef HGLRC WINAPI wglCreateContextAttribsARB_t(HDC hdc, HGLRC hShareContext, const int *attribList);
typedef BOOL WINAPI wglChoosePixelFormatARB_t(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

const wchar_t *WINDOW_CLASS_NAME = L"NanoKitWindowClass";

const int pixelFormatAttribs[] = {
    WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
    WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
    WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB,         32,
    WGL_DEPTH_BITS_ARB,         24,
    WGL_STENCIL_BITS_ARB,       8,
    0
};

const int gl33Attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0,
};

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/


/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/


static bool initialized = false;

static wglCreateContextAttribsARB_t *wglCreateContextAttribsARB;
static wglChoosePixelFormatARB_t* wglChoosePixelFormatARB;

static size_t windowCount = 0;
static Win32Window **windows = NULL;

static HGLRC currentGlrc = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static LPWSTR CreateWideString(const char* str);
static bool InitOpenGL();


/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

WindowHandle InitWindow(const char *title, size_t width, size_t height)
{

    if (!initialized)
    {

        SetConsoleOutputCP(CP_UTF8);

        WNDCLASS wc = {
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = WindowProc,
            .hInstance = GetModuleHandle(0),
            .lpszClassName = WINDOW_CLASS_NAME,
        };

        RegisterClass(&wc);
        
        if (!InitOpenGL())
        {
            LogError("Failed to initialize OpenGL");
            return 0;
        }

        initialized = true;
        
    }

    LPWSTR wtitle = CreateWideString(title);

    HWND hwnd = CreateWindowEx(
        0, 
        WINDOW_CLASS_NAME, 
        wtitle, 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 
        width, height, 
        NULL, 
        NULL, 
        GetModuleHandle(NULL), 
        NULL
    );

    free(wtitle); /* IMPORTANT: free the allocated wstring title */

    if (hwnd == NULL)
    {
        LogError("Failed to create window");
        return 0;
    }

    HDC gldc = GetDC(hwnd);

    int pixelFormat;
    UINT numFormats;
    wglChoosePixelFormatARB(gldc, pixelFormatAttribs, 0, 1, &pixelFormat, &numFormats);
    if (!numFormats) {
        LogError("Failed to set the OpenGL 3.3 pixel format.");
        return 0;
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(gldc, pixelFormat, sizeof(pfd), &pfd);
    if (!SetPixelFormat(gldc, pixelFormat, &pfd)) {
        LogError("Failed to set the OpenGL 3.3 pixel format.");
        return 0;
    }

    HGLRC glrc = wglCreateContextAttribsARB(gldc, 0, gl33Attribs);
    if (!glrc) {
        LogError("Failed to create OpenGL 3.3 context.");
        return 0;
    }

    if (!wglMakeCurrent(gldc, glrc)) {
        LogError("Failed to activate OpenGL 3.3 rendering context.");
        return 0;
    }

    currentGlrc = glrc;

    gladLoadGL();

    ShowWindow(hwnd, SW_SHOW);

    Win32Window *window = (Win32Window *)malloc(sizeof(Win32Window));
    window->hwnd = hwnd;
    window->hinstance = GetModuleHandle(NULL);
    window->title = title;
    window->width = width;
    window->height = height;
    window->hdc = gldc;
    window->hglrc = glrc;
    window->nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

    windowCount++;
    windows = (Win32Window **)realloc(windows, windowCount * sizeof(Win32Window));
    windows[windowCount - 1] = window;

    return (WindowHandle)window;
}

void FreeWindow(WindowHandle window)
{
    if (!window)
    {
        return;
    }


    if (currentGlrc == ((Win32Window *)window)->hglrc)
    {
        wglMakeCurrent(NULL, NULL);
        currentGlrc = NULL;
    }

    Win32Window *win32Window = (Win32Window *)window;

    DestroyWindow(win32Window->hwnd);
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
    windows = (Win32Window **)realloc(windows, windowCount * sizeof(Win32Window));

}

void BeginRender(WindowHandle window)
{
    if (!window)
    {
        return;
    }

    Win32Window *win32Window = (Win32Window *)window;
    
    if (currentGlrc != win32Window->hglrc)
    {
        wglMakeCurrent(win32Window->hdc, win32Window->hglrc);
        currentGlrc = win32Window->hglrc;
    }
    

    BeginPaint(win32Window->hwnd, &win32Window->ps);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, win32Window->width, win32Window->height);

}

void EndRender(WindowHandle window)
{
    if (!window)
    {
        return;
    }

    Win32Window *win32Window = (Win32Window *)window;
    SwapBuffers(win32Window->hdc);

    EndPaint(win32Window->hwnd, &win32Window->ps);
}

NVGcontext *GetNanoVGContext(WindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    Win32Window *win32Window = (Win32Window *)window;
    return win32Window->nvg;
}

Size GetWindowSize(WindowHandle window)
{
    if (!window)
    {
        return (Size){0, 0};
    }

    Win32Window *win32Window = (Win32Window *)window;
    return (Size){win32Window->width, win32Window->height};
}

bool GetWindowFromHwnd(HWND hwnd, Win32Window **window)
{
    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i]->hwnd == hwnd)
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

static LPWSTR CreateWideString(const char* str)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    LPWSTR wstr = malloc(size * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size);
    return wstr;
}

static bool InitOpenGL()
{
    /* create temporary window and window class to chose window pixel format */

    WNDCLASS windowClass = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProc,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = L"TempWindowClass",
    };

    if (!RegisterClass(&windowClass)) {
        LogError("Failed to register window class");
        return false;
    }

    HWND tempWindow = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        L"TempWindow",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        windowClass.hInstance,
        0);

    if (!tempWindow) {
        LogError("Failed to create temp window");
        return false;
    }

    HDC tempDc = GetDC(tempWindow);

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(pfd),
        .nVersion = 1,
        .iPixelType = PFD_TYPE_RGBA,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
        .cDepthBits = 24,
        .cStencilBits = 8,
    };

    int pixelFormat = ChoosePixelFormat(tempDc, &pfd);
    if (!pixelFormat) {
        LogError("Failed to find a suitable pixel format.");
        return false;
    }

    if (!SetPixelFormat(tempDc, pixelFormat, &pfd)) {
        LogError("Failed to set the pixel format.");
        return false;
    }

    HGLRC tempContext = wglCreateContext(tempDc);
    if (!tempContext) {
        LogError("Failed to create a dummy OpenGL rendering context.");
        return false;
    }

    if (!wglMakeCurrent(tempDc, tempContext)) {
        LogError("Failed to activate dummy OpenGL rendering context.");
        return false;
    }

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_t*)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(tempDc, 0);
    wglDeleteContext(tempContext);
    ReleaseDC(tempWindow, tempDc);
    DestroyWindow(tempWindow);

    return true;
}