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
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_array_free(self->children);

    // destroy dirty mutex
    ex_destroy_mutex(self->dirty_mutex);

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

    EX_MEMBER( ex_component_t, entity, NULL )

    EX_MEMBER( ex_trans2d_t, local_pos, ex_vec2f_zero )
    EX_MEMBER( ex_trans2d_t, local_ang, ex_angf_zero )
    EX_MEMBER( ex_trans2d_t, local_scale, ex_vec2f_one )
    EX_MEMBER( ex_trans2d_t, parent, NULL )
    EX_MEMBER( ex_trans2d_t, children, ex_array( ref, 8 ) )
    EX_MEMBER( ex_trans2d_t, dirty, true )
    EX_MEMBER( ex_trans2d_t, localToWorld, ex_mat33f_identity )
    EX_MEMBER( ex_trans2d_t, dirty_mutex, ex_create_mutex() )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_trans2d_t)
    EX_PROP( ex_trans2d_t, vec2f, local_pos, "local position",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, angf, local_ang, "local angle",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_trans2d_t, vec2f, local_scale, "local scale",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_trans2d_t,ex_component_t)
    EX_MEMBER_SERIALIZE( vec2f, local_pos )
    EX_MEMBER_SERIALIZE( angf, local_ang )
    EX_MEMBER_SERIALIZE( vec2f, local_scale )
    EX_MEMBER_SERIALIZE( ref, parent )
    EX_MEMBER_SERIALIZE_ARRAY( ref, children )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_trans2d_t,ex_component_t)
    EX_MEMBER_TOSTRING ( vec2f, "local position", self->local_pos )
    EX_MEMBER_TOSTRING ( angf, "local angle", self->local_ang )
    EX_MEMBER_TOSTRING ( vec2f, "local scale", self->local_scale )
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
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

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
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    ex_array_append( self->children, &_child );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __matrix_dirty ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    self->dirty = true;

    ex_array_each ( self->children, ex_ref_t *, ref ) {
        ex_trans2d_t *child = EX_REF_CAST(ex_trans2d_t,ref);
        child->dirty = true;
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __update_matrix ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_trans2d_t *parent = self->parent ? EX_REF_CAST(ex_trans2d_t,self->parent) : NULL;
    bool ret;

    if ( !self->dirty ) {
        return;
    }

    ex_mutex_lock(self->dirty_mutex);

    // calculate the local to world matrix.
    // ex_mat33f_from_TRS( &(self->localToWorld), &self->local_pos, &self->local_ang, &ex_vec2f_one );
    ex_mat33f_from_TRS( &(self->localToWorld), &self->local_pos, &self->local_ang, &self->local_scale );

    if ( parent ) {
        ex_mat33f_t matParent;
        ex_trans2d_local_to_world_mat33f ( self->parent, &matParent );
        ex_mat33f_mul( &(self->localToWorld), &self->localToWorld, &matParent );
    }

    ret = ex_mat33f_get_inverse( &self->localToWorld, &self->worldToLocal );
    if ( ret == false ) {
        ex_vec2f_t v;
        v.x = -self->localToWorld.m20;
        v.y = -self->localToWorld.m21;
        self->worldToLocal = ex_mat33f_zero;
        ex_mat33f_set_translate( &self->worldToLocal, &v ); 
    }
    // ex_assert( ret == true, "failed to get inverse matrix, the determinant is zero" );
    self->dirty = false;

    ex_mutex_unlock(self->dirty_mutex);
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
    self = EX_REF_CAST(ex_trans2d_t,_self);

    // we got the same parent, nothing need to be changed.
    if ( self->parent == _parent )
        return;

    // get the world position under current parent
    ex_trans2d_world_position(_self,&self->local_pos);
    ex_trans2d_world_scale(_self,&self->local_scale);
    ex_trans2d_world_angle(_self,&self->local_ang);

    // remove self from old parent
    if ( self->parent ) {
        __remove_child( self->parent, _self );
    }

    // add self to new parent
    if ( _parent ) {
        ex_trans2d_t *parent = EX_REF_CAST(ex_trans2d_t,_parent);
        ex_angf_t parent_ang;
        ex_vec2f_t parent_scale;
        ex_vec3f_t pos3;

        __add_child( _parent, _self );

        __update_matrix(_parent);

        ex_trans2d_world_angle( _parent, &parent_ang );
        ex_angf_sub( &self->local_ang, &self->local_ang, &parent_ang );

        ex_trans2d_world_scale( _parent, &parent_scale );
        ex_vec2f_div( &self->local_scale, &self->local_scale, &parent_scale );

        ex_vec3f_set(&pos3, self->local_pos.x, self->local_pos.y, 1.0f );
        ex_vec3f_mul_mat33f( &pos3, &pos3, &parent->worldToLocal );
        ex_vec2f_set( &self->local_pos, pos3.x, pos3.y );
    }

    __matrix_dirty(_self);
    self->parent = _parent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_detach_children ( ex_ref_t *_self ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_trans2d_t *child;

    ex_array_each ( self->children, ex_ref_t *, ref ) {
        child = EX_REF_CAST(ex_trans2d_t,ref);

        // get the world position under current parent
        ex_trans2d_world_position(ref,&child->local_pos);
        ex_trans2d_world_scale(ref,&child->local_scale);
        ex_trans2d_world_angle(ref,&child->local_ang);
        __matrix_dirty(ref);

        //
        EX_REF_CAST( ex_trans2d_t, ref )->parent = NULL;
    } ex_array_each_end
    ex_array_remove_all(self->children);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_position ( ex_ref_t *_self, ex_vec2f_t *_pos ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_translate( _pos, &self->localToWorld );

    // KEEPME: the old method { 
    // ex_mat22f_t rot;
    // *_pos = self->local_pos;
    // ex_trans2d_t *parent = self->parent ? EX_REF_CAST(ex_trans2d_t,self->parent) : NULL;
    // while ( parent ) {
    //     ex_angf_to_rot22 ( &(parent->local_ang), &rot );
    //     ex_vec2f_mul_mat22f ( _pos, _pos, &rot );
    //     ex_vec2f_add( _pos, _pos, &(parent->local_pos) );
    //     parent = parent->parent ? EX_REF_CAST(ex_trans2d_t, parent->parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_angle ( ex_ref_t *_self, ex_angf_t *_ang ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_rotation( _ang, &self->localToWorld );

    // KEEPME { 
    // ex_trans2d_t *parent = self->parent ? EX_REF_CAST(ex_trans2d_t,self->parent) : NULL;
    // ex_ref_t *parent_parent = NULL;
    // *_ang = self->local_ang;
    // while ( parent ) {
    //     ex_angf_add( _ang, _ang, &(parent->local_ang) );

    //     parent_parent = parent->parent;
    //     parent = parent_parent ? EX_REF_CAST(ex_trans2d_t, parent_parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    __update_matrix(_self);
    ex_mat33f_get_scale( _scale, &self->localToWorld );

    // KEEPME { 
    // ex_trans2d_t *parent = self->parent ? EX_REF_CAST(ex_trans2d_t,self->parent) : NULL;
    // ex_ref_t *parent_parent = NULL;
    // *_scale = self->local_scale;
    // while ( parent ) {
    //     ex_vec2f_mul( _scale, _scale, &(parent->local_scale) );
    //     parent_parent = parent->parent;
    //     parent = parent_parent ? EX_REF_CAST(ex_trans2d_t, parent_parent) : NULL;
    // }
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_local_position ( ex_ref_t *_self, ex_vec2f_t *_pos ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    *_pos = self->local_pos; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_local_angle ( ex_ref_t *_self, ex_angf_t *_ang ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    *_ang = self->local_ang; 
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_local_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    *_scale = self->local_scale; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_right ( ex_ref_t *_self, ex_vec2f_t *_right ) {
    ex_angf_t ang;
    ex_mat22f_t rot;

    ex_trans2d_world_angle ( _self, &ang );
    ex_angf_to_rot22 ( &ang, &rot );
    ex_vec2f_set ( _right, 1.0f, 0.0f );
    ex_vec2f_mul_mat22f ( _right, _right, &rot );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_up ( ex_ref_t *_self, ex_vec2f_t *_up ) {
    ex_angf_t ang;
    ex_mat22f_t rot;

    ex_trans2d_world_angle ( _self, &ang );
    ex_angf_to_rot22 ( &ang, &rot );
    ex_vec2f_set ( _up, 0.0f, 1.0f );
    ex_vec2f_mul_mat22f ( _up, _up, &rot );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_local_to_world_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ) {
    __update_matrix (_self);
    *_result = EX_REF_CAST(ex_trans2d_t,_self)->localToWorld;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_world_to_local_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ) {
    __update_matrix (_self);
    *_result = EX_REF_CAST(ex_trans2d_t,_self)->worldToLocal;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_position ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->local_pos, _x, _y );
    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_angle ( ex_ref_t *_self, float _radians ) {
	ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
	
    ex_angf_set_by_radians ( &self->local_ang, _radians );
    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_local_scale ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    ex_vec2f_set ( &self->local_scale, _x, _y );
    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_position ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_ref_t *parent_ref = self->parent;

    if ( parent_ref ) {
        ex_vec3f_t v3;
        ex_trans2d_t *parent = EX_REF_CAST(ex_trans2d_t,parent_ref);

        __update_matrix(parent_ref);

        ex_vec3f_set ( &v3, _x, _y, 1.0f );
        ex_vec3f_mul_mat33f( &v3, &v3, &parent->worldToLocal );
        ex_vec2f_set ( &self->local_pos, v3.x, v3.y );
    }
    else {
        ex_vec2f_set ( &self->local_pos, _x, _y );
    }

    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_angle ( ex_ref_t *_self, float _radians ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_ref_t *parent_ref = self->parent;

    if ( parent_ref ) {
        ex_angf_t ang,parent_ang;

        ex_angf_set_by_radians( &ang, _radians );
        ex_trans2d_world_angle( parent_ref, &parent_ang );
        ex_angf_sub( &self->local_ang, &ang, &parent_ang );
    }
    else {
        ex_angf_set_by_radians( &self->local_ang, _radians );
    }

    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_world_scale ( ex_ref_t *_self, float _x, float _y ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_ref_t *parent_ref = self->parent;

    if ( parent_ref ) {
        ex_vec2f_t scale,parent_scale;

        ex_vec2f_set ( &scale, _x, _y );
        ex_trans2d_world_scale( parent_ref, &parent_scale );
        ex_vec2f_div( &self->local_scale, &scale, &parent_scale );
    }
    else {
        ex_vec2f_set ( &self->local_scale, _x, _y );
    }

    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_right ( ex_ref_t *_self, const ex_vec2f_t *_right ) {
    float sin_theta, cos_theta, rad;

    ex_vec2f_t w_right;
    ex_vec2f_set( &w_right, 1.0f, 0.0f );
    sin_theta = ex_vec2f_cross( &w_right, _right );
    cos_theta = ex_vec2f_dot( &w_right, _right );
    rad = atanf( sin_theta/cos_theta );

    if ( ex_signf(cos_theta) < 0.0f )
        rad += EX_PI;
    ex_trans2d_set_world_angle( _self, rad );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_set_up ( ex_ref_t *_self, const ex_vec2f_t *_up ) {
    float sin_theta, cos_theta, rad;

    ex_vec2f_t w_up;
    ex_vec2f_set( &w_up, 0.0f, 1.0f );
    sin_theta = ex_vec2f_cross( &w_up, _up );
    cos_theta = ex_vec2f_dot( &w_up, _up );
    rad = atanf( sin_theta/cos_theta );

    if ( ex_signf(cos_theta) < 0.0f )
        rad += EX_PI;
    ex_trans2d_set_world_angle( _self, rad );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_transform_point ( ex_ref_t *_self, const ex_vec2f_t *_pos, ex_vec2f_t *_oPos ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_vec3f_t v3;

    __update_matrix(_self);

    ex_vec3f_set ( &v3, _pos->x, _pos->y, 1.0f );
    ex_vec3f_mul_mat33f ( &v3, &v3, &self->localToWorld );
    ex_vec2f_set ( _oPos, v3.x, v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_inverse_transform_point ( ex_ref_t *_self, const ex_vec2f_t *_pos, ex_vec2f_t *_oPos ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_vec3f_t v3;

    __update_matrix(_self);

    ex_vec3f_set ( &v3, _pos->x, _pos->y, 1.0f );
    ex_vec3f_mul_mat33f ( &v3, &v3, &self->worldToLocal );
    ex_vec2f_set ( _oPos, v3.x, v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_transform_dir ( ex_ref_t *_self, const ex_vec2f_t *_dir, ex_vec2f_t *_oDir ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_vec3f_t v3;

    __update_matrix(_self);

    ex_vec3f_set ( &v3, _dir->x, _dir->y, 0.0f );
    ex_vec3f_mul_mat33f ( &v3, &v3, &self->localToWorld );
    ex_vec2f_set ( _oDir, v3.x, v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_inverse_transform_dir ( ex_ref_t *_self, const ex_vec2f_t *_dir, ex_vec2f_t *_oDir ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_vec3f_t v3;

    __update_matrix(_self);

    ex_vec3f_set ( &v3, _dir->x, _dir->y, 0.0f );
    ex_vec3f_mul_mat33f ( &v3, &v3, &self->worldToLocal );
    ex_vec2f_set ( _oDir, v3.x, v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_translate ( ex_ref_t *_self, float _x, float _y, int _space ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);

    if ( self->parent ) {
        if ( _space == EX_SPACE_SELF ) {
            ex_vec2f_t s;
            ex_vec2f_t v2;

            // NOTE: without scale, we can't get the same speed when translating.
            ex_trans2d_world_scale( self->parent, &s );
            ex_vec2f_set ( &v2, _x / s.x, _y / s.y );
            ex_vec2f_add ( &self->local_pos, &self->local_pos, &v2 );
        }
        else if ( _space == EX_SPACE_WORLD ) {
            ex_trans2d_t *parent = EX_REF_CAST(ex_trans2d_t,self->parent);
            ex_vec3f_t v3;

            __update_matrix(self->parent);

            // NOTE: use 0.0f will only apply direction
            ex_vec3f_set ( &v3, _x, _y, 0.0f );
            ex_vec3f_mul_mat33f( &v3, &v3, &parent->worldToLocal );
            ex_vec2f_add ( &self->local_pos, &self->local_pos, (ex_vec2f_t *)(&v3) ); // NOTE: yes, we can directly cast v3 to v2.
        }
    }
    else {
        ex_vec2f_t v2;
        ex_vec2f_set ( &v2, _x, _y );
        ex_vec2f_add ( &self->local_pos, &self->local_pos, &v2 );
    }

    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_translate_relative_to ( ex_ref_t *_self, float _x, float _y, ex_ref_t *_trans2d ) {
    ex_vec2f_t v2;
    ex_angf_t ang;
    ex_mat22f_t rot;

    __update_matrix(_trans2d);

    ex_trans2d_world_angle(_trans2d,&ang);
    ex_angf_to_rot22(&ang,&rot);
    ex_vec2f_set ( &v2, _x, _y );
    ex_vec2f_mul_mat22f( &v2, &v2, &rot );
    ex_trans2d_translate(_self, v2.x, v2.y, EX_SPACE_WORLD );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_rotate ( ex_ref_t *_self, float _radians ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_angf_t new_ang;

    ex_angf_set_by_radians( &new_ang, _radians );
    ex_angf_add( &self->local_ang, &self->local_ang, &new_ang );
    __matrix_dirty(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_rotate_around ( ex_ref_t *_self, float _radians, const ex_vec2f_t *_worldpos ) {
    ex_trans2d_t *self = EX_REF_CAST(ex_trans2d_t,_self);
    ex_angf_t rot_ang;
    ex_mat33f_t m3;
    ex_vec3f_t wpos_v3;
    ex_vec2f_t wpos_v2;

    // TODO: a simple way
    // TODO: I do remember Unreal have an easy rotate around algorithm, that only multiply order difference. 
#if 0
    ex_mat22f_t rot;
    ex_angf_set_by_radians( &rot_ang, _radians );
    ex_angf_to_rot22 ( &rot_ang, &rot );
    ex_mat33f_from_TRS( &m3, _worldpos, &rot_ang, &ex_vec2f_one );

    ex_trans2d_world_position( _self, &wpos_v2 );
    ex_vec3f_set( &wpos_v3, wpos_v2.x, wpos_v2.y, 1.0f );
    ex_mat33f_mul_vec3f( &wpos_v3, &m3, &wpos_v3 );
    // ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );
#else
    ex_angf_set_by_radians( &rot_ang, _radians );

    ex_trans2d_world_position( _self, &wpos_v2 );
    ex_vec3f_set( &wpos_v3, wpos_v2.x, wpos_v2.y, 1.0f );

    m3 = ex_mat33f_identity;
    ex_vec2f_get_neg ( _worldpos, &wpos_v2 );
    ex_mat33f_set_translate( &m3, &wpos_v2 );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    m3 = ex_mat33f_identity;
    ex_mat33f_set_rotation( &m3, &rot_ang );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    m3 = ex_mat33f_identity;
    ex_mat33f_set_translate( &m3, _worldpos );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    // NOTE: this will also change the orientation.
    ex_angf_add( &self->local_ang, &self->local_ang, &rot_ang );
#endif

    ex_trans2d_set_world_position( _self, wpos_v3.x, wpos_v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_trans2d_rotate_around_fix_orient ( ex_ref_t *_self, float _radians, const ex_vec2f_t *_worldpos ) {
    ex_angf_t rot_ang;
    ex_mat33f_t m3;
    ex_vec3f_t wpos_v3;
    ex_vec2f_t wpos_v2;

    // the only difference between this and rotate around is, we don't change the self-ang
    ex_angf_set_by_radians( &rot_ang, _radians );

    ex_trans2d_world_position( _self, &wpos_v2 );
    ex_vec3f_set( &wpos_v3, wpos_v2.x, wpos_v2.y, 1.0f );

    m3 = ex_mat33f_identity;
    ex_vec2f_get_neg ( _worldpos, &wpos_v2 );
    ex_mat33f_set_translate( &m3, &wpos_v2 );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    m3 = ex_mat33f_identity;
    ex_mat33f_set_rotation( &m3, &rot_ang );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    m3 = ex_mat33f_identity;
    ex_mat33f_set_translate( &m3, _worldpos );
    ex_vec3f_mul_mat33f( &wpos_v3, &wpos_v3, &m3 );

    ex_trans2d_set_world_position( _self, wpos_v3.x, wpos_v3.y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_trans2d_find ( ex_ref_t *_self, const char *_name ) {
    ex_array_t splits;
    ex_ref_t *current;

    // if this is a NULL or an empty string, just return NULL reference.
    if ( _name == NULL || strcmp( _name, "" ) == 0 )
        return NULL;

    // split name list by "/" and recursively search them.
    ex_array_init( &splits, EX_STRID_NULL, sizeof(char *), 8,
                   __ex_array_alloc,
                   __ex_array_realloc,
                   __ex_array_dealloc );
    ex_str_split_into_array( &splits, "/", _name );

    current = _self;
    ex_array_each( &splits, char *, text ) {
        ex_trans2d_t *current_trans = EX_REF_CAST(ex_trans2d_t,current);
        strid_t nameID = ex_strid(text);
        bool found = false;

        // find the name in children
        if ( current_trans->children ) {
            ex_array_each( current_trans->children, ex_ref_t *, child_ref ) {
                ex_object_t *ent = EX_REF_CAST( ex_object_t, EX_REF_CAST(ex_component_t,child_ref)->entity ); 
                if ( ent->name == nameID ) {
                    current = child_ref;
                    found = true;
                    break;
                }
            } ex_array_each_end
        }

        // if nothing found, return NULL directly 
        if ( !found ) {
            current = NULL;
            break;
        }
    } ex_array_each_end

    // free the array
    ex_array_each( &splits, char *, text ) {
        ex_free(text);
    } ex_array_each_end
    ex_array_deinit(&splits);

    return current;
}
