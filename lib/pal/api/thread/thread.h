/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  thread.h
** Module       :  thread
** Author       :  SH
** Created      :  2025-02-13 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Thread API
**
***************************************************************/

#ifndef THREAD_H
#define THREAD_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdbool.h>
#include <stddef.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef void* ThreadHandle;
typedef int (*ThreadFunction)(void *);

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

size_t GetNumberOfProcessors();

ThreadHandle CreateThread(ThreadFunction function, void *arg);


#endif /* THREAD_H */