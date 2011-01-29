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
extern void __component_init ( void * );
// ------------------------------------------------------------------ 

static void init ( void *_self ) {
    __component_init(_self); // parent init
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( void * );
// ------------------------------------------------------------------ 

static void deinit ( void *_self ) {
    __component_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void level_start ( void *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void start ( void *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void update ( void *_self ) {
    ex_simple_t *self = (ex_simple_t *)_self;
    ex_component_t *comp = (ex_component_t *)_self;
    ex_trans2d_t *trans2d = comp->owner->trans2d;

    if ( trans2d ) {
        ex_vec2f_t new_pos;
        ex_angf_t new_ang;

        // process movement
        new_pos = ex_vec2f_zero;
        ex_vec2f_mul_scalar ( &new_pos, &self->move_dir, self->move_speed * ex_dt() );
        ex_vec2f_add ( &new_pos, &new_pos, &trans2d->pos );
        ex_vec2f_set ( &trans2d->pos, new_pos.x, new_pos.y );
        // ex_trans2d_world_position ( trans2d, &new_pos );

        // process rotate
        ex_angf_set_by_degrees( &new_ang, self->rot_speed * ex_dt() );
        ex_angf_add( &trans2d->ang, &new_ang, &trans2d->ang );
        // ex_trans2d_world_rotation ( trans2d, &new_ang );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void post_update ( void *_self ) {
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_OBJECT_BEGIN( ex_simple_t,
                     "Simple Behavior",
                     init,
                     deinit )

    EX_MEMBER( ex_component_t, owner, NULL )
    EX_MEMBER( ex_component_t, active, true )

    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, level_start, level_start )
    EX_MEMBER( ex_behavior_t, start, start )
    EX_MEMBER( ex_behavior_t, update, update )
    EX_MEMBER( ex_behavior_t, post_update, post_update )

    EX_MEMBER( ex_simple_t, move_dir, ex_vec2f_zero )
    EX_MEMBER( ex_simple_t, move_speed, 1.0f )
    EX_MEMBER( ex_simple_t, rot_speed, 1.0f )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_simple_t)
    EX_PROP ( ex_simple_t, vec2f, move_dir, "move direction", EX_PROP_ATTR_NONE )
    EX_PROP ( ex_simple_t, float, move_speed, "move speed", EX_PROP_ATTR_NONE )
    EX_PROP ( ex_simple_t, float, rot_speed, "rot spee", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_simple_t,ex_component_t)
    EX_MEMBER_SERIALIZE( vec2f, move_dir )
    EX_MEMBER_SERIALIZE( float, move_speed )
    EX_MEMBER_SERIALIZE( float, rot_speed )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_simple_t,ex_component_t)
    EX_MEMBER_TOSTRING( vec2f, "move direction", self->move_dir )
    EX_MEMBER_TOSTRING( float, "move speed", self->move_speed )
    EX_MEMBER_TOSTRING( float, "rotate speed", self->rot_speed )
EX_DEF_TOSTRING_END
