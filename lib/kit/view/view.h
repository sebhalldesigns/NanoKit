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
    LAYOUT_TYPE_NONE,
    LAYOUT_TYPE_DOCK,
    LAYOUT_TYPE_VSTACK,
    LAYOUT_TYPE_HSTACK
} ViewLayoutType;


typedef enum
{
    DOCK_POSITION_TOP,
    DOCK_POSITION_BOTTOM,
    DOCK_POSITION_LEFT,
    DOCK_POSITION_RIGHT
} DockPosition;

typedef struct ViewClass
{
    const char *name;
    struct ViewClass * super;   /* optional superclass to inherit from */

    /* Callbacks */
    ViewLayoutCallback layoutCallback;
    ViewDrawCallback drawCallback;      /* set to NULL for no drawing */ 

    size_t dataSize;

} ViewClass;

typedef struct View
{
    ViewClass *class; /* can be NULL */
    
    Rect frame;

    Size sizeRequest;
    
    void *data;

    struct View *parent; /* can be NULL*/
    
    size_t subviewCount;
    struct View *subviews; /* can be NULL*/

    DockPosition dockPosition;

    uintptr_t textureAttachment;
} View;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

void RegisterViewClass(ViewClass *viewClass);
ViewClass *GetViewClassByName(const char *name);

View *CreateView(ViewClass *viewClass);
void DestroyView(View *view);

/* called by system on window resize */
void LayoutView(View *view);

/* called by system when view should be rendered */
void RenderView(View *view);

#endif /* VIEW_H */