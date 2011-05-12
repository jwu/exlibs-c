// ======================================================================================
// File         : atomic.h
// Author       : Wu Jie 
// Last Change  : 05/12/2011 | 09:55:12 AM | Thursday,May
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ATOMIC_H_1305165314
#define ATOMIC_H_1305165314
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

typedef int ex_spin_lock_t;
typedef struct { int value; } ex_atomic_t;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_atomic_try_lock ( ex_spin_lock_t *_lock );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_atomic_lock ( ex_spin_lock_t *_lock );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_atomic_unlock ( ex_spin_lock_t *_lock );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#if (EX_COMPILER == EX_MSVC)
    void _ReadWriteBarrier(void);
    #pragma intrinsic(_ReadWriteBarrier)
    #define ex_compiler_barrier()  _ReadWriteBarrier()
#elif (EX_COMPILER == EX_GCC)
    #define ex_compiler_barrier()  __asm__ __volatile__ ("" : : : "memory")
#else
    #define ex_compiler_barrier()  ({ ex_spin_lock_t _tmp = 0; ex_atomic_lock(&_tmp); ex_atomic_unlock(&_tmp); })
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#if (EX_PLATFORM == EX_WIN32)

    #define ex_atomic_set(a, v)     _InterlockedExchange((long*)&(a)->value, (v))
    #define ex_atomic_add(a, v)     _InterlockedExchangeAdd((long*)&(a)->value, (v))
    #define ex_atomic_cas(a, oldval, newval) (_InterlockedCompareExchange((long*)&(a)->value, (newval), (oldval)) == (oldval))
    #define ex_atomic_setptr(a, v)  _InterlockedExchangePointer((a), (v))
    #if _M_IX86
        #define ex_atomic_casptr(a, oldval, newval) (_InterlockedCompareExchange((long*)(a), (long)(newval), (long)(oldval)) == (long)(oldval))
    #else
        #define ex_atomic_casptr(a, oldval, newval) (_InterlockedCompareExchangePointer((a), (newval), (oldval)) == (oldval))
    #endif

#elif (EX_PLATFORM == EX_MACOSX)

    #define ex_atomic_cas(a, oldval, newval) OSAtomicCompareAndSwap32Barrier((oldval), (newval), &(a)->value)
    #ifdef __LP64__
        #define ex_atomic_casptr(a, oldval, newval) OSAtomicCompareAndSwap64Barrier((int64_t)(oldval), (int64_t)(newval), (int64_t*)(a))
    #else
        #define ex_atomic_casptr(a, oldval, newval) OSAtomicCompareAndSwap32Barrier((int32_t)(oldval), (int32_t)(newval), (int32_t*)(a))
    #endif

#elif (EX_PLATFORM == EX_IOS)  

    #define ex_atomic_set(a, v)     __sync_lock_test_and_set(&(a)->value, v)
    #define ex_atomic_add(a, v)     __sync_fetch_and_add(&(a)->value, v)
    #define ex_atomic_setptr(a, v)  __sync_lock_test_and_set(a, v)
    #define ex_atomic_cas(a, oldval, newval) __sync_bool_compare_and_swap(&(a)->value, oldval, newval)
    #define ex_atomic_casptr(a, oldval, newval) __sync_bool_compare_and_swap(a, oldval, newval)

#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_cas
#define ex_atomic_cas ex_atomic_cas_
#endif
extern int ex_atomic_cas_( ex_atomic_t *_a, int _oldval, int _newval );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_set
static inline int ex_atomic_set( ex_atomic_t *_a, int _v ) {
    int value;
    do {
        value = _a->value;
    } while ( !ex_atomic_cas(_a, value, _v) );
    return value;
}
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_get
static inline int ex_atomic_get ( ex_atomic_t *_a )
{
    int value = _a->value;
    ex_compiler_barrier();
    return value;
}
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_add
static inline int ex_atomic_add( ex_atomic_t *_a, int _v )
{
    int value;
    do {
        value = _a->value;
    } while (!ex_atomic_cas(_a, value, (value + _v)));
    return value;
}
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_incref
#define ex_atomic_incref(a)    ex_atomic_add(a, 1)
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_decref
#define ex_atomic_decref(a)    (ex_atomic_add(a, -1) == 1)
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_casptr
#define ex_atomic_casptr ex_atomic_casptr_
#endif
extern int ex_atomic_casptr_ ( void **_a, void *_oldval, void *_newval );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_setptr
static inline void *ex_atomic_setptr( void **_a, void *_v )
{
    void *value;
    do {
        value = *_a;
    } while ( !ex_atomic_casptr(_a, value, _v) );
    return value;
}
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#ifndef ex_atomic_getptr
static inline void *ex_atomic_getptr( void **_a )
{
    void* value = *_a;
    ex_compiler_barrier();
    return value;
}
#endif

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END ATOMIC_H_1305165314
// #################################################################################


