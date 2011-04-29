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
extern void __behavior_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __debug2d_init ( ex_ref_t *_self ) {
    __behavior_init(_self); // parent init
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __behavior_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __debug2d_deinit ( ex_ref_t *_self ) {
    ex_debug2d_t *self = EX_REF_CAST(ex_debug2d_t,_self);
    ex_stop_timer(self->trail_timer);

    __behavior_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __debug2d_start ( ex_ref_t * );
extern void __debug2d_update ( ex_ref_t * );
extern void __debug2d_post_update ( ex_ref_t * );
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_debug2d_t,
                     "Debug 2D",
                     __debug2d_init,
                     __debug2d_deinit )

    EX_MEMBER( ex_component_t, entity, NULL )

    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, enabled, true )
    EX_MEMBER( ex_behavior_t, awake, NULL )
    EX_MEMBER( ex_behavior_t, start, __debug2d_start )
    EX_MEMBER( ex_behavior_t, update, __debug2d_update )
    EX_MEMBER( ex_behavior_t, post_update, __debug2d_post_update )
    EX_MEMBER( ex_behavior_t, on_render, NULL )

    EX_MEMBER( ex_debug2d_t, shapeType, EX_DEBUG_SHAPE_RECT )
    ex_rectf_set( &(((ex_debug2d_t *)__obj__)->rect), ex_vec2f_zero, 1.0f, 1.0f );
    ex_circlef_set( &(((ex_debug2d_t *)__obj__)->circle), ex_vec2f_zero, 1.0f );
    EX_MEMBER( ex_debug2d_t, trail_idx, 0 )
    EX_MEMBER( ex_debug2d_t, trail_timer, -1 )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_debug2d_t)
    EX_PROP( ex_debug2d_t, int, shapeType, "shape type",  EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_debug2d_t,ex_component_t)
    EX_MEMBER_SERIALIZE( int, shapeType )
    EX_SERIALIZE( _stream, vec2f, "rect_center", &(self->rect.center) )
    EX_SERIALIZE( _stream, float, "rect_width", &(self->rect.width) )
    EX_SERIALIZE( _stream, float, "rect_height", &(self->rect.height) )
    // TODO: you can serialize "ex_vec2f_t trails[EX_MAX_TRAIL_VERTS]"
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
    ex_debug2d_t *self = EX_REF_CAST(ex_debug2d_t,_self);
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
#if ( EX_PLATFORM != EX_IOS )
    ex_debug2d_t *self = EX_REF_CAST(ex_debug2d_t,_self);
    ex_entity_t *ent = EX_REF_CAST( ex_entity_t, ((ex_component_t *)self)->entity );
    ex_vec2f_t worldPos;
    ex_vec2f_t worldScale;
    ex_angf_t worldRot;

    static const bool show_coord = true;
    static const bool show_parentlink = true;
    static const bool show_trail = true;

    //
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
        ex_trans2d_world_angle( ent->trans2d, &worldRot );

        {
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glTranslatef(self->rect.center.x + worldPos.x, self->rect.center.y + worldPos.y, 0.0f);
            glRotatef(ex_angf_to_degrees_360(&worldRot), 0.0f, 0.0f, 1.0f);
        }

        // draw non-scale geometry first
        if ( show_coord ) {
            glBegin(GL_LINES); {
                glScalef( 1.0f, 1.0f, 1.0f );
                glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(20.0f, 0.0f, 0.0f);

                glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 20.0f, 0.0f);
            } glEnd();
        }

        //
        if ( show_parentlink ) {
            ex_ref_t *parent = EX_REF_CAST(ex_trans2d_t,ent->trans2d)->parent;
            if ( parent ) {
                ex_vec2f_t my_worldpos,parent_worldpos;

                ex_trans2d_world_position( ent->trans2d, &my_worldpos );
                ex_trans2d_world_position( parent, &parent_worldpos );

                glPushMatrix();
                glMatrixMode( GL_MODELVIEW );
                glLoadIdentity();
                glBegin(GL_LINES); {
                    glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
                    glVertex3f( my_worldpos.x, my_worldpos.y, 0.0f );
                    glVertex3f( parent_worldpos.x, parent_worldpos.y, 0.0f );
                } glEnd();
                glPopMatrix();
            }
        }

        //
        if ( show_trail ) {
            ex_vec2f_t my_worldpos;
            ex_trans2d_world_position( ent->trans2d, &my_worldpos );

            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glBegin(GL_LINE_STRIP); {
                int cnt,i;

                glColor4f( 1.0f, 0.5f, 1.0f, 1.0f );

                cnt = 0;
                i = (self->trail_idx+1)%EX_MAX_TRAIL_VERTS;
                while ( cnt < EX_MAX_TRAIL_VERTS ) {
                    ex_vec2f_t v2 = self->trails[i];
                    glVertex3f( v2.x, v2.y, 0.0f );

                    i = (i+1)%EX_MAX_TRAIL_VERTS; 
                    ++cnt;
                }
            } glEnd();
        }

        // draw scaled geometry
        {
            ex_mat33f_t r;

            glMatrixMode( GL_MODELVIEW );
            ex_trans2d_local_to_world_mat33f(ent->trans2d,&r);
            {
                float m[16] = {
                    r.m00, r.m01, r.m02, 0.0f,
                    r.m10, r.m11, r.m12, 0.0f,
                    0.0f,  0.0f,  1.0f,  0.0f,
                    r.m20, r.m21, r.m22, 1.0f,
                };
                glLoadMatrixf(m);

                glVertexPointer ( 2, GL_FLOAT, 0, verts );
                glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
                glDrawArrays(GL_LINE_LOOP, 0, 4);
            }
        }

    }
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int32 __add_trail ( uint32 _interval, void *_params ) {
    ex_ref_t *dbg2d_ref = *((ex_ref_t **)_params); 
    ex_debug2d_t *self = EX_REF_CAST(ex_debug2d_t,dbg2d_ref);
    ex_entity_t *ent = EX_REF_CAST( ex_entity_t, ((ex_component_t *)self)->entity );
    ex_vec2f_t worldPos;

    ex_trans2d_world_position( ent->trans2d, &worldPos );
    self->trail_idx = (self->trail_idx+1) % EX_MAX_TRAIL_VERTS; 
    self->trails[self->trail_idx] = worldPos;

    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __debug2d_start ( ex_ref_t *_self ) {
    ex_debug2d_t *self = EX_REF_CAST(ex_debug2d_t,_self);
    ex_entity_t *ent = EX_REF_CAST( ex_entity_t, ((ex_component_t *)self)->entity );
    ex_vec2f_t worldPos;
    int i;

    ex_trans2d_world_position( ent->trans2d, &worldPos );
    for ( i = 0; i < EX_MAX_TRAIL_VERTS; ++i )
        self->trails[i] = worldPos;

    self->trail_timer = ex_add_timer( __add_trail, 
                                      NULL,
                                      &_self, 
                                      sizeof(ex_ref_t *), 
                                      (timespan_t)0, 
                                      ex_timespan_from(0,50), 
                                      EX_TIMESPAN_INFINITY );
    ex_start_timer(self->trail_timer);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __debug2d_update ( ex_ref_t *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __debug2d_post_update ( ex_ref_t *_self ) {
}
