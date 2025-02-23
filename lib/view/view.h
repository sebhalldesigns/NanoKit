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

typedef struct ViewClass
{
    const char *name;
    size_t dataSize;    
    ViewResizeCallback resizeCallback;
} ViewClass;

typedef struct 
{
    ViewClass *viewClass;
    Rect frame;
    void *data;
} View;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

void RegisterViewClass(ViewClass *viewClass);
ViewClass *GetViewClassByName(const char *name);

View *InitView(ViewClass *viewClass);
void FreeView(View *view);

#endif /* VIEW_H */