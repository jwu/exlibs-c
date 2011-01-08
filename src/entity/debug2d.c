// ======================================================================================
// File         : debug2d.c
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 16:44:18 PM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "debug2d.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_CREATOR(ex_debug2d_t) {
    ex_debug2d_t* obj = alloc_ex_debug2d_t();
    ex_component_t* comp = (ex_component_t*)obj;
    comp->init = ex_debug2d_init;
    comp->deinit = NULL;
    return comp;
}

EX_DEF_PROPS_BEGIN(ex_debug2d_t)
    // EX_PROP( ex_trans2d_t, _pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
    // EX_PROP( ex_trans2d_t, _ang, "angle",  EX_PROP_ATTR_NONE, ex_prop_set_raw_angf, ex_prop_get_raw_angf )
    // EX_PROP( ex_trans2d_t, _scale, "scale",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END(ex_debug2d_t)

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_init ( void* _self ) {
    ex_rectf_t r;
    ex_circlef_t c;
    ex_vec2f_t center;
    ex_debug2d_t* dbg2d = (ex_debug2d_t*)_self; 

    ex_component_init(_self); // parent init
    ex_vec2f_zero(&center);
    ex_rectf_set ( &r, center, 1.0f, 1.0f );
    ex_circlef_set ( &c, center, 1.0f );

    dbg2d->_shapeType = EX_DEBUG_SHAPE_RECT;
    dbg2d->_rect = r;
    dbg2d->_circle = c;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_deinit ( void* _self ) {
    ex_component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_set_rect ( ex_debug2d_t* _self, 
                           float _x, float _y, float _width, float _height ) 
{
    ex_rectf_t r;
    ex_vec2f_t v;

    ex_vec2f_set ( &v, _x, _y );
    ex_rectf_set ( &r, v, _width, _height );
    _self->_rect = r;
    _self->_shapeType = EX_DEBUG_SHAPE_RECT;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_draw ( ex_debug2d_t* _self ) {
    if ( _self->_shapeType == EX_DEBUG_SHAPE_RECT ) {
        float half_width = _self->_rect.width * 0.5f; 
        float half_height = _self->_rect.height * 0.5f; 

        // glColor3f(v,v,v);
        glRectf( _self->_rect.center.x - half_width, 
                 _self->_rect.center.y - half_height, 
                 _self->_rect.center.x + half_width, 
                 _self->_rect.center.y + half_height 
               );
    }
}
