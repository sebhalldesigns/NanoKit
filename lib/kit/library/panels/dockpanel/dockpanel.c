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

#include <log/log.h>

#include "dockpanel.h"

#include <string.h>
#include <stdlib.h>
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

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static void MeasureCallback(nkView *view);
static void ArrangeCallback(nkView *view);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

nkDockPanel *nkDockPanel_Create(void)
{
    nkDockPanel *dockPanel = (nkDockPanel *)malloc(sizeof(nkDockPanel));
    if (!dockPanel)
    {
        LogError("Failed to allocate memory for nkDockPanel");
        return NULL;
    }

    dockPanel->View = CreateView();
    if (!dockPanel->View)
    {
        return NULL;
    }

    dockPanel->View->MeasureCallback = MeasureCallback;
    dockPanel->View->ArrangeCallback = ArrangeCallback;

    // Set default values
    dockPanel->LastChildFill = true;

    dockPanel->View->Data = dockPanel;

    return dockPanel;
}

void nkDockPanel_Destroy(nkDockPanel *dockPanel)
{

}


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

static void MeasureCallback(nkView *view)
{
    printf("DOCKPANEL MEASURE\n");

    nkSize total = {0, 0};
    nkView *child = view->Child;

    while (child)
    {
        // We assume child->SizeRequest is already filled (bottom-up)
        if (child->DockPosition == DOCK_POSITION_LEFT || child->DockPosition == DOCK_POSITION_RIGHT)
        {
            total.Width += child->SizeRequest.Width;
            
            if (child->SizeRequest.Height > total.Height)
            {
                total.Height = child->SizeRequest.Height;
            }
                
        }
        else if (child->DockPosition == DOCK_POSITION_TOP || child->DockPosition == DOCK_POSITION_BOTTOM)
        {
            total.Height += child->SizeRequest.Height;

            if (child->SizeRequest.Width > total.Width)
            {
                total.Width = child->SizeRequest.Width;
            }
        }

        child = child->Sibling;
    }

    view->SizeRequest = total;
}

static void ArrangeCallback(nkView *view)
{

    printf("DOCKPANEL ARRANGE\n");

    nkRect client = view->Frame; /* available space inside this view */
    nkView *child = view->Child;

    while (child)
    {

        if (!child->Sibling)
        {
            child->Frame = client;
            return;
        }

        nkRect childRect = client;

        switch (child->DockPosition)
        {
            case DOCK_POSITION_LEFT:
            {
                childRect.Size.Width = child->SizeRequest.Width;
                childRect.Size.Height = client.Size.Height;
                
                if (childRect.Size.Width > client.Size.Width)
                {
                    childRect.Size.Width = client.Size.Width;
                }

                client.Origin.X += childRect.Size.Width;
                client.Size.Width -= childRect.Size.Width;
            } break;

            case DOCK_POSITION_RIGHT:
            {
                childRect.Origin.X = client.Origin.X + client.Size.Width - child->SizeRequest.Width;
                childRect.Size.Width = child->SizeRequest.Width;
                
                if (childRect.Size.Width > client.Size.Width)
                {
                    childRect.Size.Width = client.Size.Width;
                }
                
                childRect.Size.Height = client.Size.Height;
                client.Size.Width -= childRect.Size.Width;
            } break;

            case DOCK_POSITION_TOP:
            {
                childRect.Size.Height = child->SizeRequest.Height;
                childRect.Size.Width = client.Size.Width;
                
                if (childRect.Size.Height > client.Size.Height)
                {
                    childRect.Size.Height = client.Size.Height;
                }

                client.Origin.Y += childRect.Size.Height;
                client.Size.Height -= childRect.Size.Height;
            } break;

            case DOCK_POSITION_BOTTOM:
            {

                childRect.Origin.Y = client.Origin.Y + client.Size.Height - child->SizeRequest.Height;
                childRect.Size.Height = child->SizeRequest.Height;
                
                if (childRect.Size.Height > client.Size.Height)
                {
                    childRect.Size.Height = client.Size.Height;
                }

                childRect.Size.Width = client.Size.Width;
                client.Size.Height -= childRect.Size.Height;
            } break;

            default:
            {
                /* shouldn't get to here */
            } break;
        }


        child->Frame = childRect;
        child = child->Sibling;
       
    }
}