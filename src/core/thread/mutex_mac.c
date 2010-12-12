// ======================================================================================
// File         : mutex_mac.c
// Author       : Wu Jie 
// Last Change  : 12/03/2010 | 02:03:19 AM | Friday,December
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

struct ex_mutex_t { 
    pthread_mutex_t id;
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_mutex_t* ex_mutex_create ()
{
    ex_mutex_t* mutex = NULL;
    pthread_mutexattr_t attr;

    /* Allocate the structure */
    mutex = (ex_mutex_t*)ex_malloc_nomng(sizeof(ex_mutex_t));
    if (mutex) {
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        if (pthread_mutex_init(&mutex->id, &attr) != 0) {
			ex_error("pthread_mutex_init() failed");
            ex_free_nomng(mutex);
            mutex = NULL;
        }
    } 
    // TODO { 
    // else {
    //     SDL_OutOfMemory();
    // }
    // } TODO end 

    return mutex;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_mutex_destroy ( ex_mutex_t* _mutex )
{
    if (_mutex) {
        pthread_mutex_destroy(&_mutex->id);
        ex_free_nomng(_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_lock ( ex_mutex_t* _mutex )
{
// TODO { 
//     int retval;
// #if FAKE_RECURSIVE_MUTEX
//     pthread_t this_thread;
// #endif

//     if (mutex == NULL) {
//         SDL_SetError("Passed a NULL mutex");
//         return -1;
//     }

//     retval = 0;
// #if FAKE_RECURSIVE_MUTEX
//     this_thread = pthread_self();
//     if (mutex->owner == this_thread) {
//         ++mutex->recursive;
//     } else {
//         /* The order of operations is important.
//            We set the locking thread id after we obtain the lock
//            so unlocks from other threads will fail.
//          */
//         if (pthread_mutex_lock(&mutex->id) == 0) {
//             mutex->owner = this_thread;
//             mutex->recursive = 0;
//         } else {
//             SDL_SetError("pthread_mutex_lock() failed");
//             retval = -1;
//         }
//     }
// #else
//     if (pthread_mutex_lock(&mutex->id) < 0) {
//         SDL_SetError("pthread_mutex_lock() failed");
//         retval = -1;
//     }
// #endif
//     return retval;
// } TODO end 

    return(0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_unlock ( ex_mutex_t* _mutex )
{
// TODO { 
//     int retval;

//     if (mutex == NULL) {
//         SDL_SetError("Passed a NULL mutex");
//         return -1;
//     }

//     retval = 0;
// #if FAKE_RECURSIVE_MUTEX
//     /* We can only unlock the mutex if we own it */
//     if (pthread_self() == mutex->owner) {
//         if (mutex->recursive) {
//             --mutex->recursive;
//         } else {
//             /* The order of operations is important.
//                First reset the owner so another thread doesn't lock
//                the mutex and set the ownership before we reset it,
//                then release the lock semaphore.
//              */
//             mutex->owner = 0;
//             pthread_mutex_unlock(&mutex->id);
//         }
//     } else {
//         SDL_SetError("mutex not owned by this thread");
//         retval = -1;
//     }

// #else
//     if (pthread_mutex_unlock(&mutex->id) < 0) {
//         SDL_SetError("pthread_mutex_unlock() failed");
//         retval = -1;
//     }
// #endif /* FAKE_RECURSIVE_MUTEX */

//     return retval;
// } TODO end 

	return(0);
}

