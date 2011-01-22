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
    ex_trans2d_t *obj = __alloc_ex_trans2d_t();
    ex_component_t *comp = (ex_component_t *)obj;
    comp->init = ex_trans2d_init;
    comp->deinit = ex_trans2d_deinit;
    return comp;
}

EX_DEF_PROPS_BEGIN(ex_trans2d_t)
    // EX_PROP( ex_trans2d_t, pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
    // EX_PROP( ex_trans2d_t, ang, "angle",  EX_PROP_ATTR_NONE, ex_prop_set_raw_angf, ex_prop_get_raw_angf )
    // EX_PROP( ex_trans2d_t, scale, "scale",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END(ex_trans2d_t)

EX_SERIALIZE_BEGIN(ex_trans2d_t)
    EX_SERIALIZE( vec2f, pos );
    EX_SERIALIZE( angf, ang );
    EX_SERIALIZE( vec2f, scale );
    // EX_SERIALIZE( reference, parent );
    // EX_SERIALIZE_ARRAY( float, children );
    // EX_SERIALIZE_MAP( strid, float, children_map );
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_trans2d_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_init ( void *_self ) {
    ex_trans2d_t *trans2d = (ex_trans2d_t *)_self; 

    ex_component_init(_self); // parent init
    ex_vec2f_zero(&trans2d->pos);
    ex_angf_zero(&trans2d->ang);
    ex_vec2f_one(&trans2d->scale);
    trans2d->parent = NULL;
    trans2d->children = ex_array_alloc( sizeof(ex_trans2d_t*), 8 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_deinit ( void *_self ) {
    ex_trans2d_t *trans2d = (ex_trans2d_t *)_self; 

    ex_array_free(trans2d->children);
    ex_component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_position ( ex_trans2d_t *_self, ex_vec2f_t *_pos ) {
    ex_trans2d_t *parent = _self->parent;
    *_pos = _self->pos;
    while ( parent ) {
        ex_vec2f_add( _pos, _pos, &(parent->pos) );
        parent = parent->parent;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_rotation ( ex_trans2d_t *_self, ex_angf_t *_ang ) {
    ex_trans2d_t *parent = _self->parent;
    *_ang = _self->ang;
    while ( parent ) {
        ex_angf_add( _ang, _ang, &(parent->ang) );
        parent = parent->parent;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_scale ( ex_trans2d_t *_self, ex_vec2f_t *_scale ) {
    ex_trans2d_t *parent = _self->parent;
    *_scale = _self->scale;
    while ( parent ) {
        ex_vec2f_mul( _scale, _scale, &(parent->scale) );
        parent = parent->parent;
    }
}
