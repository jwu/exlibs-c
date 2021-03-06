// ======================================================================================
// File         : memmng.c
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 07:43:17 AM | Monday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// config
///////////////////////////////////////////////////////////////////////////////

#define MAX_HASH            4096
#define MAX_NEW_ALLOCINFO   256
// DISABLE: #define MEM_LOG             "./mem_dump.err"

#define VERIFY_PREFIX       1
#define VERIFY_SUFFIX       1
#define VERIFY_FREE         1
#define VERIFY_REALLOC      1

// |________________| |________________________| |_________________|
//  PRE_PATTERN_SIZE        _size_to_alloc        POST_PATTERN_SIZE

#define PREFIX_COUNT        4 
#define PREFIX_PATTERN      0xABBCCDDA

#define SUFFIX_COUNT        4 
#define SUFFIX_PATTERN      0xFEEDDCCF

///////////////////////////////////////////////////////////////////////////////
// typedefs
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct alloc_unit_t {
    size_t               org_size;
    size_t               dbg_size;
    void                *org_addr;
    void                *dbg_addr;
    const char          *tag_name;
    const char          *file_name;
    const char          *func_name;
    size_t               line_nr;
    bool                 break_onfree;
    bool                 break_onrealloc;
    size_t               alloc_nr;
    thread_id_t          thread_id;
    struct alloc_unit_t *next;
    struct alloc_unit_t *prev;
} alloc_unit_t; // end struct alloc_unit_t 

///////////////////////////////////////////////////////////////////////////////
// variables
///////////////////////////////////////////////////////////////////////////////

static const size_t __prefix_size = sizeof(uint32) * PREFIX_COUNT;
static const size_t __suffix_size = sizeof(uint32) * SUFFIX_COUNT;
static const size_t __pattern_size = sizeof(uint32) * PREFIX_COUNT + sizeof(uint32) * SUFFIX_COUNT;

static alloc_unit_t *__reserved_au_list = NULL;
static ex_hashmap_t __au_map;
static ex_list_t __au_bucket;
static ex_mutex_t *__access_mutex = NULL;

static size_t __au_count = 0;
static size_t __total_org_memory = 0;
static size_t __total_dbg_memory = 0;
static size_t __accumulate_org_memory = 0;
static size_t __accumulate_dbg_memory = 0;

static bool __initialized = false;

