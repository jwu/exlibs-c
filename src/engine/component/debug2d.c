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
#include "trans2d.h"

#include "../entity.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_debug2d_t)

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
    ex_debug2d_init, // init
    NULL, // deinit

    // ======================================================== 
    // ex_debug2d_t
    // ======================================================== 

    EX_DEBUG_SHAPE_RECT, // shapeType
    EX_VEC2F_ZERO, 1.0f, 1.0f, // rect
    EX_VEC2F_ZERO, 1.0f, // circle
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_debug2d_t)
    EX_PROP( ex_debug2d_t, int, shapeType, "shape type",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_debug2d_t,ex_component_t)
    EX_MEMBER_SERIALIZE( int, shapeType )
    EX_SERIALIZE( _stream, vec2f, "rect_center", &(self->rect.center) )
    EX_SERIALIZE( _stream, float, "rect_width", &(self->rect.width) )
    EX_SERIALIZE( _stream, float, "rect_height", &(self->rect.height) )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_debug2d_t,ex_component_t)
    EX_MEMBER_TOSTRING( int, "shape type", self->shapeType )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_init ( void *_self ) {
    ex_rectf_t r;
    ex_circlef_t c;
    ex_vec2f_t center;
    ex_debug2d_t *dbg2d = (ex_debug2d_t *)_self; 

    ex_component_init(_self); // parent init
    ((ex_object_t *)_self)->name = ex_strid("Debug 2D");

    center = ex_vec2f_zero;
    ex_rectf_set ( &r, center, 1.0f, 1.0f );
    ex_circlef_set ( &c, center, 1.0f );

    dbg2d->shapeType = EX_DEBUG_SHAPE_RECT;
    dbg2d->rect = r;
    dbg2d->circle = c;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_deinit ( void *_self ) {
    ex_component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_set_rect ( ex_debug2d_t *_self, 
                           float _x, float _y, float _width, float _height ) 
{
    ex_rectf_t r;
    ex_vec2f_t v;

    ex_vec2f_set ( &v, _x, _y );
    ex_rectf_set ( &r, v, _width, _height );
    _self->rect = r;
    _self->shapeType = EX_DEBUG_SHAPE_RECT;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_draw ( ex_debug2d_t *_self ) {
    ex_entity_t *ent = ((ex_component_t *)_self)->owner;
    ex_trans2d_t *trans2d = ent->trans2d;
    ex_vec2f_t worldPos;
    ex_vec2f_t worldScale;
    ex_angf_t worldRot;

    if ( _self->shapeType == EX_DEBUG_SHAPE_RECT ) {
        float cx = _self->rect.center.x;
        float cy = _self->rect.center.y;
        float half_width = _self->rect.width * 0.5f; 
        float half_height = _self->rect.height * 0.5f; 
        float verts[8];

        verts[0] = cx - half_width; verts[1] = cy + half_height;  
        verts[2] = cx + half_width; verts[3] = cy + half_height;  
        verts[4] = cx + half_width; verts[5] = cy - half_height;  
        verts[6] = cx - half_width; verts[7] = cy - half_height;  

        ex_trans2d_world_position( trans2d, &worldPos );
        ex_trans2d_world_scale( trans2d, &worldScale );
        ex_trans2d_world_rotation( trans2d, &worldRot );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glTranslatef(_self->rect.center.x + worldPos.x, _self->rect.center.y + worldPos.y, 0.0f);
        glRotatef(ex_angf_to_degrees_360(&worldRot), 0.0f, 0.0f, 1.0f);
        glScalef(worldScale.x, worldScale.y, 1.0f);

        glVertexPointer ( 2, GL_FLOAT, 0, verts );
        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
}
