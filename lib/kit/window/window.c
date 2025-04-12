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

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

nkWindow *CreateWindow(const char* title, int width, int height)
{

    nkWindow *window = (nkWindow *)malloc(sizeof(nkWindow));
    if (window == NULL)
    {
        LogError("Failed to allocate Window");
        return NULL;
    }

    window->Title = title;
    window->Size.Width = width;
    window->Size.Height = height;

    window->Content = NULL;
    window->ResizeCallback = NULL;
    window->RenderCallback = NULL;

    window->PlatformHandle = InitPlatformWindow(title, width, height, window);

    return window;
}

void DestroyWindow(nkWindow *window)
{
    if (window == NULL)
    {
        return;
    }

    FreePlatformWindow(window->PlatformHandle);
    free(window);
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

