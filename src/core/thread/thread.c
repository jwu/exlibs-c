// ======================================================================================
// File         : thread.c
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 14:24:39 PM | Tuesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// internal structures
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: thread_args_t
// ------------------------------------------------------------------ 

typedef int (*user_pfn_t) (void *);
typedef struct thread_args_t {
    user_pfn_t user_pfn;
    void* data;
    ex_thread_t* info;
    ex_semaphore_t* wait;
} thread_args_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int __create_thread ( ex_thread_t* _thread, void* _args );
extern void __wait_thread ( ex_thread_t* _thread );
extern void __setup_thread ();

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_thread_t* ex_create_thread ( ex_thread_pfn_t _fn, void* _data ) {
    ex_thread_t* thread;
    thread_args_t* args;
    int ret;

    // Allocate memory for the thread info structure
    thread = (ex_thread_t*) ex_malloc_nomng(sizeof(ex_thread_t));
    if ( thread == NULL ) {
        ex_error ("out of memory!");
        return NULL;
    }
    memset( thread, 0, sizeof(ex_thread_t) );
    thread->status = -1;

    // Set up the arguments for the thread
    args = (thread_args_t*) ex_malloc_nomng(sizeof(thread_args_t));
    if ( args == NULL ) {
        ex_error ("out of memory!");
        ex_free_nomng(thread);
        return NULL;
    }
    args->user_pfn = _fn;
    args->data = _data;
    args->info = thread;
    args->wait = ex_create_semaphore(0);
    if ( args->wait == NULL ) {
        ex_free_nomng(thread);
        ex_free_nomng(args);
        return NULL;
    }

    // jwu DISABLE: I think this just used for manage thread { 
    // /* Add the thread to the list of available threads */
    // SDL_AddThread(thread);
    // } jwu DISABLE end 

    // Create the thread and go!
    ret = __create_thread(thread, args);
    if ( ret >= 0 ) {
        // Wait for the thread function to use arguments
        ex_semaphore_wait(args->wait);
    } else {
        // Oops, failed.  Gotta free everything
        // jwu DISABLE: same reason as above { 
        // SDL_DelThread(thread);
        // } jwu DISABLE end 
        ex_free_nomng(thread);
        thread = NULL;
    }
    ex_destroy_semaphore(args->wait);
    ex_free_nomng(args);

    // Everything is running now
    return thread;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_wait_thread ( ex_thread_t* _thread, int* _status ) {
    if ( _thread ) {
        __wait_thread(_thread);
        if ( _status ) {
            *_status = _thread->status;
        }
        // jwu DISABLE: same reason as above { 
        // SDL_DelThread(thread);
        // } jwu DISABLE end 
        ex_free_nomng (_thread);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_run_thread ( void* _data ) {
    thread_args_t* args;
    user_pfn_t user_pfn;
    void* user_data;
    int* status_loc;

    // Perform any system-dependent setup
    // this function cannot fail
    __setup_thread();

    // Get the thread id
    args = (thread_args_t*)_data;
    args->info->threadID = ex_current_threadID();

    // Figure out what function to run
    user_pfn = args->user_pfn;
    user_data = args->data;
    status_loc = &args->info->status;

    // Wake up the parent thread
    ex_semaphore_post(args->wait);

    // Run the function
    *status_loc = user_pfn(user_data);
}
