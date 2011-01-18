// ======================================================================================
// File         : stream_json.c
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 09:28:55 AM | Monday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// static defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct __json_node_t {
    strid_t _typeid;
    strid_t _name; 
    void* _val;
	struct __json_node_t* _parent;
	struct ex_array_t* _children;
} __json_node_t;    

//
static __json_node_t* __create_node () {
    __json_node_t* node = (__json_node_t*)ex_malloc( sizeof(__json_node_t) );

    node->_typeid = EX_STRID_INVALID;
    node->_name = EX_STRID_INVALID;
    node->_val = NULL;
    node->_parent = NULL;
    node->_children = NULL;

    return node;
}

//
static void __destroy_node ( __json_node_t* _node ) {
    if ( _node->_children ) {
        ex_array_each ( _node->_children, __json_node_t*, _child ) {
            __destroy_node (_child);
        } ex_array_each_end 
        ex_array_free ( _node->_children );
    }

    if ( _node->_val )
        ex_free ( _node->_val );
    ex_free(_node);
}

//
static void __add_child ( __json_node_t* _parent, __json_node_t* _child ) {
    if ( _parent->_children == NULL ) {
        _parent->_children = ex_array_alloc( sizeof(__json_node_t*), 8 );
    }
    ex_array_append( _parent->_children, &_child );
    _child->_parent = _parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define NODE_STATUS_GET_VALUE 0
#define NODE_STATUS_GET_ARRAY 1

typedef struct {
    __json_node_t* root;
    __json_node_t* parent;
    __json_node_t* current;
    __json_node_t* next;
    int get_method;
    int tmp_idx;
} __context;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yajl_null( void* _ctx ) {
    ex_warning( "null data" );
    return 1;
}

static int __yajl_boolean ( void* _ctx, int _bool ) {
    __context* ctx = (__context*)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->_val = ex_malloc ( sizeof(int) ); 
        memcpy ( ctx->current->_val, &_bool, sizeof(int) );
    }
    return 1;
}

static int __yajl_integer ( void* _ctx, long _integer ) {
    __context* ctx = (__context*)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->_val = ex_malloc ( sizeof(long) ); 
        memcpy ( ctx->current->_val, &_integer, sizeof(long) );
    }
    else if ( ctx->get_method == NODE_STATUS_GET_ARRAY ) {
        ((long*)ctx->current->_val)[ctx->tmp_idx] = _integer;
        ctx->tmp_idx++;
    }
    return 1;
}

static int __yajl_double ( void* _ctx, double _double ) {
    __context* ctx = (__context*)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->_val = ex_malloc ( sizeof(double) ); 
        memcpy ( ctx->current->_val, &_double, sizeof(double) );
    }
    else if ( ctx->get_method == NODE_STATUS_GET_ARRAY ) {
        ((double*)ctx->current->_val)[ctx->tmp_idx] = _double;
        ctx->tmp_idx++;
    }
    return 1;
}

static int __yajl_string ( void* _ctx, const unsigned char* _text, unsigned int _len ) {
    __context* ctx = (__context*)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        char* buf = ex_malloc ( sizeof(char) * (_len+1) ); 
        strncpy ( buf, (const char*)_text, sizeof(_len) );
        buf[_len] = 0;
        ctx->current->_val = buf; 
    }
    return 1;
}

