/***************************************************************
**
** NanoKit Library Source File
**
** File         :  window.c
** Module       :  window
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit Window API
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "window.h"
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

static WindowClass *windowClasses = NULL;
static size_t windowClassCount = 0;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

void RegisterWindowClass(WindowClass *windowClass)
{
    if (windowClasses == NULL)
    {
        windowClasses = (WindowClass *)malloc(sizeof(WindowClass));
    }
    else
    {
        windowClasses = (WindowClass *)realloc(windowClasses, sizeof(WindowClass) * (windowClassCount + 1));
    }

    windowClasses[windowClassCount] = *windowClass;
    windowClassCount++;

    LogInfo("Registered Window Class: %s", windowClass->name);
}

WindowClass *GetWindowClassByName(const char *name)
{
    return NULL;
}

Window *InitWindow(WindowClass *windowClass, const char *title, size_t width, size_t height)
{

    
    return NULL;
}

void FreeWindow(Window *window)
{

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

