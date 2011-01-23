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
    strid_t typeid;
    strid_t name; 
    bool used;
    void *val;
	struct __json_node_t *parent;
	struct ex_list_t *children;
} __json_node_t;    

//
static __json_node_t *__create_node () {
    __json_node_t *node = (__json_node_t*)ex_malloc( sizeof(__json_node_t) );

    node->typeid = EX_STRID_NULL;
    node->name = EX_STRID_NULL;
    node->used = false;
    node->val = NULL;
    node->parent = NULL;
    node->children = NULL;

    return node;
}

//
static void __destroy_node ( __json_node_t *_node ) {
    if ( _node->children ) {
        ex_list_each ( _node->children, __json_node_t*, _child ) {
            __destroy_node (_child);
        } ex_list_each_end 
        ex_list_free ( _node->children );
    }

    if ( _node->val )
        ex_free ( _node->val );
    ex_free(_node);
}

//
static void __add_child ( __json_node_t *_parent, __json_node_t *_child ) {
    if ( _parent->children == NULL ) {
        _parent->children = ex_list_alloc( sizeof(__json_node_t*) );
    }
    ex_list_append( _parent->children, &_child );
    _child->parent = _parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define NODE_STATUS_GET_VALUE 0
#define NODE_STATUS_GET_ARRAY 1

typedef struct {
    __json_node_t *root;
    __json_node_t *parent;
    __json_node_t *current;
    __json_node_t *next;
    int get_method;
    int tmp_idx;
} __context;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yajl_null( void *_ctx ) {
    ex_warning( "null data" );
    return 1;
}

static int __yajl_boolean ( void *_ctx, int _bool ) {
    __context *ctx = (__context *)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->val = ex_malloc ( sizeof(int) ); 
        memcpy ( ctx->current->val, &_bool, sizeof(int) );
    }
    return 1;
}

// FIXME: it seems that uin64 will exceed the range.
static int __yajl_integer ( void *_ctx, long _integer ) {
    __context *ctx = (__context *)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->val = ex_malloc ( sizeof(long) ); 
        memcpy ( ctx->current->val, &_integer, sizeof(long) );
    }
    else if ( ctx->get_method == NODE_STATUS_GET_ARRAY ) {
        ((long *)ctx->current->val)[ctx->tmp_idx] = _integer;
        ctx->tmp_idx++;
    }
    return 1;
}

static int __yajl_double ( void *_ctx, double _double ) {
    __context *ctx = (__context *)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        ctx->current->val = ex_malloc ( sizeof(double) ); 
        memcpy ( ctx->current->val, &_double, sizeof(double) );
    }
    else if ( ctx->get_method == NODE_STATUS_GET_ARRAY ) {
        ((double *)ctx->current->val)[ctx->tmp_idx] = _double;
        ctx->tmp_idx++;
    }
    return 1;
}

static int __yajl_string ( void *_ctx, const unsigned char *_text, unsigned int _len ) {
    __context *ctx = (__context*)_ctx;
    if ( ctx->get_method == NODE_STATUS_GET_VALUE ) {
        char *buf = ex_malloc ( sizeof(char) * (_len+1) ); 
        strncpy ( buf, (const char *)_text, _len );
        buf[_len] = 0;
        ctx->current->val = buf; 
    }
    return 1;
}

