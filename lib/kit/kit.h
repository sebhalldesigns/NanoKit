/***************************************************************
**
** NanoKit Library Header File
**
** File         :  kit.h
** Module       :  kit
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Kit lifecycle management
**
***************************************************************/

#ifndef KIT_H
#define KIT_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#ifdef _WIN32
    #include <windows.h>
#endif

#include <stdbool.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#ifdef _WIN32
    LRESULT CALLBACK Win32WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

bool InitKit();
int RunKit();

#endif /* KIT_H */