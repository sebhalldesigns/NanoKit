/***************************************************************
**
** NanoKit Library Header File
**
** File         :  view.h
** Module       :  view
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit View API
**
***************************************************************/

#ifndef VIEW_H
#define VIEW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../common/geometry.h"
#include "../common/color.h"

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

struct nkView; /* forward declaration */

typedef void (*ViewMeasureCallback)(struct nkView *view); 
typedef void (*ViewArrangeCallback)(struct nkView *view);
typedef void (*ViewDrawCallback)(struct nkView *view); 
typedef void (*ViewDestroyCallback)(struct nkView *view); /* called when view is destroyed */

typedef enum
{
    DOCK_POSITION_TOP,
    DOCK_POSITION_BOTTOM,
    DOCK_POSITION_LEFT,
    DOCK_POSITION_RIGHT
} nkDockPosition;

typedef struct
{
    size_t Row;
    size_t Column;
    size_t RowSpan;
    size_t ColumnSpan;
} nkGridLocation;


typedef struct nkView
{    

    const char* Name; /* name of the view */

    nkRect Frame; /* overwritten in layout phase */
    nkSize SizeRequest;

    /* tree structure handles */
    struct nkView *Parent; /* can be NULL*/
    struct nkView *Sibling; /* can be NULL*/
    struct nkView *Child; /* can be NULL*/

    /* Generic layout requests to parent */
    nkStretchType StretchType;
    nkHorizontalAlignment HorizontalAlignment;
    nkVerticalAlignment VerticalAlignment;

    /* Parent panel specific requests */
    nkDockPosition DockPosition;
    nkGridLocation GridLocation;
    nkRect CanvasRect;
    
    /* callbacks */
    ViewMeasureCallback MeasureCallback;
    ViewArrangeCallback ArrangeCallback;
    ViewDrawCallback DrawCallback; /* called when view should be drawn */
    ViewDestroyCallback DestroyCallback; /* called when view is destroyed */

    nkColor BackgroundColor;

    void *Data;

} nkView;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

nkView *CreateView(void);

/* detroys view and all children */
void DestroyView(nkView *view);

/* called by system on window resize */
void LayoutView(nkView *view);

/* called by system when view should be rendered */
void RenderView(nkView *view);

/* VIEW TREE MANAGEMENT */

void AddChildView(nkView *parent, nkView *child);
void RemoveChildView(nkView *parent, nkView *child);
void RemoveView(nkView *view);
void InsertView(nkView *parent, nkView *child, nkView *before);
void ReplaceView(nkView *oldView, nkView *newView);


/* TREE TRAVERSAL */

nkView *NextViewInTree(nkView *view);
nkView *PreviousViewInTree(nkView *view);
nkView *DeepestViewInTree(nkView *view);
nkView *RootViewInTree(nkView *view);
nkView *FirstChildView(nkView *view);
nkView *LastChildView(nkView *view);
nkView *NextSiblingView(nkView *view);
nkView *PreviousSiblingView(nkView *view);





#endif /* VIEW_H */