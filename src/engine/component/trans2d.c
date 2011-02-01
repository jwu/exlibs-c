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

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __trans2d_init ( ex_ref_t *_self ) {
    // parent init
    __component_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __trans2d_deinit ( ex_ref_t *_self ) {
    // NOTE: we do those childrent deinit in ex_entity_t instead of here. 
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_array_free(self->children);

    // parent deinint
    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_trans2d_t,
                     "Transform 2D",
                     __trans2d_init,
                     __trans2d_deinit )

    EX_MEMBER( ex_component_t, owner, NULL )
    EX_MEMBER( ex_component_t, active, true )

    EX_MEMBER( ex_trans2d_t, pos, ex_vec2f_zero )
    EX_MEMBER( ex_trans2d_t, ang, ex_angf_zero )
    EX_MEMBER( ex_trans2d_t, scale, ex_vec2f_zero )
    EX_MEMBER( ex_trans2d_t, parent, NULL )
    EX_MEMBER( ex_trans2d_t, children, ex_array( ref, 8 ) )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_trans2d_t)
    EX_PROP( ex_trans2d_t, vec2f, pos, "position",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, angf, ang, "angle",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, vec2f, scale, "scale",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_trans2d_t,ex_component_t)
    EX_MEMBER_SERIALIZE( vec2f, pos )
    EX_MEMBER_SERIALIZE( angf, ang )
    EX_MEMBER_SERIALIZE( vec2f, scale )
    EX_MEMBER_SERIALIZE( ref, parent )
    EX_MEMBER_SERIALIZE_ARRAY( ref, children )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_trans2d_t,ex_component_t)
    EX_MEMBER_TOSTRING ( vec2f, "position", self->pos )
    EX_MEMBER_TOSTRING ( angf, "angle", self->ang )
    EX_MEMBER_TOSTRING ( vec2f, "scale", self->scale )
    EX_MEMBER_TOSTRING ( ref, "parent", self->parent )
    EX_MEMBER_TOSTRING ( array, "childrent", self->children )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __remove_child ( ex_ref_t *_self, ex_ref_t *_child ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_array_each ( self->children, ex_ref_t *, ref ) {
        if ( ref == _child ) {
            ex_array_remove_at_fast( self->children, __idx__ );
            break;
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __add_child ( ex_ref_t *_self, ex_ref_t *_child ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_array_append( self->children, &_child );
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_parent ( ex_ref_t *_self, ex_ref_t *_parent ) {
    ex_trans2d_t *self;
    ex_assert_return ( _self != _parent, /*dummy*/, "can't not add self as parent." );

    self = EX_REF_PTR(ex_trans2d_t,_self);

    // we got the same parent, nothing need to be changed.
    if ( self->parent == _parent )
        return;

    // remove self from old parent
    if ( self->parent ) {
        __remove_child( self->parent, _self );
    }

    // add self to new parent
    if ( _parent ) {
        __add_child( _parent, _self );
    }
    self->parent = _parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_detach_children ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_array_each ( self->children, ex_ref_t *, ref ) {
        EX_REF_PTR( ex_trans2d_t, ref )->parent = NULL;
    } ex_array_each_end
    ex_array_remove_all(self->children);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_position ( ex_ref_t *_self, ex_vec2f_t *_pos ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    ex_ref_t *parent_parent = NULL;

    *_pos = self->pos;
    while ( parent ) {
        parent_parent = parent->parent;
        ex_vec2f_add( _pos, _pos, &(parent->pos) );
        parent = parent_parent ? EX_REF_PTR(ex_trans2d_t, parent_parent) : NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_rotation ( ex_ref_t *_self, ex_angf_t *_ang ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    ex_ref_t *parent_parent = NULL;

    *_ang = self->ang;
    while ( parent ) {
        parent_parent = parent->parent;
        ex_angf_add( _ang, _ang, &(parent->ang) );
        parent = parent_parent ? EX_REF_PTR(ex_trans2d_t, parent_parent) : NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    ex_ref_t *parent_parent = NULL;

    *_scale = self->scale;
    while ( parent ) {
        parent_parent = parent->parent;
        ex_vec2f_mul( _scale, _scale, &(parent->scale) );
        parent = parent_parent ? EX_REF_PTR(ex_trans2d_t, parent_parent) : NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_position ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->pos, _x, _y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_rotation ( ex_ref_t *_self, float _radians ) {
	ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
	
    ex_angf_set_by_radians ( &self->ang, _radians );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_scale ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->scale, _x, _y );
}
