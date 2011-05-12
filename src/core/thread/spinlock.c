// ======================================================================================
// File         : spinlock.c
// Author       : Wu Jie 
// Last Change  : 05/12/2011 | 10:21:45 AM | Thursday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

int ex_atomic_try_lock ( ex_spin_lock_t *_lock ) {

#if (EX_PLATFORM == EX_WIN32)

    return (InterlockedExchange((long*)_lock, 1) == 0);

#elif (EX_PLATFORM == EX_MACOSX)

    return OSAtomicCompareAndSwap32Barrier(0, 1, _lock);

#elif (EX_PLATFORM == EX_IOS)

    return (__sync_lock_test_and_set(_lock, 1) == 0);

#elif defined(__GNUC__) && defined(__arm__) && \
    (defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || \
     defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5TE__))
    int result;
    __asm__ __volatile__ (
                          "swp %0, %1, [%2]\n"
                          : "=&r,&r" (result) : "r,0" (1), "r,r" (_lock) : "memory");
    return (result == 0);

#elif defined(__GNUC__) && defined(__arm__)

    int result;
    __asm__ __volatile__ (
                          "ldrex %0, [%2]\nteq   %0, #0\nstrexeq %0, %1, [%2]"
                          : "=&r" (result) : "r" (1), "r" (_lock) : "cc", "memory");
    return (result == 0);

#else

    /* Need CPU instructions for spinlock here! */
    __need_spinlock_implementation__

#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_atomic_lock ( ex_spin_lock_t *_lock ) {
    /* FIXME: Should we have an eventual timeout? */
    while (!ex_atomic_try_lock (_lock)) {
        ex_sleep(0);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_atomic_unlock ( ex_spin_lock_t *_lock ) {

#if (EX_PLATFORM == EX_WIN32)

    _ReadWriteBarrier();
    *_lock = 0;

#elif (EX_PLATFORM == EX_IOS)  

    __sync_lock_release(_lock);

#else

    *_lock = 0;

#endif
}
