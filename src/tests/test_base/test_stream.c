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

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// static const char* media_file = "e:/project/dev/exsdk/res/";
static const char* media_file = "/Users/Johnny/dev/projects/exdev/exsdk/res/";
#define maxPATH 256

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void json_read () {
    char path[maxPATH];

    strncpy ( path, media_file, maxPATH );
    ex_stream_t* stream = ex_create_json_read_stream( strcat(path, "simple_json.json") );
    ex_destroy_json_stream((ex_stream_json_t*)stream);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_stream () {
    json_read();
}
