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

static WindowDelegate *windowDelegate = NULL;

static bool initialized = false;

static size_t windowCount = 0;
static nkMacOSWindow **windows = NULL;

static NSOpenGLContext *currentGLContext = NULL;

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
        @autoreleasepool {
            windowDelegate = [[WindowDelegate alloc] init];
        }

        initialized = true;
    }

    NSWindow *nsWindow = NULL;
    NSOpenGLView *nsOpenGLView = NULL;
    NSOpenGLContext *glContext = NULL;

    @autoreleasepool {

        NSRect frame = NSMakeRect(100, 100, width, height);

        NSUInteger style = NSWindowStyleMaskTitled
            |   NSWindowStyleMaskClosable           
            |   NSWindowStyleMaskResizable
            |   NSWindowStyleMaskMiniaturizable;

        nsWindow = [NSWindow alloc];
        if (!nsWindow)
        {
            LogError("Failed to create window");
            return 0;
        }

        nsWindow = [nsWindow 
            initWithContentRect: frame
            styleMask: style
            backing: NSBackingStoreBuffered
            defer: NO
        ];

        [nsWindow setTitle:[NSString stringWithUTF8String:title]];

        nsWindow.delegate = windowDelegate;
//        nsWindow.preservesContentDuringLiveResize = NO;

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAColorSize,     24,
            NSOpenGLPFAAlphaSize,     8,
            NSOpenGLPFADepthSize,     24,
            NSOpenGLPFAStencilSize,   8,
            NSOpenGLPFADoubleBuffer, 
            NSOpenGLPFAAccelerated,
            0
        };

        NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (!pixelFormat)
        {
            LogError("Failed to create OpenGL pixel format");
            return 0;
        }

        glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
        if (!glContext)
        {
            LogError("Failed to create OpenGL context");
            return 0;
        }


        nsOpenGLView = [NSOpenGLView alloc];
        if (!nsOpenGLView)
        {
            LogError("Failed to create content view");
            return 0;
        }

        nsOpenGLView = [nsOpenGLView initWithFrame:nsWindow.contentView.bounds pixelFormat:pixelFormat];


        [nsOpenGLView setWantsBestResolutionOpenGLSurface: YES];
        [nsOpenGLView setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable];
        [nsOpenGLView setOpenGLContext:glContext];

        [nsOpenGLView setLayerContentsPlacement: NSViewLayerContentsPlacementTopLeft];
        [nsOpenGLView setLayerContentsRedrawPolicy: NSViewLayerContentsRedrawNever];

        /* disable vsync */
        [glContext setValues:(const int[]){0} forParameter:NSOpenGLContextParameterSwapInterval];


        [glContext makeCurrentContext];

        [nsWindow setContentView:nsOpenGLView];
    }

    printf("InitPlatformWindow: %s\n", title);

    nkMacOSWindow *window = (nkMacOSWindow *)malloc(sizeof(nkMacOSWindow));
    window->nsWindow = nsWindow;
    window->nsOpenGLView = nsOpenGLView;
    window->GLContext = glContext;

    window->Title = title;
    window->Width = width;
    window->Height = height;
    window->hasRendered = false;

    
    window->Nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgCreateFont(window->Nvg, "sans", "/Library/Fonts/Arial Unicode.ttf");


    window->Data = data;

    windowCount++;
    windows = (nkMacOSWindow **)realloc(windows, windowCount * sizeof(nkMacOSWindow));
    windows[windowCount - 1] = window;

    [nsWindow center];
    
    [nsWindow makeKeyAndOrderFront:nil];

    return (PlatformWindowHandle)window;
}

void FreePlatformWindow(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;

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
    windows = (nkMacOSWindow **)realloc(windows, windowCount * sizeof(nkMacOSWindow));

}

void *GetPlatformWindowData(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;
    return win32Window->Data;
}

void BeginPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;

    if (currentGLContext != win32Window->GLContext)
    {
        [win32Window->GLContext makeCurrentContext];
        currentGLContext = win32Window->GLContext;
    }

    [win32Window->GLContext update];

    float width = win32Window->Width;
    float height = win32Window->Height;

    glViewport(0, 0, win32Window->Width, win32Window->Height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear all buffers    
    // Begin NanoVG frame
    nvgBeginFrame(win32Window->Nvg, width, height, 1.0f);
    printf("Starting frame...\n");

    // Set up text properties
    nvgBeginPath(win32Window->Nvg);

    nvgFontSize(win32Window->Nvg, 18.0f);
    nvgFontFace(win32Window->Nvg, "sans");
    nvgFillColor(win32Window->Nvg, nvgRGB(255, 255, 255)); // White color
    nvgStrokeColor(win32Window->Nvg, nvgRGB(0, 0, 0)); // Black color
    nvgTextAlign(win32Window->Nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    // Render text
    nvgText(win32Window->Nvg, width / 2.0f,  height / 2.0f, "Hello, NanoVG!", NULL);
    nvgText(win32Window->Nvg, 200,  100, "Hello, NanoVG!", NULL);
    
    // draw a translucent circle in the top left
    nvgBeginPath(win32Window->Nvg);
    nvgCircle(win32Window->Nvg, 50, 50, 20);
    nvgFillColor(win32Window->Nvg, nvgRGBA(255, 0, 0, 128)); // Red color with alpha
    nvgFill(win32Window->Nvg);
    nvgStrokeColor(win32Window->Nvg, nvgRGB(0, 0, 0)); // Black color
    nvgStrokeWidth(win32Window->Nvg, 2.0f);
    nvgStroke(win32Window->Nvg);

    // End the NanoVG frame
    nvgEndFrame(win32Window->Nvg);

    
}

void EndPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;

    if (currentGLContext != win32Window->GLContext)
    {
        [win32Window->GLContext makeCurrentContext];
        currentGLContext = win32Window->GLContext;
    }

    [win32Window->GLContext flushBuffer];


        [win32Window->GLContext update];

}

NVGcontext *GetNanoVGContext(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;
    return win32Window->Nvg;
}

nkSize GetWindowSize(PlatformWindowHandle window)
{
    if (!window)
    {
        return (nkSize){0, 0};
    }

    nkMacOSWindow *win32Window = (nkMacOSWindow *)window;
    return (nkSize){win32Window->Width, win32Window->Height};
}

bool GetWindowFromNSWindow(NSWindow *nsWindow, nkMacOSWindow **window)
{
    for (size_t i = 0; i < windowCount; i++)
    {
        if (windows[i]->nsWindow == nsWindow)
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