///////////////////////////////////////////////////////////////////////////////
// private
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline size_t __calc_dbg_size ( size_t _size ) {
    return _size + __pattern_size;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static alloc_unit_t *__request_au () {
    alloc_unit_t *tmp_au;

    if ( !__reserved_au_list ) {
        uint i = 0;

        // Allocate MAX_NEW_ALLOCINFO reserve alloc-info
        __reserved_au_list = (alloc_unit_t *)ex_malloc_nomng( MAX_NEW_ALLOCINFO * sizeof(alloc_unit_t) );

        // Assert exAllocInfo != NULL
        ex_assert_return( __reserved_au_list != NULL, NULL, "out of memory!" );

        // Build a linked-list of the elements in our reserve-allocinfo-list
        // ex_memzero setting
        ex_memzero( __reserved_au_list, MAX_NEW_ALLOCINFO * sizeof(alloc_unit_t) );
        for( i = 0; i < MAX_NEW_ALLOCINFO-1; ++i )
        {
            __reserved_au_list[i].next = __reserved_au_list + (i+1);
            __reserved_au_list[i+1].prev = __reserved_au_list + i; 
        }

        // Add this address to our __reserved_au_list so we can free it later
        ex_list_append ( &__au_bucket, __reserved_au_list );
    }

    // this is the standard use of go next and unlink
    tmp_au = __reserved_au_list;
    __reserved_au_list = __reserved_au_list->next;

    // unlink the getted alloc info
    tmp_au->next = NULL;
    tmp_au->prev = NULL;

    //
    return tmp_au;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __verify_pattern ( alloc_unit_t *_au, 
                               const char *_file_name, 
                               const char *_func_name, 
                               size_t _line_nr )
{
    // verify pre-pattern
#if VERIFY_PREFIX
    {
        uint32 *pre  = (uint32 *)_au->dbg_addr;
        uint i_pre;

        //
        for( i_pre = 0; i_pre < PREFIX_COUNT; ++i_pre, ++pre ) {
            if( *pre != PREFIX_PATTERN ) {
                ex_warning( "Memory Prefix Conflict\n"
                            "FielName: %s\n"
                            "Line: %d\n"
                            "FunctionName: %s\n"
                            "Alloc ThreadID: %d\n"
                            "Current ThreadID: %d\n", 
                            _file_name, (int)_line_nr, _func_name, (int)(_au->thread_id), (int)ex_current_threadID() );
                break;
            }
        }
    }
#endif

    // verify post-pattern
#if VERIFY_SUFFIX
    {
        uint32 *post = (uint32 *)( (int8 *)_au->dbg_addr + __suffix_size + _au->org_size );
        uint i_post;

        //
        for( i_post = 0; i_post < SUFFIX_COUNT; ++i_post, ++post ) {
            if( *post != SUFFIX_PATTERN ) {
                ex_warning( "Memory Postfix Conflict\n"
                            "FielName: %s\n"
                            "Line: %d\n"
                            "FunctionName: %s\n"
                            "Alloc ThreadID: %d\n"
                            "Current ThreadID: %d\n", 
                            _file_name, (int)_line_nr, _func_name, (int)(_au->thread_id), (int)ex_current_threadID() );
                break;
            }
        }
    }
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __reclaim_au ( alloc_unit_t *_au ) {
    if ( ex_hashmap_remove_at ( &__au_map, &_au->org_addr ) == NULL )
        ex_error ( "failed to reclaim alloc unit" );

    // append to reserve alloc info
    _au->next = __reserved_au_list;
    _au->prev = NULL;

    // reserve alloc info could be use out
    if ( __reserved_au_list )
        __reserved_au_list->prev = _au;

    __reserved_au_list = _au;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
static inline int __push_au ( alloc_unit_t *_au ) { 
    size_t idx = -1;
    ex_hashmap_insert ( &__au_map, &_au->org_addr, &_au, &idx ); 
    return idx;
}

//
static inline alloc_unit_t *__get_au ( void *_ptr ) { 
    return *((alloc_unit_t **)ex_hashmap_get ( &__au_map, &_ptr, NULL )); 
}

//
static inline int __rearrange_au ( void *_ptr, alloc_unit_t *au ) {
    if ( ex_hashmap_remove_at ( &__au_map, &_ptr ) == NULL )
        return -1;
    return __push_au ( au );
}

// DISABLE: we use ex_log instead { 
// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// static void __log ( const char *_file_name, const char *_format, ... ) {
//     char buffer[2048];
//     FILE *fp;

//     // The Buffer
//     EX_GET_VA_STRING( buffer, 2048, _format );

//     // open the log file
//     fp = fopen( _file_name, "ab" );

//     // The file must be opened
//     ex_assert( fp, "Can't not open the log file:%s", _file_name );

//     // print the log into the file
//     fprintf( fp, "%s", buffer );

//     // close the file
//     fclose(fp);
// }
// } DISABLE end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __dump () {
    //
    ex_assert ( ex_hashmap_count(&__au_map) == 0, "There are %d place(s) exsits memory leak.",  ex_hashmap_count(&__au_map) );
    
    //
    if ( ex_hashmap_count(&__au_map) ) {
        ex_hashmap_each ( &__au_map, alloc_unit_t *, au ) {
            char text[2048];
            ex_memzero ( text, 2048 );
            snprintf ( text, 2048, 
                       "%s:%d: error memory leak\r\n"
                       "function name:  %s\r\n"
                       "tag name:       %s\r\n"
                       "original addr:  %#.8lx\r\n"
                       "debug addr:     %#.8lx\r\n"
                       "original size:  %lu(%#.8lX)\r\n"
                       "debug size:     %lu(%#.8lX)\r\n"
                       "alloc nr:       %lu(%#.8lX)\r\n"
                       "thread ID:      %lu(%#.8lX)\r\n"
                       "\r\n"
                       ,au->file_name ,(int)au->line_nr 
                       ,au->func_name 
                       ,au->tag_name 
                       ,ex_ptr_to_addr(au->org_addr) 
                       ,ex_ptr_to_addr(au->dbg_addr) 
                       ,au->org_size, au->org_size 
                       ,au->dbg_size, au->dbg_size 
                       ,au->alloc_nr, au->alloc_nr 
                       ,au->thread_id, au->thread_id 
                     );

            // format log info
            ex_log( "%s", text ); 
            // DISABLE: use ex_log above instaed { 
            // __log ( "%s", text );
            // printf ( "%s", text ); // also show on the console.
            // } DISABLE end 

        } ex_hashmap_each_end;
    }
}

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mem_init () {

    // if the memmng already initialized, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "memory manager already initialized" );
        return 1;
    }

    //
    __access_mutex = ex_create_mutex();

    ex_hashmap_init( &__au_map, 
                     EX_STRID_NULL, sizeof(void *), 
                     EX_STRID_NULL, sizeof(alloc_unit_t *), 
                     256,
                     ex_hashkey_ptr, ex_keycmp_ptr,
                     __ex_hashmap_alloc_nomng,
                     __ex_hashmap_realloc_nomng,
                     __ex_hashmap_dealloc_nomng
                   );
    ex_list_init ( &__au_bucket,
                   EX_STRID_NULL,
                   sizeof(alloc_unit_t *),
                   __ex_list_alloc_nomng,
                   __ex_list_realloc_nomng,
                   __ex_list_dealloc_nomng
                 );

    // DISABLE: we use ex_log instead { 
    // remove log file if it exists
    // unlink ( MEM_LOG );
    // } DISABLE end 

    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_mem_deinit () {

    if ( __initialized ) {
        if ( __access_mutex )
            ex_destroy_mutex(__access_mutex);

        __dump ();

        //
        ex_hashmap_deinit ( &__au_map );

        // free the reserve alloc info buffer
        ex_list_deinit ( &__au_bucket );

        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_mem_initialized () {
    return __initialized;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_malloc_mng( size_t _size, const char *_tag, const char *_file_name, const char *_func_name, size_t _line_nr )
{
    size_t dbg_size;
    void *dbg_ptr, *org_ptr;
    uint32 *pre, *post;
    uint i_pre, i_post;
    alloc_unit_t *au;

    ex_mutex_lock(__access_mutex);

    // ANSI says: allocation requests of 0 bytes will still return a valid value
    if ( _size == 0) _size = 1;

    // use allocator to allocate memory
    dbg_size = __calc_dbg_size(_size);
    dbg_ptr = dlmemalign( EX_MEM_ALIGN, dbg_size );
    org_ptr = (void *)( (int8 *)dbg_ptr + __prefix_size );

    // get au, if not, create one
    au = __request_au();
    if ( au == NULL ) {
        ex_error( "AllocInfo not found, check if the _ptr have been free or invalid" );
        ex_mutex_unlock(__access_mutex);
        return NULL;
    }
    // write information
    au->org_size         = _size;
    au->dbg_size         = dbg_size;
    au->dbg_addr         = dbg_ptr;
    au->org_addr         = org_ptr;
    au->tag_name         = _tag;
    au->file_name        = _file_name;
    au->func_name        = _func_name;
    au->line_nr          = _line_nr;
    au->break_onfree     = false;
    au->break_onrealloc  = false;
    au->alloc_nr         = __au_count;
    au->thread_id        = ex_current_threadID () ;

    //
    // |________________| |________________________| |_________________|
    //  PRE_PATTERN_SIZE        _size_to_alloc        POST_PATTERN_SIZE
    //

    // add protector
    pre  = (uint32 *)dbg_ptr;
    post = (uint32 *)( (int8 *)org_ptr + _size );

    // add pre&post pattern
    for( i_pre = 0; i_pre < PREFIX_COUNT; ++i_pre, ++pre )
        *pre = PREFIX_PATTERN;
    for( i_post = 0; i_post < SUFFIX_COUNT; ++i_post, ++post )
        *post = SUFFIX_PATTERN;

    // Record memory information
    __au_count += 1;
    __total_org_memory       += au->org_size;      
    __total_dbg_memory       += au->dbg_size;
    __accumulate_org_memory  += au->org_size;
    __accumulate_dbg_memory  += au->dbg_size;

    // finally push the alloc info into hash_list 
    if ( __push_au (au) == -1 ) {
        ex_error ( "failed to insert alloc unit" );
        ex_mutex_unlock(__access_mutex);
        return NULL;
    }

    ex_mutex_unlock(__access_mutex);
    return org_ptr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_realloc_mng( void *_ptr, size_t _size, const char *_tag, const char *_file_name, const char *_func_name, size_t _line_nr )
{
    alloc_unit_t *au;
    size_t dbg_size;
    void *dbg_ptr, *org_ptr;
    uint32 *pre, *post;
    uint i_pre, i_post;

    ex_mutex_lock(__access_mutex);

    // alloc NULL memory address
    if ( _ptr == NULL ) {
        ex_mutex_unlock(__access_mutex);
        return ex_malloc_mng( _size, _tag, _file_name, _func_name, _line_nr );
    }

    // realloc zero bytes free 
    if ( _size == 0 ) {
        ex_free_mng( _ptr, _file_name, _func_name, _line_nr );
        ex_mutex_unlock(__access_mutex);
        return NULL;
    }

    // find au * by address
    au =  __get_au(_ptr);
    if ( au == NULL ) {
        ex_error ( "AllocInfo not found, check if the _ptr have been free or invalid" );
        ex_mutex_unlock(__access_mutex);
        return NULL;
    }

    // verify memory conflic first if needed
#if VERIFY_REALLOC
    __verify_pattern ( au, _file_name, _func_name, _line_nr );
#endif

    // remove original memory information
    __total_org_memory -= au->org_size;      
    __total_dbg_memory -= au->dbg_size;

    // use allocator to allocate memory
    dbg_size = __calc_dbg_size(_size);
    dbg_ptr = dlrealloc ( au->dbg_addr, dbg_size );
    org_ptr = (void *)( (int8 *)dbg_ptr + __prefix_size );

    // write information
    au->org_size          = _size;
    au->dbg_size          = dbg_size;
    au->dbg_addr          = dbg_ptr;
    au->org_addr          = org_ptr;
    au->tag_name          = _tag;
    au->file_name         = _file_name;
    au->func_name         = _func_name;
    au->line_nr           = _line_nr;
    au->break_onfree      = false;
    au->break_onrealloc   = false;
    au->alloc_nr          = au->alloc_nr; // alloc_nr will keep the same as malloc.
    au->thread_id         = ex_current_threadID() ;

    //
    // |________________| |________________________| |_________________|
    //  PRE_PATTERN_SIZE        _size_to_alloc        POST_PATTERN_SIZE
    //

    // add protector
    pre  = (uint32 *)dbg_ptr;
    post = (uint32 *)( (int8 *)org_ptr + _size );

    // add pre&post pattern
    for( i_pre = 0; i_pre < PREFIX_COUNT; ++i_pre, ++pre )
        *pre = PREFIX_PATTERN;
    for( i_post = 0; i_post < SUFFIX_COUNT; ++i_post, ++post )
        *post = SUFFIX_PATTERN;

    // Record memory information
    __total_org_memory       += au->org_size;      
    __total_dbg_memory       += au->dbg_size;
    __accumulate_org_memory  += au->org_size;
    __accumulate_dbg_memory  += au->dbg_size;

    // finally push the alloc info into hash_list 
    if ( __rearrange_au ( _ptr, au ) == -1 ) {
        ex_error ( "failed to re-arrange alloc unit" );
        ex_mutex_unlock(__access_mutex);
        return NULL;
    }

    ex_mutex_unlock(__access_mutex);
    return org_ptr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_free_mng( void *_ptr, const char *_file_name, const char *_func_name, size_t _line_nr )
{
    alloc_unit_t *au;

    ex_mutex_lock ( __access_mutex );

    // nothing to do with NULL ptr
    if ( _ptr == NULL ) {
        ex_mutex_unlock(__access_mutex);
        return;
    }

    // find SAllocInfo *by address
    au = __get_au (_ptr);

    // verify memory conflic first if needed
#if VERIFY_FREE
    __verify_pattern( au, _file_name, _func_name, _line_nr );
#endif

    // check allocator
    dlfree( au->dbg_addr );

    // remove original memory information
    __au_count               -= 1;
    __total_org_memory       -= au->org_size;      
    __total_dbg_memory       -= au->dbg_size;
    __accumulate_org_memory  -= au->org_size;
    __accumulate_dbg_memory  -= au->dbg_size;

    // reclaim the alloc info for next alloc
    __reclaim_au ( au );

    ex_mutex_unlock(__access_mutex);
}

///////////////////////////////////////////////////////////////////////////////
// debugs
///////////////////////////////////////////////////////////////////////////////

size_t ex_total_org_memory () { return __total_org_memory; }
size_t ex_total_dbg_memory () { return __total_dbg_memory; }
size_t ex_accumulate_org_memory () { return __accumulate_org_memory; }
size_t ex_accumulate_dbg_memory () { return __accumulate_dbg_memory; }

size_t ex_total_dl_memory () {
    // struct mallinfo {
    //     MALLINFO_FIELD_TYPE arena;    /* non-mmapped space allocated from system */
    //     MALLINFO_FIELD_TYPE ordblks;  /* number of free chunks */
    //     MALLINFO_FIELD_TYPE smblks;   /* always 0 */
    //     MALLINFO_FIELD_TYPE hblks;    /* always 0 */
    //     MALLINFO_FIELD_TYPE hblkhd;   /* space in mmapped regions */
    //     MALLINFO_FIELD_TYPE usmblks;  /* maximum total allocated space */
    //     MALLINFO_FIELD_TYPE fsmblks;  /* always 0 */
    //     MALLINFO_FIELD_TYPE uordblks; /* total allocated space */
    //     MALLINFO_FIELD_TYPE fordblks; /* total free space */
    //     MALLINFO_FIELD_TYPE keepcost; /* releasable (via malloc_trim) space */
    // } info;

    // info = dlmallinfo();
    // return info.uordblks;
    return dlmalloc_footprint();
}
