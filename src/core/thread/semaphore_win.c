// ======================================================================================
// File         : semaphore_win32.c
// Author       : Wu Jie 
// Last Change  : 02/06/2011 | 11:48:57 AM | Sunday,February
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

struct ex_semaphore_t {
    HANDLE id;
    uint32 volatile count;
};

// ------------------------------------------------------------------ 
// Desc: Create a semaphore, initialized with value
// ------------------------------------------------------------------ 

ex_semaphore_t *ex_create_semaphore ( uint32 _initial_value ) {
    ex_semaphore_t *sem = (ex_semaphore_t *) ex_malloc_nomng( sizeof(ex_semaphore_t) );
    if (!sem) {
        ex_error("out of memory!");
        return NULL;
    }

    sem->id = CreateSemaphore(NULL, _initial_value, 32*1024, NULL);
    sem->count = _initial_value;
    if ( !sem->id ) {
        ex_error ("couldn't create semaphore");
        ex_free_nomng(sem);
        sem = NULL;
    }

    return sem;
}

// ------------------------------------------------------------------ 
// Desc: You cannot call this function when another thread is using the semaphore.
// ------------------------------------------------------------------ 

void ex_destroy_semaphore ( ex_semaphore_t *_sem ) {
    if (_sem) {
        _sem->count = 0xFFFFFFFF;
        if ( _sem->id ) {
            CloseHandle(_sem->id);
            _sem->id = 0;
        }
        ex_free_nomng(_sem);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_try_wait ( ex_semaphore_t *_sem ) {
    return ex_semaphore_wait_timeout(_sem,0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait ( ex_semaphore_t *_sem ) {
    return ex_semaphore_wait_timeout(_sem, EX_MUTEX_MAXWAIT);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait_timeout ( ex_semaphore_t *_sem, uint32 _timeout ) {
	int retval;
	DWORD dwMilliseconds;

	if ( !_sem ) {
		ex_error("Passed a NULL _sem");
		return -1;
	}

    if ( _timeout == EX_MUTEX_MAXWAIT )
        dwMilliseconds = INFINITE;
    else
        dwMilliseconds = (DWORD)_timeout;

    switch ( WaitForSingleObject(_sem->id, dwMilliseconds) ) {
    case WAIT_OBJECT_0:
        InterlockedDecrement(&_sem->count);
        retval = 0;
        break;

    case WAIT_TIMEOUT:
        retval = EX_MUTEX_TIMEOUT;
        break;

    default:
        ex_error( "WaitForSingleObject() failed" );
        retval = -1;
        break;
    }
	return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint32 ex_semaphore_value ( ex_semaphore_t *_sem ) {
    if ( !_sem ) {
        ex_error("Passed a NULL semaphore");
        return 0;
    }
    return _sem->count;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_post ( ex_semaphore_t *_sem ) {
	if ( !_sem ) {
		ex_error("Passed a NULL semaphore");
		return -1;
	}

	/* Increase the counter in the first place, because
	 * after a successful release the semaphore may
	 * immediately get destroyed by another thread which
	 * is waiting for this semaphore.
	 */
	InterlockedIncrement(&_sem->count);
	if ( ReleaseSemaphore(_sem->id, 1, NULL) == FALSE ) {
		InterlockedDecrement(&_sem->count);	/* restore */
		ex_error( "ReleaseSemaphore() failed" );
		return -1;
	}
	return 0;
}

