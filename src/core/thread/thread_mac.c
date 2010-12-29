// ======================================================================================
// File         : thread_mac.c
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 13:44:20 PM | Tuesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static const int __sig_list[] = {
    SIGHUP, 
    SIGINT, 
    SIGQUIT, 
    SIGPIPE, 
    SIGALRM, 
    SIGTERM, 
    SIGCHLD, 
    SIGWINCH,
    SIGVTALRM, 
    SIGPROF, 
    0
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_run_thread ( void* );
static void* __run_thread (void* _data) {
    ex_run_thread(_data);
    pthread_exit((void *) 0);
    return ((void *) 0); // Prevent compiler warning
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_sys_create_thread ( ex_thread_t* _thread, void* _args ) {
    pthread_attr_t type;

    // Set the thread attributes
    if ( pthread_attr_init(&type) != 0 ) {
        ex_error ( "Couldn't initialize pthread attributes" );
        return -1;
    }
    pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);

    // Create the thread and go!
    if ( pthread_create(&_thread->handle, &type, __run_thread, _args) != 0 ) {
        ex_error ( "Not enough resources to create thread" );
        return -1;
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_wait_thread ( ex_thread_t* _thread ) {
    pthread_join(_thread->handle, 0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_setup_thread () {
    int i;
    sigset_t mask;

    // Mask asynchronous signals for this thread
    sigemptyset(&mask);
    for ( i = 0; __sig_list[i]; ++i ) {
        sigaddset(&mask, __sig_list[i]);
    }
    pthread_sigmask(SIG_BLOCK, &mask, 0);

// jwu DISABLE: don't know why we need this { 
// #ifdef PTHREAD_CANCEL_ASYNCHRONOUS
//     // Allow ourselves to be asynchronously cancelled
//     {
//         int oldstate;
//         pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate);
//     }
// #endif
// } jwu DISABLE end 
}
