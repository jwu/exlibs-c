// ======================================================================================
// File         : thread_win32.c
// Author       : Wu Jie 
// Last Change  : 02/06/2011 | 11:58:09 AM | Sunday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include <process.h>    /* _beginthread, _endthread */

///////////////////////////////////////////////////////////////////////////////
// typedefs
///////////////////////////////////////////////////////////////////////////////

typedef uintptr_t (__cdecl *pfn_BeginThread) ( void *, unsigned,
                                               unsigned (__stdcall *func)(void *), void *arg, 
                                               unsigned, unsigned *threadID );
typedef void (__cdecl *pfn_EndThread)( unsigned code );

typedef struct __ThreadStartParms_t {
    void *args;
    pfn_EndThread end_thread;
} __ThreadStartParms_t;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_run_thread ( void * );
// ------------------------------------------------------------------ 

static unsigned __stdcall __run_thread ( void *_data ) {
    __ThreadStartParms_t *threadParms = (__ThreadStartParms_t *)_data;
    pfn_EndThread endThread = NULL;

    // Call the thread function!
    ex_run_thread(threadParms->args);

    // Get the current endthread we have to use!
    if (threadParms) {
        endThread = threadParms->end_thread;
        ex_free_nomng(threadParms);
    }
    // Call endthread!
    if ( endThread )
        (*endThread)(0);
    return(0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_sys_create_thread ( ex_thread_t *_thread, void *_args ) {
    unsigned threadid;
    __ThreadStartParms_t *threadParms = (__ThreadStartParms_t *)ex_malloc_nomng(sizeof(__ThreadStartParms_t));

    if ( !threadParms ) {
        ex_error("out of memory!");
        return(-1);
    }

    // Save the function which we will have to call to clear the RTL of calling app!
    threadParms->end_thread = _endthreadex;
    // Also save the real parameters we have to pass to thread function
    threadParms->args = _args;

    _thread->handle = (HANDLE)_beginthreadex(NULL, 0, __run_thread, threadParms, 0, &threadid);
    if (_thread->handle == NULL) {
        ex_error("Not enough resources to create thread");
        return(-1);
    }
    return(0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_wait_thread ( ex_thread_t *_thread ) {
	WaitForSingleObject(_thread->handle, INFINITE);
	CloseHandle(_thread->handle);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_setup_thread () {
    return;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_kill_thread ( ex_thread_t *_thread ) {
	TerminateThread(_thread->handle, FALSE);
}
