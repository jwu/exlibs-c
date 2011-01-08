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
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_CREATOR(ex_trans2d_t) {
    ex_trans2d_t* obj = alloc_ex_trans2d_t();
    ex_component_t* comp = (ex_component_t*)obj;
    comp->init = ex_trans2d_init;
    comp->deinit = ex_trans2d_deinit;
    return comp;
}

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

void ex_trans2d_init ( void* _self ) {
    ex_trans2d_t* trans2d = (ex_trans2d_t*)_self; 

    ex_component_init(_self); // parent init
    ex_vec2f_zero(&trans2d->_pos);
    ex_angf_zero(&trans2d->_ang);
    ex_vec2f_zero(&trans2d->_scale);
    trans2d->_parent = NULL;
    trans2d->_childrent = ex_array_alloc( sizeof(ex_trans2d_t), 8 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_deinit ( void* _self ) {
    ex_trans2d_t* trans2d = (ex_trans2d_t*)_self; 

    ex_array_free(trans2d->_childrent);
    ex_component_deinit(_self); // parent deinint
}
