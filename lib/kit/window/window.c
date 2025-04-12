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

static void MeasureWindow(nkWindow *window);
static void ArrangeWindow(nkWindow *window);

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

    window->RootView = NULL;
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

void LayoutWindow(nkWindow *window)
{
    if (window == NULL || window->RootView == NULL)
    {
        LogWarn("Window contains no views!");
        return;
    }

    window->RootView->Frame = (nkRect){0, 0, window->Size.Width, window->Size.Height};

    MeasureWindow(window);
    ArrangeWindow(window);

}

void RenderWindow(nkWindow *window)
{
    if (window == NULL || window->RootView == NULL)
    {
        LogWarn("Window contains no views!");
        return;
    }

    BeginPlatformRender(window->PlatformHandle);

    NVGcontext *context = GetNanoVGContext(window->PlatformHandle);

    nvgBeginFrame(context, window->Size.Width, window->Size.Height, 1.0f);


    nkView *view = window->RootView;

    while (view)
    {   

        nvgBeginPath(context);
        nvgRect(context, view->Frame.Origin.X, view->Frame.Origin.Y, view->Frame.Size.Width, view->Frame.Size.Height);
        nvgFillColor(context, (NVGcolor) {view->BackgroundColor.Red, view->BackgroundColor.Green, view->BackgroundColor.Blue, view->BackgroundColor.Alpha});
        nvgClosePath(context);
        nvgFill(context);

       // printf("Rendered rect at (%f, %f) size (%f, %f)\n", view->Frame.Origin.X, view->Frame.Origin.Y, view->Frame.Size.Width, view->Frame.Size.Height);
        //printf("Renderered color (%f, %f, %f, %f)\n", view->BackgroundColor.Red, view->BackgroundColor.Green, view->BackgroundColor.Blue, view->BackgroundColor.Alpha);

        if (view->DrawCallback)
        {
            view->DrawCallback(view);
        }

        view = NextViewInTree(view);
    }

    nvgEndFrame(context);

    EndPlatformRender(window->PlatformHandle);

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

static void MeasureWindow(nkWindow *window)
{
    nkView *view = DeepestViewInTree(window->RootView);
    
    /* measure views in a bottom-up traversal */
    while (view)
    {
        if (view->MeasureCallback)
        {
            view->MeasureCallback(view);
        }

        view = PreviousViewInTree(view);
    }
}

static void ArrangeWindow(nkWindow *window)
{

    /* arrange views in a top-down traversal */

    nkView *view = window->RootView;

    while (view)
    {

        if (view->ArrangeCallback)
        {
            view->ArrangeCallback(view);
        }

        view = NextViewInTree(view);
    }
}
