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

EX_DEF_CLASS_BEGIN(ex_trans2d_t)

    // ======================================================== 
    // ex_object_t 
    // ======================================================== 

    EX_UID_INVALID, // uid
    EX_STRID_NULL, // name

    // ======================================================== 
    // ex_component_t
    // ======================================================== 

    NULL, // owner
    true, // active
    ex_trans2d_init, // init
    ex_trans2d_deinit, // deinit

    // ======================================================== 
    // ex_trans2d_t
    // ======================================================== 

    EX_VEC2F_ZERO, // pos 
    EX_ANGF_ZERO,  // ang
    EX_VEC2F_ONE,  // scale 
    NULL, // parent
    NULL, // children

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_trans2d_t)
    EX_PROP( ex_trans2d_t, vec2f, pos, "position",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, angf, ang, "angle",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, vec2f, scale, "scale",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_trans2d_t,ex_component_t)
    EX_MEMBER_SERIALIZE( vec2f, pos )
    EX_MEMBER_SERIALIZE( angf, ang )
    EX_MEMBER_SERIALIZE( vec2f, scale )
    // TODO { 
    // EX_MEMBER_SERIALIZE( reference, parent )
    // EX_MEMBER_SERIALIZE_ARRAY( reference, children )
    // } TODO end 
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_trans2d_t,ex_component_t)
    EX_MEMBER_TOSTRING ( vec2f, "position", self->pos )
    EX_MEMBER_TOSTRING ( angf, "angle", self->ang )
    EX_MEMBER_TOSTRING ( vec2f, "scale", self->scale )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_init ( void *_self ) {
    // ex_trans2d_t *trans2d = (ex_trans2d_t *)_self; 
    ex_component_init(_self); // parent init
    ((ex_object_t *)_self)->name = ex_strid("Transform 2D");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_deinit ( void *_self ) {
    ex_trans2d_t *trans2d = (ex_trans2d_t *)_self; 

    if ( trans2d->children )
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
