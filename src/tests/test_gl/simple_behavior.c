// ======================================================================================
// File         : simple_behavior.c
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 14:22:21 PM | Thursday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "simple_behavior.h"

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void init ( ex_ref_t *_self ) {
    __component_init(_self); // parent init
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void deinit ( ex_ref_t *_self ) {
    __component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void start ( ex_ref_t *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void update ( ex_ref_t *_self ) {
    ex_simple_t *self = EX_REF_CAST(ex_simple_t, _self);
    ex_component_t *comp = (ex_component_t *)self;
    ex_trans2d_t *trans2d = EX_REF_CAST( ex_trans2d_t, EX_REF_CAST(ex_entity_t,comp->entity)->trans2d );

    if ( trans2d ) {
        ex_vec2f_t new_pos;
        ex_angf_t new_ang;

        // process movement
        new_pos = ex_vec2f_zero;
        ex_vec2f_mul_scalar ( &new_pos, &self->move_dir, self->move_speed * ex_dt() );
        ex_trans2d_translate ( EX_REF_CAST(ex_entity_t,comp->entity)->trans2d, new_pos.x, new_pos.y, EX_SPACE_LOCAL );

        // process rotate
        ex_angf_set_by_radians( &new_ang, self->rot_speed * ex_dt() );
        switch ( self->rot_around ) {
        case ROT_AROUND_SELF:
            ex_trans2d_rotate ( EX_REF_CAST(ex_entity_t,comp->entity)->trans2d, new_ang.rad );
            break;

        case ROT_AROUND_WORLD:
            ex_trans2d_rotate_around ( EX_REF_CAST(ex_entity_t,comp->entity)->trans2d, 
                                       new_ang.rad, 
                                       &ex_vec2f_zero );
            break;

        case ROT_AROUND_PARENT:
            {
                ex_ref_t *parent = EX_REF_CAST(ex_trans2d_t,EX_REF_CAST(ex_entity_t,comp->entity)->trans2d)->parent; 
                ex_vec2f_t wd_pos;

                if ( parent ) {
                    ex_trans2d_world_position( parent, &wd_pos );
                    ex_trans2d_rotate_around ( EX_REF_CAST(ex_entity_t,comp->entity)->trans2d, 
                                               new_ang.rad, 
                                               &wd_pos );
                    ex_vec2f_set( &wd_pos, 0.0f, 1.0f );
                    ex_trans2d_set_up( EX_REF_CAST(ex_entity_t,comp->entity)->trans2d, 
                                       &wd_pos );
                }
            }
            break;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void post_update ( ex_ref_t *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void awake ( ex_ref_t *_self ) {
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_OBJECT_BEGIN( ex_simple_t,
                     "Simple Behavior",
                     init,
                     deinit )

    EX_MEMBER( ex_component_t, entity, NULL )
    EX_MEMBER( ex_component_t, enabled, true )

    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, awake, NULL )
    EX_MEMBER( ex_behavior_t, start, start )
    EX_MEMBER( ex_behavior_t, update, update )
    EX_MEMBER( ex_behavior_t, post_update, post_update )

    EX_MEMBER( ex_simple_t, move_dir, ex_vec2f_zero )
    EX_MEMBER( ex_simple_t, move_speed, 1.0f )
    EX_MEMBER( ex_simple_t, rot_speed, 1.0f )
    EX_MEMBER( ex_simple_t, rot_around, ROT_AROUND_PARENT )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_simple_t)
    EX_PROP ( ex_simple_t, vec2f, move_dir, "move direction", EX_PROP_ATTR_NONE )
    EX_PROP ( ex_simple_t, float, move_speed, "move speed", EX_PROP_ATTR_NONE )
    EX_PROP ( ex_simple_t, float, rot_speed, "rot spee", EX_PROP_ATTR_NONE )
    EX_PROP ( ex_simple_t, int, rot_around, "rot around", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_simple_t,ex_component_t)
    EX_MEMBER_SERIALIZE( vec2f, move_dir )
    EX_MEMBER_SERIALIZE( float, move_speed )
    EX_MEMBER_SERIALIZE( float, rot_speed )
    EX_MEMBER_SERIALIZE( int, rot_around )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_simple_t,ex_component_t)
    EX_MEMBER_TOSTRING( vec2f, "move direction", self->move_dir )
    EX_MEMBER_TOSTRING( float, "move speed", self->move_speed )
    EX_MEMBER_TOSTRING( float, "rotate speed", self->rot_speed )
    EX_MEMBER_TOSTRING( float, "rotate around", self->rot_around )
EX_DEF_TOSTRING_END
