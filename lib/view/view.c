/***************************************************************
**
** NanoKit Library Source File
**
** File         :  view.c
** Module       :  view
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit View API
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "view.h"
#include "../log/log.h"

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

static ViewClass *viewClasses = NULL;
static size_t viewClassCount = 0;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

void RegisterViewClass(ViewClass *viewClass)
{
    if (viewClasses == NULL)
    {
        viewClasses = (ViewClass *)malloc(sizeof(ViewClass));
    }
    else
    {
        viewClasses = (ViewClass *)realloc(viewClasses, sizeof(ViewClass) * (viewClassCount + 1));
    }

    viewClasses[viewClassCount] = *viewClass;
    viewClassCount++;

    LogInfo("Registered View Class: %s", viewClass->name);
}

ViewClass *GetViewClassByName(const char *name)
{
    return NULL;
}

View *InitView(ViewClass *viewClass)
{
    return NULL;

}

void FreeView(View *view)
{

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

