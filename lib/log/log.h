/***************************************************************
**
** NanoKit Library Header File
**
** File         :  log.h
** Module       :  log
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Basic logging to console
**
***************************************************************/

#ifndef LOG_H
#define LOG_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

void log_Info(const char* message, ...);

void log_Warn(const char* message, ...);

void log_Error(const char* message, ...);


#endif /* LOG_H */