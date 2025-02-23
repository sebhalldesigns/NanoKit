/***************************************************************
**
** NanoKit Library Header File
**
** File         :  app.h
** Module       :  app
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit App API
**
***************************************************************/

#ifndef APP_H
#define APP_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void (*AppLaunchedCallback)(void *app);

typedef struct
{
    const char* name;
    const char* description;
    AppLaunchedCallback launchedCallback;
} App;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

int RunApp(App *app);

#endif /* APP_H */