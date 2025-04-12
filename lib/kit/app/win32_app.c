/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  win32_app.c
** Module       :  app
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_app.h"
#include "win32_app.h"
#include "../window/win32_window.h"

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
static WindowEventCallback windowEventCallback = NULL;
static ApplicationEventCallback appEventCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static BOOL WINAPI ConsoleHandler(DWORD signal);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(ApplicationEventCallback appCallback, WindowEventCallback windowCallback)
{

    if (!appCallback || !windowCallback)
    {
        LogError("No callback provided");
        return -1;
    }

    appEventCallback = appCallback;
    windowEventCallback = windowCallback;

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
    if (uMsg == WM_DESTROY)
    {
       printf("WM_DESTROY\n");
    }

    nkWin32Window *window = NULL;
    if (!GetWindowFromHwnd(hwnd, &window))
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    WindowEvent event = {0};

    switch (uMsg)
    {
        case WM_SIZE:
        {
            event.type = WINDOW_EVENT_RESIZE;

            window->Width = LOWORD(lParam);
            window->Height = HIWORD(lParam);  

            event.windowResize.width = window->Width;
            event.windowResize.height = window->Height;

            (windowEventCallback)((PlatformWindowHandle)window, event);
        } break;

        case WM_CLOSE:
        {
            event.type = WINDOW_EVENT_CLOSE;
            (windowEventCallback)((PlatformWindowHandle)window, event);
            DestroyWindow(hwnd);
        } break;

        case WM_PAINT:
        {
            
            BeginPlatformRender((PlatformWindowHandle)window);
            
            event.type = WINDOW_EVENT_REDRAW;
            (windowEventCallback)((PlatformWindowHandle)window, event);
            
            EndPlatformRender((PlatformWindowHandle)window);

        } break;

        case WM_MOUSEMOVE:
        {
            event.type = WINDOW_EVENT_MOUSE_MOVE;
            event.mouseMove.x = LOWORD(lParam);
            event.mouseMove.y = HIWORD(lParam);
            (windowEventCallback)((PlatformWindowHandle)window, event);
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