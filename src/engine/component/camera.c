// ======================================================================================
// File         : camera.c
// Author       : Wu Jie 
// Last Change  : 01/09/2011 | 11:46:03 AM | Sunday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "camera.h"

#include "../world.h"
#include "../entity.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __camera_init ( ex_ref_t *_self ) {
    ex_component_t *comp = EX_REF_CAST(ex_component_t, _self);
    ex_entity_t *ent = EX_REF_CAST(ex_entity_t,comp->entity);

    __component_init(_self); // parent init

    // add camera to the world.
    ex_world_add_camera( ent->world, _self );
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __camera_deinit ( ex_ref_t *_self ) {
    ex_component_t *comp = EX_REF_CAST(ex_component_t, _self);
    ex_entity_t *ent = EX_REF_CAST(ex_entity_t,comp->entity);

    if ( ent ) {
        ex_world_remove_camera( ent->world, _self );
    }

    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_camera_t,
                     "Camera",
                     __camera_init,
                     __camera_deinit )

    EX_MEMBER( ex_component_t, entity, NULL )

    EX_MEMBER( ex_camera_t, isOrtho, false )
    EX_MEMBER( ex_camera_t, orthoSize, 600/2 )
    EX_MEMBER( ex_camera_t, aspect, 4.0f/3.0f )
    EX_MEMBER( ex_camera_t, clearFlags, EX_CLEAR_COLOR|EX_CLEAR_DEPTH )
    ex_color3f_set( &(((ex_camera_t *)__obj__)->bgColor), 0.0f, 0.5f, 1.0f );
    EX_MEMBER( ex_camera_t, matWorldToView, ex_mat44f_identity )
    EX_MEMBER( ex_camera_t, matProjection, ex_mat44f_identity )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_camera_t)
    EX_PROP( ex_camera_t, bool, isOrtho, "is ortho-graphic",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_camera_t, float, orthoSize, "ortho-graphic size",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_camera_t, float, aspect, "aspect",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_camera_t, uint32, clearFlags, "clear flags",  EX_PROP_ATTR_NONE )
    EX_PROP( ex_camera_t, color3f, bgColor, "background color",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_camera_t,ex_component_t)
    EX_MEMBER_SERIALIZE( bool, isOrtho )
    EX_MEMBER_SERIALIZE( float, orthoSize )
    EX_MEMBER_SERIALIZE( float, aspect )
    EX_MEMBER_SERIALIZE( uint32, clearFlags )
    EX_MEMBER_SERIALIZE( color3f, bgColor )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_camera_t,ex_component_t)
    EX_MEMBER_TOSTRING ( bool, "is ortho-graphic", self->isOrtho )
    EX_MEMBER_TOSTRING ( float, "ortho-graphic size", self->orthoSize )
    EX_MEMBER_TOSTRING ( float, "aspect", self->aspect )
    EX_MEMBER_TOSTRING ( uint32, "clear flags", self->clearFlags )
    EX_MEMBER_TOSTRING ( color3f, "background color", self->bgColor )
    EX_MEMBER_TOSTRING ( mat44f, "world to view matrix", self->matWorldToView )
    EX_MEMBER_TOSTRING ( mat44f, "projection matrix", self->matProjection )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_ortho ( ex_ref_t *_self, bool _isOrtho ) {
    EX_REF_CAST(ex_camera_t,_self)->isOrtho = _isOrtho;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_camera_is_ortho ( ex_ref_t *_self ) {
    return EX_REF_CAST(ex_camera_t,_self)->isOrtho;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_ortho_size ( ex_ref_t *_self, float _orthoSize ) {
    EX_REF_CAST(ex_camera_t,_self)->orthoSize = _orthoSize;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_camera_ortho_size ( ex_ref_t *_self ) {
    return EX_REF_CAST(ex_camera_t,_self)->orthoSize;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_aspect ( ex_ref_t *_self, float _aspect ) {
    EX_REF_CAST(ex_camera_t,_self)->aspect = _aspect;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_camera_aspect ( ex_ref_t *_self ) {
    return EX_REF_CAST(ex_camera_t,_self)->aspect;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_apply ( ex_ref_t *_self ) {
    ex_camera_t *self = EX_REF_CAST(ex_camera_t,_self);

    GLbitfield clearFlags = 0;
    double rx = self->orthoSize * self->aspect; // half viewport width
    double ry = self->orthoSize; // half viewport height

    // clear the screen
    glClearColor( self->bgColor.r,
                  self->bgColor.g,
                  self->bgColor.b,
                  1.0f ); // RGBA background color

    if ( ex_flags_has( self->clearFlags, EX_CLEAR_COLOR ) ) {
        clearFlags |= GL_COLOR_BUFFER_BIT;
    }

    if ( ex_flags_has( self->clearFlags, EX_CLEAR_DEPTH ) ) {
        clearFlags |= GL_DEPTH_BUFFER_BIT;
        ex_glClearDepth(1.0f);
    }
    glClear(clearFlags);

    // setup view matrix
    if ( self->isOrtho ) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        ex_glOrtho(-rx, rx, -ry, ry, -100.0, 100.0); // this will make camera look along -z
        ex_glTranslate(0.5, 0.5, 0.0);
    }
    else {
        // TODO:
    }
}
