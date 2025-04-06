/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  wasm_app.c
** Module       :  app
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  WASM App Implementation
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
        

    return 0;

}


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/
