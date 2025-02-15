/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  event.h
** Module       :  event
** Author       :  SH
** Created      :  2025-02-13 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Event API
**
***************************************************************/

#ifndef EVENT_H
#define EVENT_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdbool.h>
#include <stddef.h>

#include <pal/api/window/window.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef enum 
{
    EVENT_WINDOW_RESIZE,
    EVENT_WINDOW_CLOSE,
    EVENT_WINDOW_REDRAW,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_DOWN,
    EVENT_MOUSE_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP
} EventType;

typedef struct 
{
    EventType type;
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
} Event;    

typedef void (*EventCallback)(WindowHandle window, Event event);

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

int RunLoop(EventCallback callback);

#endif /* EVENT_H */