static int __yajl_map_key ( void* _ctx, const unsigned char* _text, unsigned int _len ) {
    __context* ctx = (__context*)_ctx;
    char buf[128];
    char* my_buf = NULL;

    my_buf = buf;
    if ( _len >= 128 ) {
        my_buf = ex_malloc(sizeof(char)*_len); 
    } 
    strncpy( my_buf, (const char*)_text, _len );
    my_buf[_len] = 0; 

    // the childrent of array or map doesn't have _name
    if ( ctx->parent->_typeid == EX_TYPEID(array) ||
         ctx->parent->_typeid == EX_TYPEID(map) )
    {
        ex_assert( ctx->next == NULL, "next is not NULL, something is wrong!" );

        ctx->next = __create_node();
        ctx->next->_typeid = ex_strid(my_buf); 
        __add_child( ctx->parent, ctx->next );
        ctx->current = ctx->next;
        ctx->next = NULL;

        if ( ctx->current->_typeid == EX_TYPEID(array) ||
             ctx->current->_typeid == EX_TYPEID(map) ||
             ex_is_builtin_type( ctx->current->_typeid ) == false ) 
        {
            ctx->parent = ctx->current; 
        }
    }
    else
    {
        if ( ctx->next == NULL ) {
            ctx->next = __create_node();
            ctx->next->_name = ex_strid(my_buf);
        }
        else if ( ctx->next->_typeid == EX_STRID_INVALID ) {
            ctx->next->_typeid = ex_strid(my_buf); 
            __add_child( ctx->parent, ctx->next );
            ctx->current = ctx->next;
            ctx->next = NULL;

            if ( ctx->current->_typeid == EX_TYPEID(array) ||
                 ctx->current->_typeid == EX_TYPEID(map) ||
                 ex_is_builtin_type( ctx->current->_typeid ) == false ) 
            {
                ctx->parent = ctx->current; 
            }
        }
    }

    // free buf
    if ( _len >= 128 )
        ex_free(my_buf);

    return 1;
}

static int __yajl_start_map ( void* _ctx ) {
    __context* ctx = (__context*)_ctx;

    // if this is the root node
    if ( ctx->root == NULL ) {
        ctx->current = __create_node();
        ctx->root = ctx->parent = ctx->current;
        ctx->root->_typeid = ex_strid("__root__"); // this will make him not a builtin-type.
        return 1;
    }

	return 1;
}


static int __yajl_end_map ( void* _ctx ) {
    __context* ctx = (__context*)_ctx;

    // nice tricks to detect if pop-up to parent.
    if ( ctx->current == ctx->parent )
        ctx->parent = ctx->current->_parent;
    else
        ctx->current = ctx->parent;

    return 1;
}

