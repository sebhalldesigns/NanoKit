/***************************************************************
**
** NanoKit Library Header File
**
** File         :  label.h
** Module       :  library
** Author       :  SH
** Created      :  2025-04-12 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit Label Control header file
**
***************************************************************/

#ifndef LABEL_H
#define LABEL_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <view/view.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct 
{
    nkView view;          // Base view
    char *text;           // Text to display
    int alignment;        // Text alignment (left, center, right)
    int padding;          // Padding around the text
} nkLabel;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

nkLabel *nkLabel_Create(void);

void nkLabel_Destroy(nkLabel *label);

#endif /* LIBRARY_H */