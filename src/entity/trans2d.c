// ======================================================================================
// File         : trans2d.c
// Author       : Wu Jie 
// Last Change  : 11/30/2010 | 16:55:27 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "trans2d.h"

///////////////////////////////////////////////////////////////////////////////
// static 
///////////////////////////////////////////////////////////////////////////////

static ex_array_t* _trans2d_pool = NULL;

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_PROPS_BEGIN(ex_trans2d_t)
    // EX_PROP( ex_trans2d_t, _pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
    // EX_PROP( ex_trans2d_t, _ang, "angle",  EX_PROP_ATTR_NONE, ex_prop_set_raw_angf, ex_prop_get_raw_angf )
    // EX_PROP( ex_trans2d_t, _scale, "scale",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END(ex_trans2d_t)

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_trans2d_t* ex_trans2d_request () {
    return ex_array_append( _trans2d_pool, NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_init ( ex_trans2d_t* _comp ) {
    ex_vec2f_zero(&_comp->_pos);
    ex_angf_zero(&_comp->_ang);
    ex_vec2f_zero(&_comp->_scale);
    _comp->_parent = NULL;
    _comp->_childrent = ex_array_alloc( sizeof(ex_trans2d_t), 8 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_deinit ( ex_trans2d_t* _comp ) {
    ex_array_free(_comp->_childrent);
}
