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
#include "../log/log.h"

#include "../pal/api/event/event.h"
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

int RunApp(App *app)
{
    if (!app)
    {
        LogError("App is NULL");
        return 1;
    }

    if (!app->launchedCallback)
    {
        LogError("App launched callback is NULL");
        return 1;
    }

    app->launchedCallback(app);

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

    Window* window = GetPlatformWindowData(platformWindow);

    switch (event.type)
    {
        case EVENT_WINDOW_RESIZE:
        {
            if (window->windowClass)
            {
                (window->windowClass->resizeCallback)(window, (Size){event.windowResize.width, event.windowResize.height});
            }

        } break;

        case EVENT_WINDOW_REDRAW:
        {
            if (window->rootView)
            {
                RenderView(window->rootView);
            }

        } break;

        default:
        {
            /* unhandled event */
        } break;
            
    }
}