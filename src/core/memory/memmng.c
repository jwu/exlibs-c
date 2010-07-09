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
#include "../../container/hashmap.h"
#include "../../container/list.h"

///////////////////////////////////////////////////////////////////////////////
// config
///////////////////////////////////////////////////////////////////////////////

#define MAX_HASH            4096
#define MAX_NEW_ALLOCINFO   256
#define MEM_LOG             "./mem_dump.err"

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

typedef struct alloc_unit_
{
    size_t              org_size;
    size_t              dbg_size;
    void*               org_addr;
    void*               dbg_addr;
    const char*         tag_name;
    const char*         file_name;
    const char*         func_name;
    size_t              line_nr;
    bool                break_onfree;
    bool                break_onrealloc;
    size_t              alloc_nr;
    uint32              thread_id;
    struct alloc_unit_* next;
    struct alloc_unit_* prev;
} alloc_unit_t; // end struct alloc_unit_t 

///////////////////////////////////////////////////////////////////////////////
// variables
///////////////////////////////////////////////////////////////////////////////

static const size_t prefix_size = sizeof(uint32) * PREFIX_COUNT;
static const size_t suffix_size = sizeof(uint32) * SUFFIX_COUNT;
static const size_t pattern_size = sizeof(uint32) * PREFIX_COUNT + sizeof(uint32) * SUFFIX_COUNT;

alloc_unit_t* reserved_au_list;
hashmap_t* au_map;
list_t* au_bucket;

size_t au_count;
size_t total_org_memory;
size_t total_dbg_memory;
size_t accumulate_org_memory;
size_t accumulate_dbg_memory;

///////////////////////////////////////////////////////////////////////////////
// private
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t _calc_dbg_size ( size_t _size )
{
    return _size + pattern_size;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

alloc_unit_t* _request_au ()
{
    alloc_unit_t* tmp_au;

    if ( !reserved_au_list ) {
        uint i = 0;

        // Allocate MAX_NEW_ALLOCINFO reserve alloc-info
        reserved_au_list = (alloc_unit_t*)ex_malloc_nomng( MAX_NEW_ALLOCINFO * sizeof(alloc_unit_t) );

        // Assert exAllocInfo != NULL
        ex_assert_return( reserved_au_list != NULL, NULL, "out of memory!" );

        // Build a linked-list of the elements in our reserve-allocinfo-list
        // memzero setting
        memzero( reserved_au_list, MAX_NEW_ALLOCINFO * sizeof(alloc_unit_t) );
        for( i = 0; i < MAX_NEW_ALLOCINFO-1; ++i )
        {
            reserved_au_list[i].next = reserved_au_list + (i+1);
            reserved_au_list[i+1].prev = reserved_au_list + i; 
        }

        // Add this address to our reserved_au_list so we can free it later
        list_push_back_nomng ( au_bucket, reserved_au_list );
    }

    // this is the standard use of go next and unlink
    tmp_au = reserved_au_list;
    reserved_au_list = reserved_au_list->next;

    // unlink the getted alloc info
    tmp_au->next = NULL;
    tmp_au->prev = NULL;

    //
    return tmp_au;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _verify_pattern ( alloc_unit_t* _au, const char* _file_name, const char* _func_name, size_t _line_nr )
{
    // verify pre-pattern
#if VERIFY_PREFIX
    {
        uint32* pre  = (uint32*)_au->dbg_addr;
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
                            _file_name, _line_nr, _func_name, _au->thread_id, current_threadID() );
                break;
            }
        }
    }
#endif

    // verify post-pattern
