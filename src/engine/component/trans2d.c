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
    EX_MEMBER( ex_trans2d_t, dirty, true )
    EX_MEMBER( ex_trans2d_t, localToWorld, ex_mat33f_identity )

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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __update_matrix ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    bool ret;

    if ( !self->dirty ) {
        return;
    }

    // calculate the local to world matrix.
    // ex_mat33f_from_TRS( &(self->localToWorld), &self->pos, &self->ang, &ex_vec2f_one );
    ex_mat33f_from_TRS( &(self->localToWorld), &self->pos, &self->ang, &self->scale );

    if ( parent ) {
        ex_mat33f_t matParent;
        ex_trans2d_local_to_world_mat33f ( self->parent, &matParent );
        ex_mat33f_mul( &(self->localToWorld), &self->localToWorld, &matParent );
    }

    ret = ex_mat33f_get_inverse( &self->worldToLocal, &self->localToWorld );
    ex_assert( ret == true, "failed to get inverse matrix, the determinant is zero" );
    self->dirty = false;
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

    // get the world position under current parent
    ex_trans2d_world_position(_self,&self->pos);
    ex_trans2d_world_scale(_self,&self->scale);
    ex_trans2d_world_rotation(_self,&self->ang);

    // remove self from old parent
    if ( self->parent ) {
        __remove_child( self->parent, _self );
    }

    // add self to new parent
    if ( _parent ) {
        ex_trans2d_t *parent = EX_REF_PTR(ex_trans2d_t,_parent);
        ex_angf_t parent_ang;
        ex_vec2f_t parent_scale;
        ex_vec3f_t pos3;

        __add_child( _parent, _self );

        __update_matrix(_parent);

        ex_trans2d_world_rotation( _parent, &parent_ang );
        ex_angf_sub( &self->ang, &self->ang, &parent_ang );

        ex_trans2d_world_scale( _parent, &parent_scale );
        ex_vec2f_div( &self->scale, &self->scale, &parent_scale );

        ex_vec3f_set(&pos3, self->pos.x, self->pos.y, 1.0f );
        ex_vec3f_mul_mat33f( &pos3, &pos3, &parent->worldToLocal );
        ex_vec2f_set( &self->pos, pos3.x, pos3.y );
    }

    self->dirty = true;
    self->parent = _parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_detach_children ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
    ex_trans2d_t *child;

    ex_array_each ( self->children, ex_ref_t *, ref ) {
        child = EX_REF_PTR(ex_trans2d_t,ref);

        // get the world position under current parent
        ex_trans2d_world_position(ref,&child->pos);
        ex_trans2d_world_scale(ref,&child->scale);
        ex_trans2d_world_rotation(ref,&child->ang);
        child->dirty = true;

        //
        EX_REF_PTR( ex_trans2d_t, ref )->parent = NULL;
    } ex_array_each_end
    ex_array_remove_all(self->children);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_position ( ex_ref_t *_self, ex_vec2f_t *_pos ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_translate( _pos, &self->localToWorld );

    // KEEPME: the old method { 
    // ex_mat22f_t rot;
    // *_pos = self->pos;
    // ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    // while ( parent ) {
    //     ex_angf_to_rot22 ( &(parent->ang), &rot );
    //     ex_vec2f_mul_mat22f ( _pos, _pos, &rot );
    //     ex_vec2f_add( _pos, _pos, &(parent->pos) );
    //     parent = parent->parent ? EX_REF_PTR(ex_trans2d_t, parent->parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_rotation ( ex_ref_t *_self, ex_angf_t *_ang ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_rotation( _ang, &self->localToWorld );

    // KEEPME { 
    // ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    // ex_ref_t *parent_parent = NULL;
    // *_ang = self->ang;
    // while ( parent ) {
    //     ex_angf_add( _ang, _ang, &(parent->ang) );

    //     parent_parent = parent->parent;
    //     parent = parent_parent ? EX_REF_PTR(ex_trans2d_t, parent_parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_scale( _scale, &self->localToWorld );

    // KEEPME { 
    // ex_trans2d_t *parent = self->parent ? EX_REF_PTR(ex_trans2d_t,self->parent) : NULL;
    // ex_ref_t *parent_parent = NULL;
    // *_scale = self->scale;
    // while ( parent ) {
    //     ex_vec2f_mul( _scale, _scale, &(parent->scale) );
    //     parent_parent = parent->parent;
    //     parent = parent_parent ? EX_REF_PTR(ex_trans2d_t, parent_parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_local_to_world_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ) {
    __update_matrix (_self);
    *_result = EX_REF_PTR(ex_trans2d_t,_self)->localToWorld;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_to_local_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ) {
    __update_matrix (_self);
    *_result = EX_REF_PTR(ex_trans2d_t,_self)->worldToLocal;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_position ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->pos, _x, _y );
    self->dirty = true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_rotation ( ex_ref_t *_self, float _radians ) {
	ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);
	
    ex_angf_set_by_radians ( &self->ang, _radians );
    self->dirty = true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_scale ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_PTR(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->scale, _x, _y );
    self->dirty = true;
}
