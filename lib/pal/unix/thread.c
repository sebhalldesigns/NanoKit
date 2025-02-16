/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  thread.c
** Module       :  win32
** Author       :  SH
** Created      :  2025-02-09 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Cross-platform threading
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/


#include <pal/api/thread/thread.h>

#include <unistd.h>
#include <pthread.h>

#include <stdio.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

size_t GetNumberOfProcessors()
{
    return (size_t)(sysconf(_SC_NPROCESSORS_ONLN));
}

ThreadHandle CreateThread(ThreadFunction function, void *arg)
{
    pthread_t thread;
    pthread_create(&thread, NULL, (void *(*)(void *))function, arg);
    return (ThreadHandle)thread;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/
