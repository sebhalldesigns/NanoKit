/***************************************************************
**
** NanoKit Library Header File
**
** File         :  geometry.h
** Module       :  common
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Shared Geometry Types
**
***************************************************************/

#ifndef GEOMETRY_H
#define GEOMETRY_H

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

typedef struct 
{
    float width;
    float height;
} Size;

typedef struct 
{
    float x;
    float y;
} Point;

typedef struct 
{
    Point origin;
    Size size;
} Rect;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#endif /* VIEW_H */