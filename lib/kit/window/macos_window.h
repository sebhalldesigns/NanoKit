/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  win32_window.h
** Module       :  window
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Window Header
**
***************************************************************/

#ifndef MACOS_WINDOW_H
#define MACOS_WINDOW_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/


#include <extern/glad/glad.h>
#include <extern/nanovg/src/nanovg.h>

#include <stdbool.h>

#include "platform_window.h"


/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t PlatformWindowHandle;

typedef struct Win32Window
{
   
    NVGcontext *Nvg;

    const char *Title;
    size_t Width;
    size_t Height;

    void *Data;
} nkWin32Window;


/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/


#endif /* MACOS_WINDOW_H */