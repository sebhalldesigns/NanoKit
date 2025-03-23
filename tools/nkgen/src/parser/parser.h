/***************************************************************
**
** NanoKit Tool Header File
**
** File         :  parser.h
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen XML Parser
**
***************************************************************/

#ifndef PARSER_H
#define PARSER_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef enum 
{
    DOCK_LEFT,
    DOCK_RIGHT,
    DOCK_TOP,
    DOCK_BOTTOM
} DockLocation;

typedef struct 
{
    float Left;
    float Right;
    float Top;
    float Bottom;
} Thickness;

typedef struct View
{
    const char* Class;
    
    struct View* Content;

    /* Events */
    const char* ClickFunction;
    const char* PointerDownFunction;
    const char* PointerUpFunction;
    const char* PointerMoveFunction;

    /* Properties */
    Thickness Margin;
    Thickness Padding;
    DockLocation DockLocation;
    
    struct View* Next;
} View;
 

typedef struct RootView
{
    /* set with Class */
    const char* ClassDef;

    /* content */
    View* Content;

} RootView;

typedef struct Window
{
    /* set with Class */
    const char* ClassDef;

    /* properties */
    const char* Title;
    int Width;
    int Height;

    /* content */
    View* Content;

} Window;

typedef enum
{
    ROOT_NODE_WINDOW,
    ROOT_NODE_VIEW
} RootNodeType;

typedef struct
{
    RootNodeType rootNodeType;

    union 
    {
        Window* window;
        RootView* rootView;
    } rootNode;

} FileContents;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

FileContents ParseFile(char* contents, size_t size, const char* moduleName);

#endif /* PARSER_H */