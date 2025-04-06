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

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void (*ViewLayoutCallback)(void *view); /* possibly have layoutdown and layoutup like WPF? */
typedef void (*ViewDrawCallback)(void *view);

typedef enum
{
    DOCK_POSITION_TOP,
    DOCK_POSITION_BOTTOM,
    DOCK_POSITION_LEFT,
    DOCK_POSITION_RIGHT
} DockPosition;

typedef struct View
{    
    nkRect Frame; /* overwritten in layout phase */
    
    nkSize SizeRequest;

    struct View *Parent; /* can be NULL*/
    
    size_t SubviewCount;
    struct View *Subviews; /* can be NULL*/

    DockPosition DockPosition;

    ViewLayoutCallback LayoutCallback; /* called when layout is needed */
    ViewDrawCallback DrawCallback; /* called when view should be drawn */


    void *Data;
    size_t DataSize;

} nkView;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

nkView *CreateView(void);
void DestroyView(nkView *view);

/* called by system on window resize */
void LayoutView(nkView *view);

/* called by system when view should be rendered */
void RenderView(nkView *view);

#endif /* VIEW_H */