// ======================================================================================
// File         : stream_json.h
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 09:28:33 AM | Monday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef STREAM_JSON_H_1295227715
#define STREAM_JSON_H_1295227715
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// structures
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: ex_stream_json_t
// ------------------------------------------------------------------ 

typedef struct ex_stream_json_t {
    const ex_stream_t _;
    struct __json_node_t* root;
    struct __json_node_t* anchor; // locate current's parent
    struct __json_node_t* current;
} ex_stream_json_t;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_stream_t* ex_create_json_read_stream ( const char* _fileName );
extern ex_stream_t* ex_create_json_write_stream ();
extern void ex_destroy_json_stream ( ex_stream_json_t* _stream );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END STREAM_JSON_H_1295227715
// #################################################################################
