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
    if (!view)
    {
        LogError("Failed to allocate memory for nkView");
        return NULL;
    }

    view->Name = NULL;

    view->Frame = (nkRect){0, 0, 0, 0};
    view->SizeRequest = (nkSize){0, 0};

    
    view->Parent = NULL;
    view->Sibling = NULL;
    view->Child = NULL;

    view->StretchType = STRETCH_NONE;
    view->HorizontalAlignment = ALIGNMENT_CENTER;
    view->VerticalAlignment = ALIGNMENT_MIDDLE;

    view->DockPosition = DOCK_POSITION_TOP;
    view->GridLocation = (nkGridLocation){0, 0, 1, 1};
    view->CanvasRect = (nkRect){0, 0, 0, 0};

    view->MeasureCallback = NULL;
    view->ArrangeCallback = NULL;
    view->DrawCallback = NULL;
    view->DestroyCallback = NULL;

    view->BackgroundColor = COLOR_TRANSPARENT;

    view->Data = NULL;

    return view;
}

void DestroyView(nkView *view)
{
    return;
}

void LayoutView(nkView *view)
{
    
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

/* tree structure functions */
void AddChildView(nkView *parent, nkView *child)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }

    child->Parent = parent;

    if (parent->Child == NULL)
    {
        parent->Child = child;
    }
    else
    {
        nkView *lastChild = parent->Child;

        while (lastChild->Sibling != NULL)
        {
            lastChild = lastChild->Sibling;
        }

        lastChild->Sibling = child;
    }
}

void RemoveChildView(nkView *parent, nkView *child)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }

    if (parent->Child == child)
    {
        parent->Child = child->Sibling;
    }
    else
    {
        nkView *cursor = parent->Child;

        while (cursor != NULL && cursor->Sibling != child)
        {
            cursor = cursor->Sibling;
        }

        if (cursor != NULL)
        {
            cursor->Sibling = child->Sibling;
        }
    }
}

void RemoveView(nkView *view)
{
    if (view == NULL || view->Parent == NULL)
    {
        return;
    }

    RemoveChildView(view->Parent, view);    

}

void InsertView(nkView *parent, nkView *child, nkView *before)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }

    child->Parent = parent;

    if (before == NULL || parent->Child == NULL)
    {
        AddChildView(parent, child);
        return;
    }

    if (parent->Child == before)
    {
        child->Sibling = before;
        parent->Child = child;
        return;
    }

    nkView *prev = parent->Child;
    while (prev->Sibling != NULL && prev->Sibling != before)
    {
        prev = prev->Sibling;
    }

    if (prev->Sibling == before)
    {
        prev->Sibling = child;
        child->Sibling = before;
    }
    else
    {
        /* 'before' not found under parent, add as last child view */
        AddChildView(parent, child);
    }
    
}

void ReplaceView(nkView *oldView, nkView *newView)
{
    if (oldView == NULL || oldView->Parent == NULL || newView == NULL)
    {
        return;
    }

    nkView *parent = oldView->Parent;
    newView->Parent = parent;
    newView->Sibling = oldView->Sibling;

    if (parent->Child == oldView)
    {
        parent->Child = newView;
    }
    else
    {
        nkView *prev = parent->Child;
        while (prev != NULL && prev->Sibling != oldView)
        {
            prev = prev->Sibling;
        }

        if (prev != NULL)
        {
            prev->Sibling = newView;
        }
    }

    oldView->Parent = NULL;
    oldView->Sibling = NULL;
}



nkView *NextViewInTree(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    if (view->Child != NULL)
    {
        return view->Child;
    }
    
    while (view != NULL && view->Sibling == NULL)
    {
        view = view->Parent;
    }

    if (view != NULL)
    {
        return view->Sibling;
    }

    return NULL;
}

nkView *PreviousViewInTree(nkView *view)
{
    if (view == NULL || view->Parent == NULL)
    {
        return NULL;
    }

    nkView *cursor = view->Parent->Child;
    nkView *prev = NULL;

    while (cursor != NULL && cursor->Sibling != view)
    {
        cursor = cursor->Sibling;
    }

    prev = cursor;

    if (prev != NULL)
    {
        while (prev->Child != NULL)
        {
            prev = prev->Child;
            while (prev->Sibling != NULL)
            {
                prev = prev->Sibling;
            }
        }

        return prev;
    }

    return view->Parent;
}


nkView *DeepestViewInTree(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    while (view->Child != NULL)
    {
        view = view->Child;
        while (view->Sibling != NULL)
        {
            view = view->Sibling;
        }
    }

    return view;
}

nkView *RootViewInTree(nkView *view)
{
    while (view != NULL && view->Parent != NULL)
    {
        view = view->Parent;
    }

    return view;
}

nkView *FirstChildView(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    return view->Child;
}

nkView *LastChildView(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    nkView *lastChild = view->Child;

    while (lastChild != NULL && lastChild->Sibling != NULL)
    {
        lastChild = lastChild->Sibling;
    }

    return lastChild;
}

nkView *NextSiblingView(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    if (view->Sibling != NULL)
    {
        return view->Sibling;
    }

    return NULL;
}

nkView *PreviousSiblingView(nkView *view)
{
    if (view == NULL)
    {
        return NULL;
    }

    if (view->Parent != NULL)
    {
        nkView *cursor = view->Parent->Child;
        nkView *prev = NULL;

        while (cursor != NULL && cursor != view)
        {
            prev = cursor;
            cursor = cursor->Sibling;
        }

        return prev;
    }

    return NULL;
}



/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

