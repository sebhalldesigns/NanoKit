/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  event.c
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Event Loop
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/


#include <pal/api/event/event.h>

#include <pal/unix/x11_window.h>
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
static EventCallback eventCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(EventCallback callback)
{

    if (!callback)
    {
        LogError("No callback provided");
        return -1;
    }

    eventCallback = callback;

    exitFlag = false;

    Display *display = GetDisplay();
    XEvent xEvent;
    Window xWindow;

    X11Window *window = NULL;
    Event event = {0};

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

                    BeginPlatformRender((PlatformWindowHandle)window);

                    event.type = EVENT_WINDOW_REDRAW;
                    (eventCallback)((PlatformWindowHandle)window, event);
            
                    EndPlatformRender((PlatformWindowHandle)window);
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
                    event.type = EVENT_MOUSE_MOVE;
                    event.mouseMove.x = xEvent.xmotion.x;
                    event.mouseMove.y = xEvent.xmotion.y;
                    (eventCallback)((PlatformWindowHandle)window, event);
                } break;

                case ConfigureNotify:
                {

                    /* ignore repeat size requests */
                    if (window->width == xEvent.xconfigure.width && window->height == xEvent.xconfigure.height)
                    {
                        break;
                    }

                    //printf("Window resized to %dx%d\n", xEvent.xconfigure.width, xEvent.xconfigure.height);
                    event.type = EVENT_WINDOW_RESIZE;

                    window->width = xEvent.xconfigure.width;
                    window->height = xEvent.xconfigure.height;  

                    event.windowResize.width = window->width;
                    event.windowResize.height = window->height;

                    (eventCallback)((PlatformWindowHandle)window, event);

                } break;


                case ClientMessage:
                {
                    if (xEvent.xclient.data.l[0] == window->deleteMessage)
                    {
                        event.type = EVENT_WINDOW_CLOSE;
                        (eventCallback)((PlatformWindowHandle)window, event);
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