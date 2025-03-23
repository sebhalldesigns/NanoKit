/***************************************************************
**
** NanoKit Tool Header File
**
** File         :  source.h
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen source file writer
**
***************************************************************/

#ifndef SOURCE_H
#define SOURCE_H

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

void WriteSourceFile(const char* path, const char* moduleName, FileContents* fileContents);

#endif /* SOURCE_H */