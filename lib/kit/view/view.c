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

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/


nkView *CreateView()
{

    nkView *view = (nkView *)malloc(sizeof(nkView));
    view->Frame = (nkRect){0, 0, 0, 0};
    view->Data = NULL;
    view->DataSize = 0;
    view->Parent = NULL;
    view->Subviews = NULL;
    view->SubviewCount = 0;

    //view->textureAttachment = 0;

    return view;

}

void DestroyView(nkView *view)
{
    return;
}

void LayoutView(nkView *view)
{
    for (size_t i = 0; i < view->SubviewCount; i++)
    {
        LayoutView(&view->Subviews[i]);
    }
}

void RenderView(nkView *view)
{
    /*if (view->TextureAttachment == 0) */
    {

        /* create texture attachment */
        //printf("Creating texture attachment\n");

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

