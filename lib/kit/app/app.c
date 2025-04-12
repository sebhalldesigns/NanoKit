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

static AppLaunchedCallback appLaunchedCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

void AppEventCallback(ApplicationEvent event);
void WinEventCallback(PlatformWindowHandle window, WindowEvent event);

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

    appLaunchedCallback = appLaunched;

    int statusCode = RunLoop(AppEventCallback, WinEventCallback);

    LogInfo("Exiting...");

    return statusCode;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


void AppEventCallback(ApplicationEvent event)
{
    switch (event.type)
    {
        case APPLICATION_EVENT_LAUNCHED:
        {
            if (appLaunchedCallback)
            {
                appLaunchedCallback();
            }
        } break;

        default:
        {
            /* unhandled event */
        } break;
    }
}

void WinEventCallback(PlatformWindowHandle platformWindow, WindowEvent event)
{
    if (platformWindow == 0)
    {
        return;
    }

    nkWindow* window = GetPlatformWindowData(platformWindow);

    switch (event.type)
    {
        case WINDOW_EVENT_RESIZE:
        {

            printf("Window resized to %dx%d\n", event.windowResize.width, event.windowResize.height);
            if (window->ResizeCallback)
            {
                (window->ResizeCallback)(window, (nkSize){event.windowResize.width, event.windowResize.height});
            }

            /* update the window size */
            window->Size.Width = event.windowResize.width;
            window->Size.Height = event.windowResize.height;

            clock_t start = clock();
            
            /* layout the window */
            LayoutWindow(window);

            clock_t end = clock();
            double timeSpent = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            printf("Window layout took %.3fms\n", timeSpent);

        } break;

        case WINDOW_EVENT_REDRAW:
        {
            
            window->RenderCallback;

            if (window->RenderCallback)
            {
                (window->RenderCallback)(window);
            }
            
            /* layout the window */
            clock_t start = clock();

            /* render the window */
            RenderWindow(window);

            clock_t end = clock();
            double timeSpent = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            printf("Window render took %.3fms\n", timeSpent);
            
        } break;

        case WINDOW_EVENT_CLOSE:
        {

            printf("Window closed\n");
            
            FreePlatformWindow((PlatformWindowHandle)window);

        } break;

        default:
        {
            /* unhandled event */
        } break;
            
    }
}
