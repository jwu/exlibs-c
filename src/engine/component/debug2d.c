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

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __debug2d_init ( ex_ref_t *_self ) {
    __component_init(_self); // parent init
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __debug2d_deinit ( ex_ref_t *_self ) {
    __component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_debug2d_t,
                     "Debug 2D",
                     __debug2d_init,
                     __debug2d_deinit )

    EX_MEMBER( ex_component_t, owner, NULL )
    EX_MEMBER( ex_component_t, active, true )

    EX_MEMBER( ex_debug2d_t, shapeType, EX_DEBUG_SHAPE_RECT )
    ex_rectf_set( &(((ex_debug2d_t *)__obj__)->rect), ex_vec2f_zero, 1.0f, 1.0f );
    ex_circlef_set( &(((ex_debug2d_t *)__obj__)->circle), ex_vec2f_zero, 1.0f );

EX_DEF_OBJECT_END

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

void ex_debug2d_set_rect ( ex_ref_t *_self, 
                           float _x, float _y, float _width, float _height ) 
{
    ex_debug2d_t *self = EX_REF_PTR(ex_debug2d_t,_self);
    ex_rectf_t r;
    ex_vec2f_t v;

    ex_vec2f_set ( &v, _x, _y );
    ex_rectf_set ( &r, v, _width, _height );
    self->rect = r;
    self->shapeType = EX_DEBUG_SHAPE_RECT;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_draw ( ex_ref_t *_self ) {
    ex_debug2d_t *self = EX_REF_PTR(ex_debug2d_t,_self);
    ex_entity_t *ent = EX_REF_PTR( ex_entity_t, ((ex_component_t *)self)->owner );
    ex_vec2f_t worldPos;
    ex_vec2f_t worldScale;
    ex_angf_t worldRot;

    if ( self->shapeType == EX_DEBUG_SHAPE_RECT ) {
        float cx = self->rect.center.x;
        float cy = self->rect.center.y;
        float half_width = self->rect.width * 0.5f; 
        float half_height = self->rect.height * 0.5f; 
        float verts[8];

        verts[0] = cx - half_width; verts[1] = cy + half_height;  
        verts[2] = cx + half_width; verts[3] = cy + half_height;  
        verts[4] = cx + half_width; verts[5] = cy - half_height;  
        verts[6] = cx - half_width; verts[7] = cy - half_height;  

        ex_trans2d_world_position( ent->trans2d, &worldPos );
        ex_trans2d_world_scale( ent->trans2d, &worldScale );
        ex_trans2d_world_rotation( ent->trans2d, &worldRot );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glTranslatef(self->rect.center.x + worldPos.x, self->rect.center.y + worldPos.y, 0.0f);
        glRotatef(ex_angf_to_degrees_360(&worldRot), 0.0f, 0.0f, 1.0f);
        glScalef(worldScale.x, worldScale.y, 1.0f);

        glVertexPointer ( 2, GL_FLOAT, 0, verts );
        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
}
