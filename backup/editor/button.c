// ======================================================================================
// File         : button.c
// Author       : Wu Jie 
// Last Change  : 05/11/2011 | 09:18:18 AM | Wednesday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"

#include "button.h"
#include <cogl/cogl.h>

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

G_DEFINE_TYPE (ClutterButton, clutter_button, CLUTTER_TYPE_ACTOR);

enum {
    PROP_0,
    // PROP_BORDER_WIDTH,
    PROP_BORDER_COLOR
};

#define CLUTTER_BUTTON_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_BUTTON, ClutterButtonPrivate))

struct _ClutterButtonPrivate {
    ClutterColor borderColor;
};

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean on_enter ( ClutterActor *_actor, ClutterEvent *_event, gpointer _user_data ) {
    ClutterState *state = CLUTTER_STATE (_user_data);
    clutter_state_set_state (state, "hover");
    return TRUE;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean on_leave ( ClutterActor *_actor, ClutterEvent *_event, gpointer _user_data ) {
    ClutterState *state = CLUTTER_STATE (_user_data);
    clutter_state_set_state (state, "normal");
    return TRUE;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_paint ( ClutterActor *_self ) {
    ClutterGeometry geom;
    ClutterButtonPrivate *priv;
    int borderWidth = 1;

    priv = CLUTTER_BUTTON_GET_PRIVATE (_self);
    clutter_actor_get_geometry (_self, &geom);

    cogl_push_matrix ();
        cogl_path_round_rectangle ( -borderWidth, -borderWidth, 
                                    geom.width + borderWidth, geom.height + borderWidth, 
                                    2, 1 );
        cogl_set_source_color4ub ( priv->borderColor.red, 
                                   priv->borderColor.green, 
                                   priv->borderColor.blue, 
                                   priv->borderColor.alpha );
        cogl_path_fill ();

        cogl_path_round_rectangle ( 0, 0, geom.width, geom.height, 2, 1 );
        cogl_set_source_color4ub ( 0xcf, 0xcf, 0xcf, 0xff );
        cogl_path_fill ();
    cogl_pop_matrix();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_set_property ( GObject      *_object,
                                          guint         _prop_id,
                                          const GValue *_value,
                                          GParamSpec   *_pspec )
{
    ClutterButton *button = CLUTTER_BUTTON(_object);

    switch (_prop_id) {

    case PROP_BORDER_COLOR:
        clutter_button_set_border_color (button, g_value_get_boxed (_value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (_object, _prop_id, _pspec);
        break;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_get_property ( GObject    *_object,
                                          guint       _prop_id,
                                          GValue     *_value,
                                          GParamSpec *_pspec )
{
    ClutterButton *button = CLUTTER_BUTTON(_object);
    ClutterColor color;

    switch (_prop_id) {

    case PROP_BORDER_COLOR:
        clutter_button_get_border_color (button, &color);
        g_value_set_boxed (_value, &color);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (_object, _prop_id, _pspec);
        break;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_finalize ( GObject *_object ) {
    G_OBJECT_CLASS (clutter_button_parent_class)->finalize (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_dispose ( GObject *_object ) {
    ClutterButtonPrivate *priv;

    priv = CLUTTER_BUTTON_GET_PRIVATE (_object);

    G_OBJECT_CLASS (clutter_button_parent_class)->dispose (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_class_init ( ClutterButtonClass *_klass ) {

    GObjectClass *gobject_class = G_OBJECT_CLASS (_klass);
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (_klass);
    GParamSpec *pspec;

    /* Provide implementations for ClutterActor vfuncs: */
    actor_class->paint = clutter_button_paint;

    gobject_class->finalize     = clutter_button_finalize;
    gobject_class->dispose      = clutter_button_dispose;
    gobject_class->set_property = clutter_button_set_property;
    gobject_class->get_property = clutter_button_get_property;

    // ClutterButton:color:
    // The color of the button.
    pspec = g_param_spec_boxed ( "border-color",
                                 "Border Color",
                                 "The border color of button",
                                 CLUTTER_TYPE_COLOR,
                                 G_PARAM_READABLE|G_PARAM_WRITABLE );
    g_object_class_install_property ( gobject_class,
                                      PROP_BORDER_COLOR,
                                      pspec );

    //
    g_type_class_add_private (gobject_class, sizeof (ClutterButtonPrivate));
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_button_init ( ClutterButton *_self ) {
    ClutterButtonPrivate *priv;
    ClutterState *state;

    _self->priv = priv = CLUTTER_BUTTON_GET_PRIVATE (_self);

    priv->borderColor = *CLUTTER_COLOR_LightGray;
    clutter_actor_set_reactive ( CLUTTER_ACTOR(_self), TRUE );

    // init state machine
    state = clutter_state_new ();
    g_object_set_data_full ( G_OBJECT (_self), "hover-state-machine", state, g_object_unref );
    g_signal_connect ( _self, "enter-event", G_CALLBACK (on_enter), state );
    g_signal_connect ( _self, "leave-event", G_CALLBACK (on_leave), state );
    clutter_state_set ( state, NULL, "normal",
                        _self, "border-color", CLUTTER_LINEAR, CLUTTER_COLOR_DarkGray,
                        NULL );
    clutter_state_set ( state, NULL, "hover",
                        _self, "border-color", CLUTTER_LINEAR, CLUTTER_COLOR_LightSkyBlue,
                        NULL );
    clutter_state_set_duration ( state, NULL, NULL, 200 );

    // set init state
    clutter_state_set_state (state, "normal");
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ClutterActor *clutter_button_new (void) {
    ClutterActor *actor = g_object_new (CLUTTER_TYPE_BUTTON, NULL);
    return actor;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_button_get_border_color ( ClutterButton *_button,
                                       ClutterColor *_color )
{
    ClutterButtonPrivate *priv;

    g_return_if_fail (CLUTTER_IS_BUTTON (_button));
    g_return_if_fail (_color != NULL);

    priv = _button->priv;

    _color->red = priv->borderColor.red;
    _color->green = priv->borderColor.green;
    _color->blue = priv->borderColor.blue;
    _color->alpha = priv->borderColor.alpha;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_button_set_border_color ( ClutterButton *_button,
                                       const ClutterColor *_color )
{
    ClutterButtonPrivate *priv;

    g_return_if_fail (CLUTTER_IS_BUTTON (_button));
    g_return_if_fail (_color != NULL);

    g_object_ref (_button);

    priv = _button->priv;

    priv->borderColor.red   = _color->red;
    priv->borderColor.green = _color->green;
    priv->borderColor.blue  = _color->blue;
    priv->borderColor.alpha = _color->alpha;

    if ( CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (_button)) )
        clutter_actor_queue_redraw (CLUTTER_ACTOR (_button));

    g_object_notify (G_OBJECT (_button), "border-color");
    g_object_unref (_button);
}

