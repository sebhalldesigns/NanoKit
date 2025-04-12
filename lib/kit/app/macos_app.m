/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  macos_app.m
** Module       :  app
** Author       :  SH
** Created      :  2025-04-11 (YYYY-MM-DD)
** License      :  MIT
** Description  :  macOS App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_app.h"

#include "macos_app.h"

#include "../window/macos_window.h"

#include "../window/platform_window.h"

#include <kit/log/log.h>

#include <stdio.h>
#include <time.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static WindowEventCallback windowEventCallback = NULL;
static ApplicationEventCallback appEventCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(ApplicationEventCallback appCallback, WindowEventCallback windowCallback)
{
    if (!appCallback || !windowCallback)
    {
        LogError("No callback provided");
        return -1;
    }

    appEventCallback = appCallback;
    windowEventCallback = windowCallback;

    @autoreleasepool {
        NSApplication *application = [NSApplication sharedApplication];
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        application.delegate = appDelegate;
        return NSApplicationMain(0, NULL);
    }

    return 0;

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    if (appEventCallback) {

        ApplicationEvent event;
        event.type = APPLICATION_EVENT_LAUNCHED;

        appEventCallback(event);
    }

}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    /* Clean up code here */

}

@end

@implementation WindowDelegate

- (void)windowDidMove:(NSNotification *)notification {

    NSWindow *nsWindow = [notification object];

    nkMacOSWindow *window = NULL;
    if (!GetWindowFromNSWindow(nsWindow, &window))
    {
        printf("Failed to get window from NSWindow\n");
        return;
    }

    if (!window->hasRendered)
    {
        BeginPlatformRender((PlatformWindowHandle)window);

        // Perform rendering here

        EndPlatformRender((PlatformWindowHandle)window);
    }


}


- (void)windowDidResize:(NSNotification *)notification 
{

    NSWindow *nsWindow = [notification object];

    nkMacOSWindow *window = NULL;
    if (!GetWindowFromNSWindow(nsWindow, &window))
    {
        return;
    }
    
    NSRect frame = [nsWindow frame];

    window->Width = frame.size.width;
    window->Height = frame.size.height;

    if (windowEventCallback) 
    {
        WindowEvent event;
        event.type = WINDOW_EVENT_RESIZE;
        event.windowResize.width = frame.size.width;
        event.windowResize.height = frame.size.height;

        windowEventCallback((PlatformWindowHandle)window, event);
    }

    BeginPlatformRender((PlatformWindowHandle)window);

    // Perform rendering here

    EndPlatformRender((PlatformWindowHandle)window);
  
}

// You might also want to add:
- (void)windowWillStartLiveResize:(NSNotification *)notification {

    printf("Window will start live resize\n");

    NSWindow *nsWindow = [notification object];
    nkMacOSWindow *window = NULL;
    
    if (GetWindowFromNSWindow(nsWindow, &window)) {
        // Disable VSync during resize for smoother resizing
        [window->GLContext setValues:(const int[]){0} forParameter:NSOpenGLContextParameterSwapInterval];
    }
}

- (void)windowDidEndLiveResize:(NSNotification *)notification {

    printf("Window did end live resize\n");

    NSWindow *nsWindow = [notification object];
    nkMacOSWindow *window = NULL;
    
    if (GetWindowFromNSWindow(nsWindow, &window)) {
        // Re-enable VSync after resize
        [window->GLContext setValues:(const int[]){1} forParameter:NSOpenGLContextParameterSwapInterval];
        
        // Force complete redraw
        //BeginPlatformRender((PlatformWindowHandle)window);
        //EndPlatformRender((PlatformWindowHandle)window);
    }


}


@end