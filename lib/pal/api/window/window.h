/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-02-13 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Window API
**
***************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <extern/nanovg/src/nanovg.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t WindowHandle;

typedef struct Size
{
    float width;
    float height;
} Size;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

WindowHandle InitWindow(const char *title, size_t width, size_t height);
void FreeWindow(WindowHandle window);

void BeginRender(WindowHandle window);
void EndRender(WindowHandle window);

NVGcontext *GetNanoVGContext(WindowHandle window);

Size GetWindowSize(WindowHandle window);

#endif /* WINDOW_H */