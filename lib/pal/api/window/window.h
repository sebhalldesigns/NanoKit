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

#ifndef PLATFORMWINDOW_H
#define PLATFORMWINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <extern/nanovg/src/nanovg.h>
#include <kit/common/geometry.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t PlatformWindowHandle;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

PlatformWindowHandle InitPlatformWindow(const char *title, size_t width, size_t height);
void FreePlatformWindow(PlatformWindowHandle window);

void BeginPlatformRender(PlatformWindowHandle window);
void EndPlatformRender(PlatformWindowHandle window);

NVGcontext *GetNanoVGContext(PlatformWindowHandle window);

Size GetWindowSize(PlatformWindowHandle window);

#endif /* PLATFORMWINDOW_H */