static int __yajl_map_key ( void *_ctx, const unsigned char *_text, unsigned int _len ) {
    __context *ctx = (__context*)_ctx;
    char buf[128];
    char *my_buf = NULL;

    my_buf = buf;
    if ( _len+1 >= 128 ) {
        my_buf = ex_malloc(sizeof(char)*_len+1); 
    } 
    strncpy( my_buf, (const char *)_text, _len );
    my_buf[_len] = 0;

    // the childrent of array or map doesn't have name
    if ( ctx->parent->typeid == EX_TYPEID(array) ||
         ctx->parent->typeid == EX_TYPEID(map) )
    {
        ex_assert( ctx->next == NULL, "next is not NULL, something is wrong!" );

        ctx->next = __create_node();
        ctx->next->typeid = ex_strid(my_buf); 
        __add_child( ctx->parent, ctx->next );
        ctx->current = ctx->next;
        ctx->next = NULL;

        if ( ctx->current->typeid == EX_TYPEID(array) ||
             ctx->current->typeid == EX_TYPEID(map) ||
             ex_is_builtin_type( ctx->current->typeid ) == false ) 
        {
            ctx->parent = ctx->current; 
        }
    }
    else
    {
        if ( ctx->next == NULL ) {
            ctx->next = __create_node();
            ctx->next->name = ex_strid(my_buf);
        }
        else if ( ctx->next->typeid == EX_STRID_NULL ) {
            ctx->next->typeid = ex_strid(my_buf); 
            __add_child( ctx->parent, ctx->next );
            ctx->current = ctx->next;
            ctx->next = NULL;

            if ( ctx->current->typeid == EX_TYPEID(array) ||
                 ctx->current->typeid == EX_TYPEID(map) ||
                 ex_is_builtin_type( ctx->current->typeid ) == false ) 
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

static int __yajl_start_map ( void *_ctx ) {
    __context *ctx = (__context *)_ctx;

    // if this is the root node
    if ( ctx->root == NULL ) {
        ctx->current = __create_node();
        ctx->root = ctx->parent = ctx->current;
        ctx->root->typeid = ex_strid("__root__"); // this will make him not a builtin-type.
        return 1;
    }

	return 1;
}


static int __yajl_end_map ( void *_ctx ) {
    __context *ctx = (__context *)_ctx;

    // nice tricks to detect if pop-up to parent.
    if ( ctx->current == ctx->parent )
        ctx->parent = ctx->current->parent;
    else
        ctx->current = ctx->parent;

    return 1;
}

static int __yajl_start_array ( void *_ctx ) {
    __context *ctx = (__context *)_ctx;

    ctx->tmp_idx = 0;
    if ( ctx->current->typeid == EX_TYPEID(vec2f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*2 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(vec3f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(vec4f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(mat22f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(mat33f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*9 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(mat44f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*16 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(quatf) ) {
        ctx->current->val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(color3f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(color4f) ) {
        ctx->current->val = ex_malloc( sizeof(double)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(color3u) ) {
        ctx->current->val = ex_malloc( sizeof(long)*3 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }
    else if ( ctx->current->typeid == EX_TYPEID(color4u) ) {
        ctx->current->val = ex_malloc( sizeof(long)*4 );
        ctx->get_method = NODE_STATUS_GET_ARRAY;
    }

    return 1;
}

static int __yajl_end_array ( void *_ctx ) {
    __context *ctx = (__context *)_ctx;
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

static void __yajl_free( void *_ctx, void *_ptr ) {
    ex_assert(_ptr != NULL, "_ptr can't be NULL");
    TEST_CTX(_ctx)->numFrees++;
    ex_free(_ptr);
}

static void *__yajl_malloc ( void *_ctx, unsigned int _sz ) {
    ex_assert( _sz != 0, "size can't not be zero" );
    TEST_CTX(_ctx)->numMallocs++;
    return ex_malloc(_sz);
}

static void *__yajl_realloc( void *_ctx, void *_ptr, unsigned int _sz ) {
    if ( _ptr == NULL ) {
        ex_assert( _sz != 0, "size can't not be zero" );
        TEST_CTX(_ctx)->numMallocs++;        
    } else if ( _sz == 0 ) {
        TEST_CTX(_ctx)->numFrees++;                
    }

    return ex_realloc( _ptr, _sz );
}

///////////////////////////////////////////////////////////////////////////////
// read functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// successful: 0
// failed: -1
// ------------------------------------------------------------------ 

static int __read_next ( ex_stream_t *_stream, strid_t _name, strid_t _typeID ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    __json_node_t *parent = stream->anchor;

    ex_list_each( parent->children, __json_node_t *, jnode ) {
        // if we found the node with the same name and type
        if ( jnode->used == false && 
             jnode->name == _name && 
             jnode->typeid == _typeID ) 
        {
            stream->current = jnode;
            jnode->used = true;
            return 0;
        }
    } ex_list_each_end

    ex_warning( "can't find node %s with type %s", ex_strid_to_cstr(_name), ex_strid_to_cstr(_typeID) );
    return -1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_push ( ex_stream_t *_stream ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    stream->anchor = stream->current;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_pop ( ex_stream_t *_stream ) {
    ex_stream_json_t* stream = (ex_stream_json_t *)_stream; 
    stream->anchor = stream->anchor->parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_bool ( ex_stream_t *_stream, bool *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = *(int *)node->val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int ( ex_stream_t *_stream, int *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = *(int *)node->val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_size_t ( ex_stream_t *_stream, size_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = *(size_t *)node->val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int8 ( ex_stream_t *_stream, int8 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (int8)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int16 ( ex_stream_t *_stream, int16 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (int16)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int32 ( ex_stream_t *_stream, int32 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (int32)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_int64 ( ex_stream_t *_stream, int64 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (int64)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint8 ( ex_stream_t *_stream, uint8 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (uint8)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint16 ( ex_stream_t *_stream, uint16 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (uint16)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint32 ( ex_stream_t *_stream, uint32 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (uint32)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_uint64 ( ex_stream_t *_stream, uint64 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (uint64)(*(long *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_float ( ex_stream_t *_stream, float *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (float)(*(double *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_double ( ex_stream_t *_stream, double *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = (*(double *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_cstr ( ex_stream_t *_stream, char **_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    // TODO: think about this, can have fix string and dynamic string { 
    *_val = (char *)ex_malloc ( strlen((const char *)node->val)+1 );
    strcpy ( *_val, (const char *)node->val );
    // } TODO end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_string ( ex_stream_t *_stream, ex_string_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    ex_string_ncpy( _val, (const char *)node->val, strlen((const char *)node->val) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_strid ( ex_stream_t *_stream, strid_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    *_val = ex_strid((char *)node->val);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec2f ( ex_stream_t *_stream, ex_vec2f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_vec2f_set( _val, (float)(buf[0]), (float)(buf[1]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec3f ( ex_stream_t *_stream, ex_vec3f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_vec3f_set( _val, (float)(buf[0]), (float)(buf[1]), (float)(buf[2]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_vec4f ( ex_stream_t *_stream, ex_vec4f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_vec4f_set( _val, (float)(buf[0]), (float)(buf[1]), (float)(buf[2]), (float)(buf[3]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat22f ( ex_stream_t *_stream, ex_mat22f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_mat22f_set( _val, 
                   (float)(buf[0]), (float)(buf[1]), 
                   (float)(buf[2]), (float)(buf[3]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat33f ( ex_stream_t *_stream, ex_mat33f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_mat33f_set( _val, 
                   (float)(buf[0]), (float)(buf[1]), (float)(buf[2]),
                   (float)(buf[3]), (float)(buf[4]), (float)(buf[5]),
                   (float)(buf[6]), (float)(buf[7]), (float)(buf[8]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_mat44f ( ex_stream_t *_stream, ex_mat44f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_mat44f_set( _val, 
                   (float)(buf[0]),  (float)(buf[1]),  (float)(buf[2]),  (float)(buf[3]),
                   (float)(buf[4]),  (float)(buf[5]),  (float)(buf[6]),  (float)(buf[7]),
                   (float)(buf[8]),  (float)(buf[9]),  (float)(buf[10]), (float)(buf[11]),
                   (float)(buf[12]), (float)(buf[13]), (float)(buf[14]), (float)(buf[15]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_quatf ( ex_stream_t *_stream, ex_quatf_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_quatf_set( _val, (float)(buf[0]), (float)(buf[1]), (float)(buf[2]), (float)(buf[3]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_angf ( ex_stream_t *_stream, ex_angf_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    ex_angf_set_by_radians( _val, (float)(*((double *)node->val)) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color3u ( ex_stream_t *_stream, ex_color3u_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    long *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (long *)(node->val);
    ex_color3u_set( _val, (uint8)(buf[0]), (uint8)(buf[1]), (uint8)(buf[2]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color3f ( ex_stream_t *_stream, ex_color3f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_color3f_set( _val, (float)(buf[0]), (float)(buf[1]), (float)(buf[2]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color4u ( ex_stream_t *_stream, ex_color4u_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    long *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (long *)(node->val);
    ex_color4u_set( _val, (uint8)(buf[0]), (uint8)(buf[1]), (uint8)(buf[2]), (uint8)(buf[3]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_color4f ( ex_stream_t *_stream, ex_color4f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *buf; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    buf = (double *)(node->val);
    ex_color4f_set( _val, (float)(buf[0]), (float)(buf[1]), (float)(buf[2]), (float)(buf[3]) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_array ( ex_stream_t *_stream, ex_array_t *_val, ex_serialize_pfn _pfn_serialize_el ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    void *buf;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    // it is possible our array have zero element in it.
    if ( node->children && node->children->count > 0 ) {
        buf = ex_malloc ( _val->element_bytes );
        ex_list_each ( node->children, __json_node_t *, _child ) {
            _pfn_serialize_el ( _stream, _child->name /*must be EX_STRID_NULL*/, buf );
            ex_array_append ( _val, buf );
        } ex_list_each_end 
        ex_free(buf);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __read_map ( ex_stream_t *_stream, ex_hashmap_t *_val, ex_serialize_pfn _pfn_serialize_key, ex_serialize_pfn _pfn_serialize_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    void *key_buf, *val_buf;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    // it is possible our map have zero element in it.
    if ( node->children && node->children->count > 0 ) {
        key_buf = ex_malloc ( _val->key_bytes );
        val_buf = ex_malloc ( _val->value_bytes );
        ex_list_each ( node->children, __json_node_t *, _child ) {
            // get key
            _pfn_serialize_key ( _stream, _child->name /*must be EX_STRID_NULL*/, key_buf );

            // doing a next jump manually 
            ++__idx__;
            __node__ = __node__->next;
            ex_assert_return( __node__, /*dummy*/, "value can't not be NULL if we have key!" );
            _child = *((__json_node_t **) ( __node__->value ));

            // get value
            _pfn_serialize_val ( _stream, _child->name /*must be EX_STRID_NULL*/, val_buf );

            // now insert the item 
            ex_hashmap_insert ( _val, key_buf, val_buf, NULL );
        } ex_list_each_end 
        ex_free(key_buf);
        ex_free(val_buf);
    }
}

///////////////////////////////////////////////////////////////////////////////
// write functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// successful: 0
// failed: -1
// ------------------------------------------------------------------ 

static int __write_next ( ex_stream_t *_stream, strid_t _name, strid_t _typeID ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    __json_node_t *parent = stream->anchor;
    __json_node_t *jnode = __create_node();

    jnode->name = _name;
    jnode->typeid = _typeID;
    __add_child( parent, jnode );

    stream->current = jnode;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_push ( ex_stream_t *_stream ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    stream->anchor = stream->current;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_pop ( ex_stream_t *_stream ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    stream->anchor = stream->anchor->parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_bool ( ex_stream_t *_stream, bool *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(int) ); 
    *(int *)node->val = (int)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int ( ex_stream_t *_stream, int *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_size_t ( ex_stream_t *_stream, size_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int8 ( ex_stream_t *_stream, int8 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int16 ( ex_stream_t *_stream, int16 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int32 ( ex_stream_t *_stream, int32 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_int64 ( ex_stream_t *_stream, int64 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint8 ( ex_stream_t *_stream, uint8 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint16 ( ex_stream_t *_stream, uint16 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_uint32 ( ex_stream_t *_stream, uint32 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// FIXME: it seems that uin64 will exceed the range.
// ------------------------------------------------------------------ 

static void __write_uint64 ( ex_stream_t *_stream, uint64 *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) ); 
    *(long *)node->val = (long)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_float ( ex_stream_t *_stream, float *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) ); 
    *(double *)node->val = (double)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_double ( ex_stream_t *_stream, double *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) ); 
    *(double *)node->val = (double)*_val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_cstr ( ex_stream_t *_stream, char **_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    int len = 0;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    len = strlen(*_val);
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( len+1 ); 
    strncpy( (char *)node->val, *_val, len );
    ((char *)node->val)[len] = '\0';
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_string ( ex_stream_t *_stream, ex_string_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( _val->len+1 ); 
    strncpy( (char *)node->val, _val->text, _val->len );
    ((char *)node->val)[_val->len] = '\0';
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_strid ( ex_stream_t *_stream, strid_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    char *str;
    int len = 0;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    str = ex_strid_to_cstr(*_val);
    len = strlen(str);
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( len+1 ); 
    strncpy( (char *)node->val, str, len );
    ((char *)node->val)[len] = '\0';
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec2f ( ex_stream_t *_stream, ex_vec2f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 2 ); 
    *p = (double)_val->x; ++p;
    *p = (double)_val->y;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec3f ( ex_stream_t *_stream, ex_vec3f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 3 ); 
    *p = (double)_val->x; ++p;
    *p = (double)_val->y; ++p;
    *p = (double)_val->z;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_vec4f ( ex_stream_t *_stream, ex_vec4f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 4 ); 
    *p = (double)_val->x; ++p;
    *p = (double)_val->y; ++p;
    *p = (double)_val->z; ++p;
    *p = (double)_val->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat22f ( ex_stream_t *_stream, ex_mat22f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;
    float *m;
    int i = 0;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    m = _val->m;
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 4 ); 
    while ( i < 2 ) {
        *p = (double)*m; ++p; ++m; ++i;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat33f ( ex_stream_t *_stream, ex_mat33f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;
    float *m;
    int i = 0;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    m = _val->m;
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 9 ); 
    while ( i < 9 ) {
        *p = (double)*m; ++p; ++m; ++i;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_mat44f ( ex_stream_t *_stream, ex_mat44f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;
    float *m;
    int i = 0;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    m = _val->m;
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 16 ); 
    while ( i < 16 ) {
        *p = (double)*m; ++p; ++m; ++i;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_quatf ( ex_stream_t *_stream, ex_quatf_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 4 ); 
    *p = (double)_val->x; ++p;
    *p = (double)_val->y; ++p;
    *p = (double)_val->z; ++p;
    *p = (double)_val->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_angf ( ex_stream_t *_stream, ex_angf_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) ); 
    *(double *)node->val = (double)(_val->rad);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color3u ( ex_stream_t *_stream, ex_color3u_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    long *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (long *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) * 3 ); 
    *p = (long)_val->r; ++p;
    *p = (long)_val->g; ++p;
    *p = (long)_val->b;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color3f ( ex_stream_t *_stream, ex_color3f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 3 ); 
    *p = (double)_val->r; ++p;
    *p = (double)_val->g; ++p;
    *p = (double)_val->b;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color4u ( ex_stream_t *_stream, ex_color4u_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    long *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (long *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(long) * 4 ); 
    *p = (long)_val->r; ++p;
    *p = (long)_val->g; ++p;
    *p = (long)_val->b; ++p;
    *p = (long)_val->a;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_color4f ( ex_stream_t *_stream, ex_color4f_t *_val ) {
    ex_stream_json_t *stream;
    __json_node_t *node; 
    double *p;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    p = (double *)node->val; 
    stream = (ex_stream_json_t *)_stream; 
    node = stream->current;

    node->val = ex_malloc ( sizeof(double) * 4 ); 
    *p = (double)_val->r; ++p;
    *p = (double)_val->g; ++p;
    *p = (double)_val->b; ++p;
    *p = (double)_val->a;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_array ( ex_stream_t *_stream, ex_array_t *_val, ex_serialize_pfn _pfn_serialize_el ) {
    ex_stream_json_t *stream;
    size_t idx = 0;
    __json_node_t *parent, *jnode;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    stream = (ex_stream_json_t *)_stream; 
    parent = stream->anchor;

    while ( idx < _val->count ) {
        void *el = ex_array_get(_val,idx);

        // add element node
        jnode = __create_node();
        stream->current = jnode;
        _pfn_serialize_el ( _stream, EX_STRID_NULL, el );
        __add_child( parent, jnode );

        ++idx;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __write_map ( ex_stream_t *_stream, ex_hashmap_t *_val, ex_serialize_pfn _pfn_serialize_key, ex_serialize_pfn _pfn_serialize_val ) {
    ex_stream_json_t *stream;
    size_t idx = 0, count = 0;
    __json_node_t *parent, *jnode;

    ex_assert_return ( _val, /*dummy*/, "the input value can't not be NULL" );
    count = ex_hashmap_count(_val);
    stream = (ex_stream_json_t *)_stream; 
    parent = stream->anchor;

    while ( idx < count ) {
        void *key = (char *)_val->keys + idx * _val->key_bytes;
        void *value = (char *)_val->values + idx * _val->value_bytes;

        // add key node
        jnode = __create_node();
        stream->current = jnode;
        _pfn_serialize_key ( _stream, EX_STRID_NULL, key );
        __add_child( parent, jnode );

        // add value node
        jnode = __create_node();
        stream->current = jnode;
        _pfn_serialize_val ( _stream, EX_STRID_NULL, value );
        __add_child( parent, jnode );

        ++idx;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __save_nodes ( ex_file_t *_file, __json_node_t *_node, int _level ) {
    ex_assert_return ( _node, /*dummy*/, "node can't be NULL" );
    char buf[2048];

    // TODO: should use buffer that write text
    // TODO: for safety, we need to do while realloc protection. { 
    snprintf ( buf, 2048, "%*s%s: { %s: ",
               _level, "\t", 
               ex_strid_to_cstr(_node->name), 
               ex_strid_to_cstr(_node->typeid) );
    // } TODO end 
    ex_fwrite ( _file, buf, 1, strlen(buf) );

    // write value
    // TODO: for safety, we need to do while realloc protection. { 
    if ( _node->typeid == EX_TYPEID(bool) ) {
        snprintf ( buf, 2048, "%s }", *((bool *)_node->val) == true ? "true" : "false" );
    }
    else if ( ex_is_integer(_node->typeid) ) {
        snprintf ( buf, 2048, "%ld }", *((long *)_node->val) );
    }
    else if ( ex_is_fp(_node->typeid) ) {
        snprintf ( buf, 2048, "%f }", *((double *)_node->val) );
    }
    else if ( ex_is_string(_node->typeid) ) {
        snprintf ( buf, 2048, "%s }", (char *)_node->val );
    }
    else if ( _node->typeid >= EX_TYPEID(vec2f) && _node->typeid < EX_TYPEID(mat44f) ) {
        snprintf ( buf, 2048, "%s }", (char *)_node->val );
    }

    ex_fwrite ( _file, buf, 1, strlen(buf) );
    // } TODO end 

    // save child nodes
    if ( _node->children ) {
        ex_fwrite ( _file, ",\n", 1, 2 );
        ex_list_each ( _node->children, __json_node_t *, _child ) {
            __save_nodes ( _file, _child, ++_level );
        } ex_list_each_end 
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __save_to_file ( ex_stream_t *_stream, const char *_filename ) {
    ex_stream_json_t *stream = (ex_stream_json_t *)_stream; 
    ex_file_t *file;
    
    file = ex_fopen ( _filename, "w" );
    if ( file == NULL )
        return -1;

    __save_nodes ( file, stream->root, 0 );
    ex_fclose(file);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// func
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_stream_t *ex_create_json_read_stream ( const char *_fileName ) {
    static const ex_stream_json_t json_stream = {
        // properties
        EX_STREAM_READ,

        //
        __save_to_file,
        __read_next,
        __read_push,
        __read_pop,

        // methods
        __read_bool,
        __read_int,
        __read_size_t,
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
        __read_cstr,
        __read_string,
        __read_strid,
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
        __read_array,
        __read_map,

        // nodes
        NULL,
        NULL,
    }; // end of stream read

    // load node
    yajl_handle hand;
    ex_file_t *file;
    static unsigned char *fileData = NULL;
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
        (void *)NULL,
        (void *)NULL,
        (void *)NULL,
        (void *)NULL,
        NODE_STATUS_GET_VALUE,
        0
    };
    allocFuncs.ctx = (void *) &memCtx;

    //
    file = ex_fopen(_fileName,"r");
    if ( file == NULL ) // failed to open the file.
        return NULL;
    fileData = (unsigned char *) ex_malloc(bufSize);
    hand = yajl_alloc(&__callbacks, &cfg, &allocFuncs, &context);

    //
    done = 0;
	while (!done) {
        rd = ex_fread( file, fileData, 1, bufSize );
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
            unsigned char *str = yajl_get_error(hand, 0, fileData, rd);
            ex_error ( "failed to parse json file: %s. error: %s", _fileName, (char *)str );
            yajl_free_error(hand, str);
            break;
        }
    } 
    yajl_free(hand);
    ex_free(fileData);
    ex_fclose(file);

    // copy the read methods
    ex_stream_json_t *r_stream = (ex_stream_json_t *)ex_malloc( sizeof(ex_stream_json_t) );
    memcpy ( r_stream, &json_stream, sizeof(ex_stream_json_t) );
    r_stream->root = context.root;
    r_stream->anchor = context.root;
    r_stream->current = NULL;

    return (ex_stream_t *)r_stream;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_stream_t *ex_create_json_write_stream () {
    __json_node_t *node;
    static const ex_stream_json_t json_stream = {
        // properties
        EX_STREAM_WRITE,

        //
        __save_to_file,
        __write_next,
        __write_push,
        __write_pop,

        // methods
        __write_bool,
        __write_int,
        __write_size_t,
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
        __write_cstr,
        __write_string,
        __write_strid,
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
        __write_array,
        __write_map,

        // nodes
        NULL,
        NULL,
    }; // end of stream read

    // TODO: create node

    // copy the read methods
    ex_stream_json_t *w_stream = (ex_stream_json_t *)ex_malloc( sizeof(ex_stream_json_t) );
    memcpy ( w_stream, &json_stream, sizeof(ex_stream_json_t) );
    w_stream->root = node;
    w_stream->current = node;

    return (ex_stream_t *)w_stream;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_json_stream ( ex_stream_json_t *_stream ) {
    __destroy_node(_stream->root);
    ex_free(_stream);
}
