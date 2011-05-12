// ======================================================================================
// File         : semaphore_pthread.c
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 18:18:33 PM | Tuesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define EX_SEMAPHORE_TIMEOUT 1
#define EX_SEMAPHORE_MAXWAIT (~(uint32)0)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

struct ex_semaphore_t {
    sem_t sem;
};

// ------------------------------------------------------------------ 
// Desc: Create a semaphore, initialized with value
// ------------------------------------------------------------------ 

ex_semaphore_t *ex_create_semaphore ( uint32 _initial_value ) {
    ex_semaphore_t *sem = (ex_semaphore_t *) ex_malloc_nomng( sizeof(ex_semaphore_t) );
    if (sem) {
        if ( sem_init( &sem->sem, 0, _initial_value ) < 0 ) {
            ex_error( "sem_init() failed" );
            ex_free_nomng(sem);
            sem = NULL;
        }
    } else {
        ex_error("out of memeory");
    }
    return sem;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_semaphore ( ex_semaphore_t *_sem ) {
    if (_sem) {
        sem_destroy(&_sem->sem);
        ex_free_nomng(_sem);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_try_wait ( ex_semaphore_t *_sem ) {
    int retval;

    if (!_sem) {
        ex_error( "Passed a NULL semaphore" );
        return -1;
    }
    retval = EX_SEMAPHORE_TIMEOUT;
    if ( sem_trywait(&_sem->sem) == 0 ) {
        retval = 0;
    }
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait ( ex_semaphore_t *_sem ) {
    int retval;

    if (!_sem) {
        ex_error( "Passed a NULL semaphore" );
        return -1;
    }

    retval = sem_wait(&_sem->sem);
    if (retval < 0) {
        ex_error( "sem_wait() failed" );
    }
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait_timeout ( ex_semaphore_t *_sem, uint32 _timeout ) {
    int retval;
    struct timeval now;
    struct timespec ts_timeout;

    if (!_sem) {
        ex_error( "Passed a NULL semaphore" );
        return -1;
    }

    // Try the easy cases first
    if ( _timeout == 0 ) {
        return ex_semaphore_try_wait (_sem);
    }
    if ( _timeout == EX_SEMAPHORE_MAXWAIT ) {
        return ex_semaphore_wait (_sem);
    }

    /* Setup the timeout. sem_timedwait doesn't wait for
    * a lapse of time, but until we reach a certain time.
    * This time is now plus the timeout.
    */
    gettimeofday(&now, NULL);

    /* Add our timeout to current time */
    now.tv_usec += (timeout % 1000) * 1000;
    now.tv_sec += timeout / 1000;

    /* Wrap the second if needed */
    if ( now.tv_usec >= 1000000 ) {
        now.tv_usec -= 1000000;
        now.tv_sec ++;
    }

    /* Convert to timespec */
    ts_timeout.tv_sec = now.tv_sec;
    ts_timeout.tv_nsec = now.tv_usec * 1000;

    /* Wait. */
    do {
        retval = sem_timedwait(&sem->sem, &ts_timeout);
    } while (retval < 0 && errno == EINTR);

    if (retval < 0) {
        SDL_SetError("sem_timedwait() failed");
    }

    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint32 ex_semaphore_value ( ex_semaphore_t *_sem ) {
    int ret = 0;
    if (_sem) {
        sem_getvalue(&_sem->sem, &ret);
        if (ret < 0) {
            ret = 0;
        }
    }
    return (uint32) ret;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_post ( ex_semaphore_t *_sem ) {
    int retval;

    if (!_sem) {
        ex_error("Passed a NULL semaphore");
        return -1;
    }

    retval = sem_post(&_sem->sem);
    if (retval < 0) {
        ex_error("sem_post() failed");
    }
    return retval;
}

