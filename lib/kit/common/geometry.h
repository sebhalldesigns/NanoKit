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
    float Width;
    float Height;
} nkSize;

typedef struct 
{
    float X;
    float Y;
} nkPoint;

typedef struct 
{
    nkPoint Origin;
    nkSize Size;
} nkRect;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#endif /* GEOMETRY_H */