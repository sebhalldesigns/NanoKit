/***************************************************************
**
** NanoKit Library Source File
**
** File         :  view_library.c
** Module       :  view
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit View Library
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "view.h"
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

void org_nanokit_dock_draw(void *view);
void org_nanokit_dock_layout(void *view);

/***************************************************************
** MARK: GLOBAL VARIABLES
***************************************************************/

ViewClass org_nanokit_dock = (ViewClass) {
    .name = "org.nanokit.dock",
    .dataSize = 0,
    .drawCallback = org_nanokit_dock_draw,
    .layoutCallback = org_nanokit_dock_layout
};

ViewClass *libraryViewClasses = { &org_nanokit_dock };
size_t libraryViewClassesCount = sizeof(libraryViewClasses) / sizeof(ViewClass *);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

void org_nanokit_dock_draw(void *view)
{
    printf("Drawing Dock View\n");
}

void org_nanokit_dock_layout(void *view)
{
    printf("Layout Dock View with a size of %f %f \n", ((View *)view)->frame.size.width, ((View *)view)->frame.size.height);

    for (size_t i = 0; i < ((View *)view)->subviewCount; i++)
    {
        LayoutView(&((View *)view)->subviews[i]);
    }
}