#if VERIFY_SUFFIX
    {
        uint32* post = (uint32*)( (int8*)_au->dbg_addr + prefix_size + _au->org_size );
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
                            _file_name, _line_nr, _func_name, _au->thread_id, current_threadID() );
                break;
            }
        }
    }
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _reclaim_au ( alloc_unit_t* _au ) {
    if ( hashmap_erase ( au_map, &_au->org_addr ) == NULL )
        ex_error ( "failed to reclaim alloc unit" );

    // append to reserve alloc info
    _au->next = reserved_au_list;
    _au->prev = NULL;

    // reserve alloc info could be use out
    if ( reserved_au_list )
        reserved_au_list->prev = _au;

    reserved_au_list = _au;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
inline int _push_au ( alloc_unit_t* _au ) { 
    size_t idx = -1;
    hashmap_insert_nomng ( au_map, &_au->org_addr, &_au, &idx ); 
    return idx;
}

//
inline alloc_unit_t* _get_au ( void* _ptr ) { 
    return *((alloc_unit_t**)hashmap_get ( au_map, &_ptr )); 
}

//
inline int _rearrange_au ( void* _ptr, alloc_unit_t* au ) {
    if ( hashmap_erase ( au_map, &_ptr ) == NULL )
        return -1;
    return _push_au ( au );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _log ( const char* _file_name, const char* _format, ... )
{
    char buffer[2048];
    FILE* fp;

    // The Buffer
    EX_GET_VA_STRING( buffer, 2048, _format );

    // open the log file
    fp = fopen( _file_name, "ab" );

    // The file must be opened
    ex_assert( fp, "Can't not open the log file:%s", _file_name );

    // print the log into the file
    fprintf( fp, "%s", buffer );

    // close the file
    fclose(fp);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _dump () {
    //
    ex_assert ( hashmap_len(au_map) == 0, "There are %d place(s) exsits memory leak.",  hashmap_len(au_map) );
    
    //
    if ( hashmap_len(au_map) ) {
        hashmap_each ( alloc_unit_t*, au, au_map ) {
            char text[2048];
            memzero ( text, 2048 );
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
                       ,au->file_name ,au->line_nr 
                       ,au->func_name 
                       ,au->tag_name 
                       ,au->org_addr 
                       ,au->dbg_addr 
                       ,au->org_size, au->org_size 
                       ,au->dbg_size, au->dbg_size 
                       ,au->alloc_nr, au->alloc_nr 
                       ,au->thread_id, au->thread_id 
                     );

            // format log info
            _log ( MEM_LOG, "%s", text );
        } hashmap_each_end;
    }
}

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void mem_init ()
{
    // TODO: mutex

    au_map = hashmap_alloc_nomng ( sizeof(void*), sizeof(alloc_unit_t*), 256, hashkey_ptr, keycmp_ptr );
    au_bucket = list_alloc_nomng ( sizeof(alloc_unit_t*) );

    // remove log file if it exists
    unlink ( MEM_LOG );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void mem_deinit ()
{
    // TODO: mutex
    _dump ();

    //
    hashmap_free_nomng ( au_map );

    // free the reserve alloc info buffer
    list_free_nomng ( au_bucket );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* _mng_malloc( size_t _size, const char* _tag, const char* _file_name, const char* _func_name, size_t _line_nr )
{
    size_t dbg_size;
    void *dbg_ptr, *org_ptr;
    uint32 *pre, *post;
    uint i_pre, i_post;
    alloc_unit_t* au;

    // TODO: mutex

    // ANSI says: allocation requests of 0 bytes will still return a valid value
    if ( _size == 0) _size = 1;

    // use allocator to allocate memory
    dbg_size = _calc_dbg_size(_size);
    dbg_ptr = dlmemalign( EX_MEM_ALIGN, dbg_size );
    org_ptr = (void*)( (int8*)dbg_ptr + prefix_size );

    // get au, if not, create one
    au = _request_au();
    ex_assert_return( au, NULL, "AllocInfo not found, check if the _ptr have been free or invalid" );
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
    au->alloc_nr         = au_count;
    au->thread_id        = current_threadID () ;

    //
    // |________________| |________________________| |_________________|
    //  PRE_PATTERN_SIZE        _size_to_alloc        POST_PATTERN_SIZE
    //

    // add protector
    pre  = (uint32*)dbg_ptr;
    post = (uint32*)( (int8*)org_ptr + _size );

    // add pre&post pattern
    for( i_pre = 0; i_pre < PREFIX_COUNT; ++i_pre, ++pre )
        *pre = PREFIX_PATTERN;
    for( i_post = 0; i_post < SUFFIX_COUNT; ++i_post, ++post )
        *post = SUFFIX_PATTERN;

    // Record memory information
    au_count += 1;
    total_org_memory       += au->org_size;      
    total_dbg_memory       += au->dbg_size;
    accumulate_org_memory  += au->org_size;
    accumulate_dbg_memory  += au->dbg_size;

    // finally push the alloc info into hash_list 
    if ( _push_au (au) == -1 ) {
        ex_error ( "failed to insert alloc unit" );
        return NULL;
    }

    return org_ptr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* _mng_realloc( void* _ptr, size_t _size, const char* _tag, const char* _file_name, const char* _func_name, size_t _line_nr )
{
    alloc_unit_t* au;
    size_t dbg_size;
    void *dbg_ptr, *org_ptr;
    uint32 *pre, *post;
    uint i_pre, i_post;

    // TODO: mutex

    // alloc NULL memory address
    if ( _ptr == NULL ) {
        return _mng_malloc( _size, _tag, _file_name, _func_name, _line_nr );
    }

    // realloc zero bytes free 
    if ( _size == 0 ) {
        _mng_free( _ptr, _file_name, _func_name, _line_nr );
        return NULL;
    }

    // find au* by address
    au =  _get_au(_ptr);
    ex_assert_return ( au, NULL, "AllocInfo not found, check if the _ptr have been free or invalid" );

    // verify memory conflic first if needed
#if VERIFY_REALLOC
    _verify_pattern ( au, _file_name, _func_name, _line_nr );
#endif

    // remove original memory information
    total_org_memory -= au->org_size;      
    total_dbg_memory -= au->dbg_size;

    // use allocator to allocate memory
    dbg_size = _calc_dbg_size(_size);
    dbg_ptr = dlrealloc ( au->dbg_addr, dbg_size );
    org_ptr = (void*)( (int8*)dbg_ptr + prefix_size );

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
    au->thread_id         = current_threadID() ;

    //
    // |________________| |________________________| |_________________|
    //  PRE_PATTERN_SIZE        _size_to_alloc        POST_PATTERN_SIZE
    //

    // add protector
    pre  = (uint32*)dbg_ptr;
    post = (uint32*)( (int8*)org_ptr + _size );

    // add pre&post pattern
    for( i_pre = 0; i_pre < PREFIX_COUNT; ++i_pre, ++pre )
        *pre = PREFIX_PATTERN;
    for( i_post = 0; i_post < SUFFIX_COUNT; ++i_post, ++post )
        *post = SUFFIX_PATTERN;

    // Record memory information
    total_org_memory       += au->org_size;      
    total_dbg_memory       += au->dbg_size;
    accumulate_org_memory  += au->org_size;
    accumulate_dbg_memory  += au->dbg_size;

    // finally push the alloc info into hash_list 
    if ( _rearrange_au ( _ptr, au ) == -1 ) {
        ex_error ( "failed to re-arrange alloc unit" );
        return NULL;
    }
    return org_ptr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _mng_free( void* _ptr, const char* _file_name, const char* _func_name, size_t _line_nr )
{
    alloc_unit_t* au;

    // TODO: mutex

    // nothing to do with NULL ptr
    if ( _ptr == NULL )
        return;

    // find SAllocInfo* by address
    au = _get_au (_ptr);

    // verify memory conflic first if needed
#if VERIFY_FREE
    _verify_pattern( au, _file_name, _func_name, _line_nr );
#endif

    // check allocator
    dlfree( au->dbg_addr );

    // remove original memory information
    au_count               -= 1;
    total_org_memory       -= au->org_size;      
    total_dbg_memory       -= au->dbg_size;
    accumulate_org_memory  -= au->org_size;
    accumulate_dbg_memory  -= au->dbg_size;

    // reclaim the alloc info for next alloc
    _reclaim_au ( au );
}

