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
#include <string.h>

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
    for (size_t i = 0; i < windowClassCount; i++)
    {
        if (strcmp(windowClasses[i].name, name) == 0)
        {
            return &windowClasses[i];
        }
    }

    LogWarn("View Class not found: %s", name);

    return NULL;
}

Window *CreateWindow(WindowClass *windowClass, const char *title, size_t width, size_t height)
{

    Window *window = (Window *)malloc(sizeof(Window));
    
    window->windowClass = windowClass;

    window->title = title;
    window->size.width = width;
    window->size.height = height;

    window->rootView = NULL;

    window->platformHandle = InitPlatformWindow(title, width, height, window);

    return window;
}

void DestroyWindow(Window *window)
{

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

