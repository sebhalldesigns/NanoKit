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

#include <extern/glad/glad.h>

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

static ViewClass *viewClasses = NULL;
static size_t viewClassCount = 0;

extern ViewClass *libraryViewClasses;
extern size_t libraryViewClassesCount;


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
    for (size_t i = 0; i < viewClassCount; i++)
    {
        if (strcmp(viewClasses[i].name, name) == 0)
        {
            return &viewClasses[i];
        }
    }

    for (size_t i = 0; i < libraryViewClassesCount; i++)
    {
        if (strcmp(libraryViewClasses[i].name, name) == 0)
        {
            return &libraryViewClasses[i];
        }
    }

    LogWarn("View Class not found: %s", name);

    return NULL;
}

View *CreateView(ViewClass *viewClass)
{

    View *view = (View *)malloc(sizeof(View));
    view->class = viewClass;
    view->frame = (Rect){0, 0, 0, 0};
    view->data = NULL;
    view->parent = NULL;
    view->subviews = NULL;
    view->subviewCount = 0;

    view->textureAttachment = 0;

    return view;

}

void DestroyView(View *view)
{
    return;
}

void LayoutView(View *view)
{
    if (view->class && view->class->layoutCallback)
    {
        (view->class->layoutCallback)(view);
    }

    for (size_t i = 0; i < view->subviewCount; i++)
    {
        LayoutView(&view->subviews[i]);
    }
}

void RenderView(View *view)
{
    if (view->textureAttachment == 0)
    {

        /* create texture attachment */
        printf("Creating texture attachment\n");

        /*
        glGenTextures(1, &view->textureAttachment);

        glBindTexture(GL_TEXTURE_2D, view->textureAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, view->frame.size.width, view->frame.size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        */
    }
    
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

