// ======================================================================================
// File         : cond_mac.c
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 18:50:42 PM | Tuesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

struct ex_cond_t {
    pthread_cond_t cond;
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_cond_t* ex_create_cond () {
    ex_cond_t* cond;

    cond = (ex_cond_t *) ex_malloc_nomng (sizeof(ex_cond_t));
    if (cond) {
        if ( pthread_cond_init(&cond->cond, NULL) < 0 ) {
            ex_error("pthread_cond_init() failed");
            ex_free_nomng(cond);
            cond = NULL;
        }
    }
    return (cond);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_cond ( ex_cond_t* _cond ) {
    if (_cond) {
        pthread_cond_destroy(&_cond->cond);
        ex_free_nomng(_cond);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// Restart one of the threads that are waiting on the condition variable.
// return 0 or -1 on error.
// ------------------------------------------------------------------ 

int ex_cond_signal ( ex_cond_t* _cond ) {
    int retval;

    if (!_cond) {
        ex_error("Passed a NULL condition variable");
        return -1;
    }

    retval = 0;
    if ( pthread_cond_signal(&_cond->cond) != 0 ) {
        ex_error("pthread_cond_signal() failed");
        retval = -1;
    }
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// Restart all threads that are waiting on the condition variable.
// return 0 or -1 on error.
// ------------------------------------------------------------------ 

int ex_cond_broadcast ( ex_cond_t* _cond ) {
    int retval;

    if (!_cond) {
        ex_error("Passed a NULL condition variable");
        return -1;
    }

    retval = 0;
    if (pthread_cond_broadcast(&_cond->cond) != 0) {
        ex_error("pthread_cond_broadcast() failed");
        retval = -1;
    }
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// Wait on the condition variable, unlocking the provided mutex.
// 
// \warning The mutex must be locked before entering this function!
// 
// The mutex is re-locked once the condition variable is signaled.
// 
// \return 0 when it is signaled, or -1 on error.
// ------------------------------------------------------------------ 

int ex_cond_wait ( ex_cond_t* _cond, ex_mutex_t* _mutex ) {
    int retval;

    if (!_cond) {
        ex_error("Passed a NULL condition variable");
        return -1;
    }

    retval = 0;
    if (pthread_cond_wait(&_cond->cond, &_mutex->id) != 0) {
        ex_error("pthread_cond_wait() failed");
        retval = -1;
    }
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// Waits for at most \c ms milliseconds, and returns 0 if the condition
// variable is signaled, ::EX_MUTEX_TIMEOUT if the condition is not
// signaled in the allotted time, and -1 on error.
//
// \warning On some platforms this function is implemented by looping with a 
//          delay of 1 ms, and so should be avoided if possible.
// ------------------------------------------------------------------ 

int ex_cond_wait_timeout ( ex_cond_t* _cond, ex_mutex_t* _mutex, uint32 _ms ) {
    int retval;
    struct timeval delta;
    struct timespec abstime;

    if (!_cond) {
        ex_error("Passed a NULL condition variable");
        return -1;
    }

    gettimeofday(&delta, NULL);

    abstime.tv_sec = delta.tv_sec + (_ms / 1000);
    abstime.tv_nsec = (delta.tv_usec + (_ms % 1000) * 1000) * 1000;
    if (abstime.tv_nsec > 1000000000) {
        abstime.tv_sec += 1;
        abstime.tv_nsec -= 1000000000;
    }

  tryagain:
    retval = pthread_cond_timedwait(&_cond->cond, &_mutex->id, &abstime);
    switch (retval) {
    case EINTR:
        goto tryagain;
        break;
    case ETIMEDOUT:
        retval = EX_MUTEX_TIMEOUT;
        break;
    case 0:
        break;
    default:
        ex_error("pthread_cond_timedwait() failed");
        retval = -1;
        break;
    }
    return retval;
}

