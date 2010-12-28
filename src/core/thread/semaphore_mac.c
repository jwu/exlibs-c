// ======================================================================================
// File         : semaphore_mac.c
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 15:13:33 PM | Tuesday,December
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
    uint32 count;
    uint32 waiters_count;
    ex_mutex_t* count_lock;
    ex_cond_t* count_nonzero;
};

// ------------------------------------------------------------------ 
// Desc: Create a semaphore, initialized with value
// ------------------------------------------------------------------ 

ex_semaphore_t* ex_create_semaphore ( uint32 _initial_value ) {
    ex_semaphore_t* sem = (ex_semaphore_t*) ex_malloc_nomng( sizeof(ex_semaphore_t) );
    if (!sem) {
        ex_error("out of memory!");
        return NULL;
    }
    sem->count = _initial_value;
    sem->waiters_count = 0;

    sem->count_lock = ex_create_mutex();
    sem->count_nonzero = ex_create_cond();
    if ( !sem->count_lock || !sem->count_nonzero ) {
        ex_destroy_semaphore(sem);
        return NULL;
    }

    return sem;
}

// ------------------------------------------------------------------ 
// Desc: You cannot call this function when another thread is using the semaphore.
// ------------------------------------------------------------------ 

void ex_destroy_semaphore ( ex_semaphore_t* _sem ) {
    if (_sem) {
        _sem->count = 0xFFFFFFFF;
        while (_sem->waiters_count > 0) {
            ex_cond_signal(_sem->count_nonzero);
            ex_sleep(10);
        }
        ex_destroy_cond(_sem->count_nonzero);
        if (_sem->count_lock) {
            ex_mutex_lock(_sem->count_lock);
            ex_mutex_unlock(_sem->count_lock);
            ex_destroy_mutex(_sem->count_lock);
        }
        ex_free_nomng(_sem);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_try_wait ( ex_semaphore_t* _sem ) {
    int retval;

    if (!_sem) {
        ex_error("Passed a NULL semaphore");
        return -1;
    }

    retval = EX_MUTEX_TIMEOUT;
    ex_mutex_lock (_sem->count_lock);
    if (_sem->count > 0) {
        --_sem->count;
        retval = 0;
    }
    ex_mutex_unlock (_sem->count_lock);
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait ( ex_semaphore_t* _sem ) {
    return ex_semaphore_wait_timeout(_sem, EX_MUTEX_MAXWAIT);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_wait_timeout ( ex_semaphore_t* _sem, uint32 _timeout ) {
    int retval;

    if (!_sem) {
        ex_error("Passed a NULL semaphore");
        return -1;
    }

    // A timeout of 0 is an easy case
    if (_timeout == 0) {
        return ex_semaphore_try_wait(_sem);
    }

    ex_mutex_lock(_sem->count_lock);
    ++_sem->waiters_count;
    retval = 0;
    while ((_sem->count == 0) && (retval != EX_MUTEX_TIMEOUT)) {
        retval = ex_cond_wait_timeout (_sem->count_nonzero, _sem->count_lock, _timeout);
    }
    --_sem->waiters_count;
    if (retval == 0) {
        --_sem->count;
    }
    ex_mutex_unlock(_sem->count_lock);
    return retval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint32 ex_semaphore_value ( ex_semaphore_t* _sem ) {
    uint32 value;
    value = 0;
    if (_sem) {
        ex_mutex_lock(_sem->count_lock);
        value = _sem->count;
        ex_mutex_unlock(_sem->count_lock);
    }
    return value;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_semaphore_post ( ex_semaphore_t* _sem ) {
    if (!_sem) {
        ex_error("Passed a NULL semaphore");
        return -1;
    }

    ex_mutex_lock(_sem->count_lock);
    if (_sem->waiters_count > 0) {
        ex_cond_signal(_sem->count_nonzero);
    }
    ++_sem->count;
    ex_mutex_unlock(_sem->count_lock);

    return 0;
}

