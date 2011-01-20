// ======================================================================================
// File         : test_stream.c
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 15:58:01 PM | Monday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../entity/trans2d.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// static const char *media_file = "e:/project/dev/exsdk/res/";
static const char *media_file = "/Users/Johnny/dev/projects/exdev/exsdk/res/";
#define maxPATH 256

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void json_read () {
    char path[maxPATH];

    strncpy ( path, media_file, maxPATH );
    ex_stream_t *stream = ex_create_json_read_stream( strcat(path, "simple_json.json") );
    ex_destroy_json_stream((ex_stream_json_t *)stream);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void json_serialize () {
    typedef void (*pfn) ( ex_stream_t *, strid_t, void * );
    char path[maxPATH];
    pfn serialize_func;
    ex_trans2d_t *trans1, *trans2;

    strncpy ( path, media_file, maxPATH );
    ex_stream_t *stream = ex_create_json_read_stream( strcat(path, "trans2d.json") );
    serialize_func = (pfn)ex_rtti_get_serialize_pfn(EX_CLASSID(ex_trans2d_t));

    // serialize trans1
    trans1 = ex_create_ex_trans2d_t();
    ex_trans2d_init(trans1);
    serialize_func(stream, ex_strid("trans2d_01"), trans1 );

    // serialize trans2
    trans2 = ex_create_ex_trans2d_t();
    ex_trans2d_init(trans2);
    serialize_func(stream, ex_strid("trans2d_02"), trans2 );

    //
    ex_destroy_json_stream((ex_stream_json_t *)stream);

    // destroy
    ex_trans2d_deinit(trans1);
    ex_free(trans1);
    ex_trans2d_deinit(trans2);
    ex_free(trans2);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_stream () {
    // json_read();
    json_serialize();
}
