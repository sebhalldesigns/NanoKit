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

//#include <windowsx.h>
//#include <sysinfoapi.h>
//#include <process.h>


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
    //SYSTEM_INFO sysinfo;
    //GetSystemInfo(&sysinfo);
    return 0;//(size_t)(sysinfo.dwNumberOfProcessors);
}

ThreadHandle CreateThread(ThreadFunction function, void *arg)
{
    return NULL;//(ThreadHandle)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)function, (LPVOID)arg, 0, NULL);
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/
