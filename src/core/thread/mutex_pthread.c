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

ex_mutex_t *ex_create_mutex ()
{
    ex_mutex_t *mutex = NULL;
    pthread_mutexattr_t attr;

    /* Allocate the structure */
    mutex = (ex_mutex_t *)ex_malloc_nomng(sizeof(ex_mutex_t));
    if (mutex) {
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        if (pthread_mutex_init(&mutex->id, &attr) != 0) {
			ex_error("pthread_mutex_init() failed");
            ex_free_nomng(mutex);
            mutex = NULL;
        }
    } else {
        ex_error ("out of memory!");
    }

    return mutex;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_mutex ( ex_mutex_t *_mutex )
{
    if (_mutex) {
        pthread_mutex_destroy(&_mutex->id);
        ex_free_nomng(_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_lock ( ex_mutex_t *_mutex )
{
    int retval;

    //
    if ( _mutex == NULL ) {
        ex_error( "Passed a NULL mutex" );
        return -1;
    }

    //
    retval = 0;
    if ( pthread_mutex_lock(&_mutex->id) < 0 ) {
        ex_error( "pthread_mutex_lock() failed" );
        retval = -1;
    }

    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_unlock ( ex_mutex_t *_mutex )
{
    int retval;

    //
    if ( _mutex == NULL ) {
        ex_error( "Passed a NULL mutex" );
        return -1;
    }

    //
    retval = 0;
    if ( pthread_mutex_unlock(&_mutex->id) < 0 ) {
        ex_error( "pthread_mutex_unlock() failed" );
        retval = -1;
    }

    //
    return retval;
}

