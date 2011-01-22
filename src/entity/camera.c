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

#include "world.h"
#include "entity.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_CREATOR(ex_camera_t) {
    ex_camera_t *obj = __alloc_ex_camera_t();
    ex_component_t *comp = (ex_component_t *)obj;
    comp->init = ex_camera_init;
    comp->deinit = ex_camera_deinit;
    return comp;
}

EX_DEF_PROPS_BEGIN(ex_camera_t)
    // EX_PROP( ex_camera_t, _pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
    // EX_PROP( ex_camera_t, _ang, "angle",  EX_PROP_ATTR_NONE, ex_prop_set_raw_angf, ex_prop_get_raw_angf )
    // EX_PROP( ex_camera_t, _scale, "scale",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END(ex_camera_t)

EX_SERIALIZE_BEGIN(ex_camera_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_camera_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_init ( void *_self ) {
    ex_component_t *comp = (ex_component_t *)_self; 
    ex_camera_t *cam = (ex_camera_t *)_self; 

    ex_component_init(_self); // parent init
    cam->isOrtho = true;       // default is 2D
    cam->orthoSize = 600/2;    // default is 800 x 600
    cam->aspect = 4.0f/3.0f;   // default is 4:3 
    cam->clearFlags = EX_CLEAR_COLOR;
    ex_color3f_set( &cam->bgColor, 0.0f, 0.5f, 1.0f );

    // add camera to the world.
    ex_world_add_camera( comp->owner->world, cam );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_deinit ( void *_self ) {
	ex_component_t *comp = (ex_component_t *)_self; 
    ex_camera_t *cam = (ex_camera_t *)_self; 

    ex_world_remove_camera( comp->owner->world, cam );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_ortho ( ex_camera_t *_self, bool _isOrtho ) {
    _self->isOrtho = _isOrtho;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_camera_is_ortho ( ex_camera_t *_self ) {
    return _self->isOrtho;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_ortho_size ( ex_camera_t *_self, float _orthoSize ) {
    _self->orthoSize = _orthoSize;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_camera_ortho_size ( ex_camera_t *_self ) {
    return _self->orthoSize;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_set_aspect ( ex_camera_t *_self, float _aspect ) {
    _self->aspect = _aspect;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_camera_aspect ( ex_camera_t *_self ) {
    return _self->aspect;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_camera_apply ( ex_camera_t *_self ) {
    GLbitfield clearFlags = 0;
    double rx = _self->orthoSize * _self->aspect; // half viewport width
    double ry = _self->orthoSize; // half viewport height

    // clear the screen
    glClearColor( _self->bgColor.r,
                  _self->bgColor.g,
                  _self->bgColor.b,
                  1.0f ); // RGBA background color
    if ( ex_flags_has( _self->clearFlags, EX_CLEAR_COLOR ) )
        clearFlags |= GL_COLOR_BUFFER_BIT;
    if ( ex_flags_has( _self->clearFlags, EX_CLEAR_DEPTH ) )
        clearFlags |= GL_DEPTH_BUFFER_BIT;
    glClear(clearFlags);

    // setup view matrix
    if ( _self->isOrtho ) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-rx, rx, -ry, ry, -1.0, 1.0);
        glTranslated(0.5, 0.5, 0.0);
    }
    else {
        // TODO:
    }
}