static int __yajl_start_array ( void* _ctx ) {
    __context* ctx = (__context*)_ctx;

    ctx->tmp_idx = 0;
    if ( ctx->current->_typeid == EX_TYPEID(vec2f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*2 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(vec3f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(vec4f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(mat22f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(mat33f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*9 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(mat44f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*16 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(quatf) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(color3f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(color4f) ) {
        ctx->current->_val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(color3u) ) {
        ctx->current->_val = ex_malloc( sizeof(long)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->_typeid == EX_TYPEID(color4u) ) {
        ctx->current->_val = ex_malloc( sizeof(long)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }

    return 1;
}

static int __yajl_end_array ( void* _ctx ) {
    __context* ctx = (__context*)_ctx;
    ctx->get_method = NODE_STATUS_GET_VALUE;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: __callbacks
// ------------------------------------------------------------------ 

static yajl_callbacks __callbacks = {
    __yajl_null,
    __yajl_boolean,
    __yajl_integer,
    __yajl_double,
    NULL, // number
    __yajl_string,
    __yajl_start_map,
    __yajl_map_key,
    __yajl_end_map,
    __yajl_start_array,
    __yajl_end_array
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct {
    unsigned int numFrees;
    unsigned int numMallocs;    
} __test_mem_context;
#define TEST_CTX(vptr) ((__test_mem_context *) (vptr))

static void __yajl_free( void* _ctx, void* _ptr ) {
    ex_assert(_ptr != NULL, "_ptr can't be NULL");
    TEST_CTX(_ctx)->numFrees++;
    ex_free(_ptr);
}

static void* __yajl_malloc ( void* _ctx, unsigned int _sz ) {
    ex_assert( _sz != 0, "size can't not be zero" );
    TEST_CTX(_ctx)->numMallocs++;
    return ex_malloc(_sz);
}

static void* __yajl_realloc( void* _ctx, void* _ptr, unsigned int _sz ) {
    if ( _ptr == NULL ) {
        ex_assert( _sz != 0, "size can't not be zero" );
        TEST_CTX(_ctx)->numMallocs++;        
    } else if ( _sz == 0 ) {
        TEST_CTX(_ctx)->numFrees++;                
    }

    return ex_realloc( _ptr, _sz );
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// successful: 0
// type-invalid: 1
// name-invalid: -1
// ------------------------------------------------------------------ 

static int __check_node ( ex_stream_t* _stream, const char* _name, strid_t _typeID ) {
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int8 ( ex_stream_t* _stream, const char* _name, int8* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int16 ( ex_stream_t* _stream, const char* _name, int16* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int32 ( ex_stream_t* _stream, const char* _name, int32* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int64 ( ex_stream_t* _stream, const char* _name, int64* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint8 ( ex_stream_t* _stream, const char* _name, uint8* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint16 ( ex_stream_t* _stream, const char* _name, uint16* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint32 ( ex_stream_t* _stream, const char* _name, uint32* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint64 ( ex_stream_t* _stream, const char* _name, uint64* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_float ( ex_stream_t* _stream, const char* _name, float* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_double ( ex_stream_t* _stream, const char* _name, double* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_boolean ( ex_stream_t* _stream, const char* _name, bool* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_string ( ex_stream_t* _stream, const char* _name, const char** _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_strid ( ex_stream_t* _stream, const char* _name, strid_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_array ( ex_stream_t* _stream, const char* _name, ex_array_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_map ( ex_stream_t* _stream, const char* _name, ex_hashmap_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec2f ( ex_stream_t* _stream, const char* _name, ex_vec2f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec3f ( ex_stream_t* _stream, const char* _name, ex_vec3f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec4f ( ex_stream_t* _stream, const char* _name, ex_vec4f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat22f ( ex_stream_t* _stream, const char* _name, ex_mat22f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat33f ( ex_stream_t* _stream, const char* _name, ex_mat33f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat44f ( ex_stream_t* _stream, const char* _name, ex_mat44f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_quatf ( ex_stream_t* _stream, const char* _name, ex_quatf_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_angf ( ex_stream_t* _stream, const char* _name, ex_angf_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color3u ( ex_stream_t* _stream, const char* _name, ex_color3u_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color3f ( ex_stream_t* _stream, const char* _name, ex_color3f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color4u ( ex_stream_t* _stream, const char* _name, ex_color4u_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color4f ( ex_stream_t* _stream, const char* _name, ex_color4f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int8 ( ex_stream_t* _stream, const char* _name, int8* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int16 ( ex_stream_t* _stream, const char* _name, int16* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int32 ( ex_stream_t* _stream, const char* _name, int32* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int64 ( ex_stream_t* _stream, const char* _name, int64* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint8 ( ex_stream_t* _stream, const char* _name, uint8* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint16 ( ex_stream_t* _stream, const char* _name, uint16* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint32 ( ex_stream_t* _stream, const char* _name, uint32* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint64 ( ex_stream_t* _stream, const char* _name, uint64* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_float ( ex_stream_t* _stream, const char* _name, float* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_double ( ex_stream_t* _stream, const char* _name, double* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_boolean ( ex_stream_t* _stream, const char* _name, bool* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_string ( ex_stream_t* _stream, const char* _name, const char** _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_strid ( ex_stream_t* _stream, const char* _name, strid_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_array ( ex_stream_t* _stream, const char* _name, ex_array_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_map ( ex_stream_t* _stream, const char* _name, ex_hashmap_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec2f ( ex_stream_t* _stream, const char* _name, ex_vec2f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec3f ( ex_stream_t* _stream, const char* _name, ex_vec3f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec4f ( ex_stream_t* _stream, const char* _name, ex_vec4f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat22f ( ex_stream_t* _stream, const char* _name, ex_mat22f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat33f ( ex_stream_t* _stream, const char* _name, ex_mat33f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat44f ( ex_stream_t* _stream, const char* _name, ex_mat44f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_quatf ( ex_stream_t* _stream, const char* _name, ex_quatf_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_angf ( ex_stream_t* _stream, const char* _name, ex_angf_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color3u ( ex_stream_t* _stream, const char* _name, ex_color3u_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color3f ( ex_stream_t* _stream, const char* _name, ex_color3f_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color4u ( ex_stream_t* _stream, const char* _name, ex_color4u_t* _val ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color4f ( ex_stream_t* _stream, const char* _name, ex_color4f_t* _val ) {
}

///////////////////////////////////////////////////////////////////////////////
// func
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_stream_t* ex_create_json_read_stream ( const char* _fileName ) {
    static const ex_stream_json_t json_stream = {
        // properties
        EX_STREAM_READ,

        //
        __check_node,

        // methods
        __read_int8,
        __read_int16,
        __read_int32,
        __read_int64,
        __read_uint8,
        __read_uint16,
        __read_uint32,
        __read_uint64,
        __read_float,
        __read_double,
        __read_boolean,
        __read_string,
        __read_strid,
        __read_array,
        __read_map,
        __read_vec2f,
        __read_vec3f,
        __read_vec4f,
        __read_mat22f,
        __read_mat33f,
        __read_mat44f,
        __read_quatf,
        __read_angf,
        __read_color3u,
        __read_color3f,
        __read_color4u,
        __read_color4f,

        // nodes
        NULL,
        NULL,
    }; // end of stream read

    // load node
    yajl_handle hand;
    ex_file_t* file_hand;
    static unsigned char * fileData = NULL;
    unsigned int bufSize = 2048;
    yajl_status stat;
    size_t rd;
    yajl_parser_config cfg = { 0, 1 };
    int done;

    /* memory allocation debugging: allocate a structure which collects statistics */
    __test_mem_context memCtx = { 0,0 };

    /* memory allocation debugging: allocate a structure which holds allocation routines */
    yajl_alloc_funcs allocFuncs = {
        __yajl_malloc,
        __yajl_realloc,
        __yajl_free,
        (void *) NULL
    };

    //
    __context context = {
        (void*)NULL,
        (void*)NULL,
        (void*)NULL,
        (void*)NULL,
        NODE_STATUS_GET_VALUE,
        0
    };
    allocFuncs.ctx = (void *) &memCtx;

    //
    file_hand = ex_open_file(_fileName,"r");
    fileData = (unsigned char *) ex_malloc(bufSize);
    hand = yajl_alloc(&__callbacks, &cfg, &allocFuncs, &context);

    //
    done = 0;
	while (!done) {
        rd = ex_file_read( file_hand, fileData, 1, bufSize );
        if (rd == -1) // read error.
            break;

        // finish reading
        if (rd == 0)
            done = 1;

        if ( done )
            stat = yajl_parse_complete(hand); // parse any remaining buffered data
        else
            stat = yajl_parse(hand, fileData, rd); // read file data, pass to parser
        
        if (stat != yajl_status_insufficient_data &&
            stat != yajl_status_ok)
        {
            unsigned char * str = yajl_get_error(hand, 0, fileData, rd);
            ex_error ( "failed to parse json file: %s. error: %s", _fileName, (char *) str );
            yajl_free_error(hand, str);
            break;
        }
    } 
    yajl_free(hand);
    ex_free(fileData);
    ex_close_file(file_hand);

    // copy the read methods
    ex_stream_json_t* r_stream = (ex_stream_json_t*)ex_malloc( sizeof(ex_stream_json_t) );
    memcpy ( r_stream, &json_stream, sizeof(ex_stream_json_t) );
    r_stream->_root = context.root;
    r_stream->_cur = context.root;

    return (ex_stream_t*)r_stream;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_stream_t* ex_create_json_write_stream () {
    __json_node_t* node;
    static const ex_stream_json_t json_stream = {
        // properties
        EX_STREAM_WRITE,

        //
        __check_node,

        // methods
        __write_int8,
        __write_int16,
        __write_int32,
        __write_int64,
        __write_uint8,
        __write_uint16,
        __write_uint32,
        __write_uint64,
        __write_float,
        __write_double,
        __write_boolean,
        __write_string,
        __write_strid,
        __write_array,
        __write_map,
        __write_vec2f,
        __write_vec3f,
        __write_vec4f,
        __write_mat22f,
        __write_mat33f,
        __write_mat44f,
        __write_quatf,
        __write_angf,
        __write_color3u,
        __write_color3f,
        __write_color4u,
        __write_color4f,

        // nodes
        NULL,
        NULL,
    }; // end of stream read

    // TODO: create node

    // copy the read methods
    ex_stream_json_t* w_stream = (ex_stream_json_t*)ex_malloc( sizeof(ex_stream_json_t) );
    memcpy ( w_stream, &json_stream, sizeof(ex_stream_json_t) );
    w_stream->_root = node;
    w_stream->_cur = node;

    return (ex_stream_t*)w_stream;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_json_stream ( ex_stream_json_t* _stream ) {
    __destroy_node(_stream->_root);
    ex_free(_stream);
}
