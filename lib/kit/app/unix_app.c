/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  unix_app.c
** Module       :  app
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Unix App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/


#include "platform_app.h"

#include "../window/unix_window.h"

#include <kit/log/log.h>

#include <stdio.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static bool exitFlag = false;
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

    exitFlag = false;

    Display *display = GetDisplay();
    XEvent xEvent;
    Window xWindow;

    nkUnixWindow *window = NULL;
    WindowEvent event = {0};

    /* call application launched */
    ApplicationEvent appEvent = {0};
    appEvent.type = APPLICATION_EVENT_LAUNCHED;
    appEventCallback(appEvent);

    printf("Entering event loop\n");

    while (!exitFlag)
    {
        XNextEvent(display, &xEvent);

        xWindow = xEvent.xany.window;
        
        if (GetWindowFromHandle(xWindow, &window))
        {
            switch (xEvent.type) {
                case Expose:
                {
                    printf("Window exposed\n");

                    //BeginPlatformRender((PlatformWindowHandle)window);

                    event.type = WINDOW_EVENT_REDRAW;
                    (windowEventCallback)((PlatformWindowHandle)window, event);
            
                    //EndPlatformRender((PlatformWindowHandle)window);
                } break;

                case KeyPress:
                {
                    printf("Key pressed: %d\n", xEvent.xkey.keycode);
                    if (xEvent.xkey.keycode == 9) { // 'Escape' key (keycode 9)
                        XCloseDisplay(display);
                        return 0;
                    }
                } break;

                case ButtonPress:
                {
                    printf("Mouse button pressed at (%d, %d)\n", xEvent.xbutton.x, xEvent.xbutton.y);
                } break;
                
                case MotionNotify:
                {

                    /* Coalesce multiple MotionNotify events to avoid flooding */
                    while (XEventsQueued(display, QueuedAfterReading) > 0) 
                    {
                        XEvent nextEvent;
                        XPeekEvent(display, &nextEvent);
                        if (nextEvent.type != MotionNotify)
                        {
                            break;
                        }
                            
                        /* Consume the next MotionNotify event. */
                        XNextEvent(display, &xEvent);
                    }

                    //printf("Mouse moved to (%d, %d)\n", xEvent.xmotion.x, xEvent.xmotion.y);
                    event.type = WINDOW_EVENT_MOUSE_MOVE;
                    event.mouseMove.x = xEvent.xmotion.x;
                    event.mouseMove.y = xEvent.xmotion.y;
                    (windowEventCallback)((PlatformWindowHandle)window, event);
                } break;

                case ConfigureNotify:
                {

                    /* Coalesce multiple ConfigureNotify events to avoid flooding */
                    while (XEventsQueued(display, QueuedAfterReading) > 0) 
                    {
                        XEvent nextEvent;
                        XPeekEvent(display, &nextEvent);
                        if (nextEvent.type != ConfigureNotify)
                        {
                            break;
                        }
                            
                        /* Consume the next ConfigureNotify event. */
                        XNextEvent(display, &xEvent);
                    }

                    /* ignore repeat size requests */
                    if (window->width == xEvent.xconfigure.width && window->height == xEvent.xconfigure.height)
                    {
                        break;
                    }

                    //printf("Window resized to %dx%d\n", xEvent.xconfigure.width, xEvent.xconfigure.height);
                    event.type = WINDOW_EVENT_RESIZE;

                    window->width = xEvent.xconfigure.width;
                    window->height = xEvent.xconfigure.height;  

                    event.windowResize.width = window->width;
                    event.windowResize.height = window->height;

                    (windowEventCallback)((PlatformWindowHandle)window, event);

                } break;


                case ClientMessage:
                {
                    if (xEvent.xclient.data.l[0] == window->deleteMessage)
                    {
                        event.type = WINDOW_EVENT_CLOSE;
                        (windowEventCallback)((PlatformWindowHandle)window, event);
                    }
                } break;

                default:
                {
                    printf("Unhandled event: %d\n", xEvent.type);
                } break;
            }
    
        }
        else
        {
            printf("Window not found\n");
        }
       
    }

    return 0;

}


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/