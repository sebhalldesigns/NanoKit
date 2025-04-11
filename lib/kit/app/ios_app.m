/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  ios_app.m
** Module       :  app
** Author       :  SH
** Created      :  2025-04-11 (YYYY-MM-DD)
** License      :  MIT
** Description  :  iOS App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_app.h"

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

static WindowEventCallback windowCallback = NULL;
static ApplicationEventCallback appCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(ApplicationEventCallback appCallback, WindowEventCallback windowCallback)
{
    if (!callback)
    {
        LogError("No callback provided");
        return -1;
    }

    eventCallback = callback;


    return 0;

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

