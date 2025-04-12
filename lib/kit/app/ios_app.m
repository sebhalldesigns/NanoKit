/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  ios_app.m
** Module       :  app
** Author       :  SH
** Created      :  2025-04-12 (YYYY-MM-DD)
** License      :  MIT
** Description  :  iOS App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_app.h"

#include "ios_app.h"


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
        // UIApplicationMain sets up the app and starts the event loop
        return UIApplicationMain(0, NULL, NULL, NSStringFromClass([AppDelegate class]));
    }

    return 0;

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    if (appEventCallback) {

        ApplicationEvent event;
        event.type = APPLICATION_EVENT_LAUNCHED;

        appEventCallback(event);
    }
}


@end
