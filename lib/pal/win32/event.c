/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  event.c
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 Event Loop
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/


#include <pal/api/event/event.h>
#include <pal/win32/win32_event.h>

#include <pal/win32/win32_window.h>
#include <kit/log/log.h>

#include <stdio.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static bool exitFlag = false;
static EventCallback eventCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static BOOL WINAPI ConsoleHandler(DWORD signal);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(EventCallback callback)
{

    if (!callback)
    {
        LogError("No callback provided");
        return -1;
    }

    eventCallback = callback;

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    exitFlag = false;

    MSG msg;

    while (!exitFlag)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (GetNumberOfWindows() == 0)
        {
            LogInfo("No windows open, exiting...");
            exitFlag = true;
        }
    }

    return 0;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window *window = NULL;
    if (!GetWindowFromHwnd(hwnd, &window))
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    Event event = {0};

    switch (uMsg)
    {
        case WM_SIZE:
        {
            event.type = EVENT_WINDOW_RESIZE;

            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);  

            event.windowResize.width = window->width;
            event.windowResize.height = window->height;

            (eventCallback)((PlatformWindowHandle)window, event);
        } break;

        case WM_CLOSE:
        {
            event.type = EVENT_WINDOW_CLOSE;
            (eventCallback)((PlatformWindowHandle)window, event);
        } break;

        case WM_PAINT:
        {
            
            BeginPlatformRender((PlatformWindowHandle)window);
            
            event.type = EVENT_WINDOW_REDRAW;
            (eventCallback)((PlatformWindowHandle)window, event);
            
            EndPlatformRender((PlatformWindowHandle)window);

        } break;

        case WM_MOUSEMOVE:
        {
            event.type = EVENT_MOUSE_MOVE;
            event.mouseMove.x = LOWORD(lParam);
            event.mouseMove.y = HIWORD(lParam);
            (eventCallback)((PlatformWindowHandle)window, event);
        }  break;

        default:
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
        
    }

    return 0;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


static BOOL WINAPI ConsoleHandler(DWORD signal) {

    if (signal == CTRL_C_EVENT)
    {
        exitFlag = true;
    }

    return TRUE;
}