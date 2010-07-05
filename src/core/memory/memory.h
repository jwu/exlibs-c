// ======================================================================================
// File         : memory.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 13:25:37 PM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MEMORY_H_1277529938
#define MEMORY_H_1277529938
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// config
///////////////////////////////////////////////////////////////////////////////

#define EX_MEM_ALIGN 16

///////////////////////////////////////////////////////////////////////////////
// private memory functions
///////////////////////////////////////////////////////////////////////////////

void* _mng_malloc( size_t _size, const char* _tag, const char* _file_name, const char* _func_name, size_t _line_nr );
void* _mng_realloc( void* _ptr, size_t _size, const char* _tag, const char* _file_name, const char* _func_name, size_t _line_nr );
void _mng_free( void* _ptr, const char* _file_name, const char* _func_name, size_t _line_nr );

///////////////////////////////////////////////////////////////////////////////
// memory mng
///////////////////////////////////////////////////////////////////////////////

void mem_init ();
void mem_deinit ();

///////////////////////////////////////////////////////////////////////////////
// global namespace function declaration & define 
///////////////////////////////////////////////////////////////////////////////

// -----------------------------------
#if EX_USE_SYSTEM_MEMORY
// -----------------------------------

    // malloc
    #define ex_malloc_nomng( size )                         malloc(size)
    #define ex_malloc( size )                               malloc(size)
    #define ex_malloc_tag( size, tag )                      malloc(size)

    // realloc
    #define ex_realloc_nomng( ptr, size )                   realloc(ptr,size)
    #define ex_realloc( ptr, size )                         realloc(ptr,size)
    #define ex_realloc_tag( ptr, size, tag )                realloc(ptr,size)

    // free
    #define ex_free_nomng( ptr )                            free(ptr)
    #define ex_free( ptr )                                  free(ptr)

// -----------------------------------
#else // use managed memory
// -----------------------------------

///////////////////////////////////////////////////////////////////////////////
// ex_malloc
///////////////////////////////////////////////////////////////////////////////

#define ex_malloc_nomng( size )             dlmemalign( EX_MEM_ALIGN, size)

#if EX_USE_MEMORY_MANAGER
    #define ex_malloc( size )                               _mng_malloc( size, "default", __FILE__, __FUNCTION__, __LINE__ )
    #define ex_malloc_tag( size, tag )                      _mng_malloc( size, tag, __FILE__, __FUNCTION__, __LINE__ ) 
#else
    #define ex_malloc( size )                               ex_malloc_nomng( size )
    #define ex_malloc_tag( size, tag )                      ex_malloc_nomng( size )
#endif

///////////////////////////////////////////////////////////////////////////////
// ex_realloc
///////////////////////////////////////////////////////////////////////////////

#define ex_realloc_nomng( ptr, size )       dlrealloc(ptr,size)

#if EX_USE_MEMORY_MANAGER
    #define ex_realloc( ptr, size )                             _mng_realloc( ptr, size, "default", __FILE__, __FUNCTION__, __LINE__ )
    #define ex_realloc_tag( ptr, size, tag )                    _mng_realloc( ptr, size, tag, __FILE__, __FUNCTION__, __LINE__ )
#else
    #define ex_realloc( ptr, size )                             ex_realloc_nomng( ptr, size )
    #define ex_realloc_tag( ptr, size, tag )                    ex_realloc_nomng( ptr, size ) 
#endif

///////////////////////////////////////////////////////////////////////////////
// free
///////////////////////////////////////////////////////////////////////////////

// ========================================================
// free
// ========================================================

#define ex_free_nomng( ptr )                dlfree( ptr )

#if EX_USE_MEMORY_MANAGER
    #define ex_free( ptr )                                  _mng_free( ptr, __FILE__, __FUNCTION__, __LINE__ )
#else
    #define ex_free( ptr )                                  ex_free_nomng( ptr )
#endif

// -----------------------------------
#endif // end if EX_USE_SYSTEM_MEMORY
// -----------------------------------

// ex_safe_free_nomng
#define ex_safe_free_nomng(ptr) \
    if ( ptr ) { \
        ex_free_nomng(ptr); \
        ptr = NULL; \
    }

// ex_safe_free
#define ex_safe_free( ptr ) \
    if ( ptr ) { \
        ex_free(ptr); \
        ptr = NULL; \
    }

///////////////////////////////////////////////////////////////////////////////
// common memory operations
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void* memzero ( void* _ptr, size_t _size ) { 
    return memset ( _ptr, 0x00, _size ); 
}

// ------------------------------------------------------------------
// Desc: mem swap
// ------------------------------------------------------------------

// TODO { 
// inline void memswap( void* _p1, void* _p2, size_t _size ) {
    // void* pTmp = ex_stack_malloc(EX_ALIGN_ROUND(_size,EX_DEFAULT_ALIGNMENT));

    // memcpy( pTmp, _p1,  _size );
    // memcpy( _p1,  _p2,  _size );
    // memcpy( _p2,  pTmp, _size );
// }
// } TODO end 

// #################################################################################
#endif // END MEMORY_H_1277529938
// #################################################################################


