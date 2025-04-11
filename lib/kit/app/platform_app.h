/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  platform_app.h
** Module       :  app
** Author       :  SH
** Created      :  2025-02-13 (YYYY-MM-DD)
** License      :  MIT
** Description  :  WindowEvent API
**
***************************************************************/

#ifndef PLATFORM_EVENT_H
#define PLATFORM_EVENT_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdbool.h>
#include <stddef.h>

#include "../window/platform_window.h"

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef enum 
{
    WINDOW_EVENT_RESIZE,
    WINDOW_EVENT_CLOSE,
    WINDOW_EVENT_REDRAW,
    WINDOW_EVENT_MOUSE_MOVE,
    WINDOW_EVENT_MOUSE_DOWN,
    WINDOW_EVENT_MOUSE_UP,
    WINDOW_EVENT_KEY_DOWN,
    WINDOW_EVENT_KEY_UP
} WindowEventType;

typedef struct 
{
    WindowEventType type;
    union 
    {
        struct 
        {
            size_t width;
            size_t height;
        } windowResize;
        struct 
        {
            int x;
            int y;
        } mouseMove;
        struct 
        {
            int x;
            int y;
        } mouseDown;
        struct 
        {
            int x;
            int y;
        } mouseUp;
        struct 
        {
            int key;
        } keyDown;
        struct 
        {
            int key;
        } keyUp;
    };
} WindowEvent;    

typedef enum 
{
    APPLICATION_EVENT_LAUNCHED
} ApplicationEventType;

typedef struct 
{
    ApplicationEventType type;
  
} ApplicationEvent;

typedef void (*WindowEventCallback)(PlatformWindowHandle window, WindowEvent event);
typedef void (*ApplicationEventCallback)(ApplicationEvent event);

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

int RunLoop(ApplicationEventCallback appCallback, WindowEventCallback windowCallback);

#endif /* PLATFORM_EVENT_H */