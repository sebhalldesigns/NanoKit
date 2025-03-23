/***************************************************************
**
** NanoKit Tool Header File
**
** File         :  header.h
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen header file writer
**
***************************************************************/

#ifndef HEADER_H
#define HEADER_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>

#include <parser/parser.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

void WriteHeaderFile(const char* path, const char* moduleName, FileContents* fileContents);

#endif /* HEADER_H */