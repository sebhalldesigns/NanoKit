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

#include "../common/geometry.h"

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void (*ViewInitCallback)(void *view);
typedef void (*ViewResizeCallback)(void *view, Size newSize);

typedef enum 
{
    LAYOUT_TYPE_NONE,
    LAYOUT_TYPE_DOCK,
    LAYOUT_TYPE_VSTACK,
    LAYOUT_TYPE_HSTACK
} ViewLayoutType;

typedef struct ViewClass
{
    const char *name;
    size_t dataSize;    
    ViewResizeCallback resizeCallback;
} ViewClass;

typedef struct View
{
    ViewClass *viewClass;
    Rect frame;
    void *data;

    struct View *parent;
    struct View *subviews;
    size_t subviewCount;

    ViewLayoutType layoutType;
    
} View;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#endif /* VIEW_H */