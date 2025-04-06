/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  wasm_window.c
** Module       :  window
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  WASM Window Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_window.h"

#include <kit/log/log.h>

#include <stdio.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <extern/nanovg/src/nanovg_gl.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/


/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/


static bool initialized = false;

static size_t windowCount = 0;


/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/


/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

PlatformWindowHandle InitPlatformWindow(const char *title, size_t width, size_t height, void *data)
{

    if (!initialized)
    {


        initialized = true;
        
    }

    return 0;
}

void FreePlatformWindow(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }


}

void *GetPlatformWindowData(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }
    
    return NULL;
}

void BeginPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }



}

void EndPlatformRender(PlatformWindowHandle window)
{
    if (!window)
    {
        return;
    }

   
}

NVGcontext *GetNanoVGContext(PlatformWindowHandle window)
{
    if (!window)
    {
        return NULL;
    }

  
}

Size GetWindowSize(PlatformWindowHandle window)
{
    if (!window)
    {
        return (Size){0, 0};
    }

            return (Size){0, 0};

}

size_t GetNumberOfWindows()
{
    return windowCount;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/