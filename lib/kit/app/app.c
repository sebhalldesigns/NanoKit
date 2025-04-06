/***************************************************************
**
** NanoKit Library Source File
**
** File         :  app.c
** Module       :  app
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit App API
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "app.h"
#include "platform_app.h"
#include "../window/platform_window.h"

#include "../log/log.h"

#include "../window/window.h"
#include "../view/view.h"

#include <stdio.h>
#include <stdlib.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

void WindowEventCallback(PlatformWindowHandle window, Event event);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunApp(AppLaunchedCallback appLaunched)
{
    if (!appLaunched && GetNumberOfWindows() < 1)
    {
        LogError("App launched callback is NULL with no windows created");
        return 1;
    }

    if (appLaunched)
    {
        appLaunched();
    }

    int statusCode = RunLoop(WindowEventCallback);

    LogInfo("Exiting...");

    return statusCode;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


void WindowEventCallback(PlatformWindowHandle platformWindow, Event event)
{
    if (platformWindow == 0)
    {
        return;
    }

    nkWindow* window = GetPlatformWindowData(platformWindow);

    switch (event.type)
    {
        case EVENT_WINDOW_RESIZE:
        {

            printf("Window resized to %dx%d\n", event.windowResize.width, event.windowResize.height);
            if (window->ResizeCallback)
            {
                (window->ResizeCallback)(window, (nkSize){event.windowResize.width, event.windowResize.height});
            }

            /* resize the root view */
            if (window->Content)
            {
                window->Content->Frame.Size.Width = event.windowResize.width;
                window->Content->Frame.Size.Height = event.windowResize.height;

                LayoutView(window->Content);
            }

        } break;

        case EVENT_WINDOW_REDRAW:
        {
            
            window->RenderCallback;

            if (window->RenderCallback)
            {
                (window->RenderCallback)(window);
            }
            
            if (window->Content)
            {
                RenderView(window->Content);
            }

        } break;

        default:
        {
            /* unhandled event */
        } break;
            
    }
}