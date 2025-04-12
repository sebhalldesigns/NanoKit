/***************************************************************
**
** NanoKit Library Header File
**
** File         :  color.h
** Module       :  common
** Author       :  SH
** Created      :  2025-02-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Shared Color Types
**
***************************************************************/

#ifndef COLOR_H
#define COLOR_H

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
    float Red;
    float Green;
    float Blue;
    float Alpha;
} nkColor;

#define COLOR_BLACK      (nkColor){0.0f, 0.0f, 0.0f, 1.0f}
#define COLOR_WHITE      (nkColor){1.0f, 1.0f, 1.0f, 1.0f}
#define COLOR_RED        (nkColor){1.0f, 0.0f, 0.0f, 1.0f}
#define COLOR_GREEN      (nkColor){0.0f, 1.0f, 0.0f, 1.0f}
#define COLOR_BLUE       (nkColor){0.0f, 0.0f, 1.0f, 1.0f}
#define COLOR_YELLOW     (nkColor){1.0f, 1.0f, 0.0f, 1.0f}
#define COLOR_CYAN       (nkColor){0.0f, 1.0f, 1.0f, 1.0f}
#define COLOR_MAGENTA    (nkColor){0.75f, 0.0f, 0.75f, 1.0f}
#define COLOR_GRAY       (nkColor){0.5f, 0.5f, 0.5f, 1.0f}
#define COLOR_LIGHT_GRAY (nkColor){0.75f, 0.75f, 0.75f, 1.0f}
#define COLOR_DARK_GRAY  (nkColor){0.25f, 0.25f, 0.25f, 1.0f}

#define COLOR_TRANSPARENT (nkColor){0.0f, 0.0f, 0.0f, 0.0f}



/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#endif /* COLOR_H */