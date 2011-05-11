// ======================================================================================
// File         : widget.c
// Author       : Wu Jie 
// Last Change  : 05/08/2011 | 12:53:30 PM | Sunday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"

#include "widget.h"
#include <cogl/cogl.h>

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

G_DEFINE_TYPE (ClutterWidget, clutter_widget, CLUTTER_TYPE_GROUP);

enum {
    PROP_0,
    // PROP_BORDER_WIDTH,
    PROP_BORDER_COLOR
};

#define CLUTTER_WIDGET_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_WIDGET, ClutterWidgetPrivate))

struct _ClutterWidgetPrivate {
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

static void on_drag_begin ( ClutterDragAction *_action,
                            ClutterActor *_actor,
                            gfloat _event_x,
                            gfloat _event_y,
                            ClutterModifierType _modifiers )
{
    ex_log("on drag begin.");
    // clutter_actor_animate (_actor, CLUTTER_LINEAR, 150,
    //                        "@effects.disable.factor", 1.0,
    //                        NULL);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void on_drag_end ( ClutterDragAction *_action,
                          ClutterActor *_actor,
                          gfloat _event_x,
                          gfloat _event_y,
                          ClutterModifierType _modifiers )
{
    ex_log("on drag end.");
    // clutter_actor_animate (_actor, CLUTTER_LINEAR, 150,
    //                        "@effects.disable.factor", 0.0,
    //                        NULL);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean clutter_widget_get_paint_volume ( ClutterActor *self,
                                                  ClutterPaintVolume *volume )
{
    // TODO: learn clutter-group, it looks more effeicent
    return FALSE;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_paint ( ClutterActor *_self ) {
    ClutterGeometry geom;
    ClutterWidgetPrivate *priv;
    int borderWidth = 1;

    priv = CLUTTER_WIDGET_GET_PRIVATE (_self);
    clutter_actor_get_geometry (_self, &geom);

    cogl_push_matrix ();
        cogl_path_round_rectangle ( -borderWidth, -borderWidth, 
                                    geom.width + borderWidth, geom.height + borderWidth, 
                                    5, 1 );
        cogl_set_source_color4ub ( priv->borderColor.red, 
                                   priv->borderColor.green, 
                                   priv->borderColor.blue, 
                                   priv->borderColor.alpha );
        cogl_path_fill ();

        cogl_path_round_rectangle ( 0, 0, geom.width, geom.height, 5, 1 );
        cogl_set_source_color4ub ( 0xcf, 0xcf, 0xcf, 0xff );
        cogl_path_fill ();
    cogl_pop_matrix();

    // call the parent paint
    CLUTTER_ACTOR_CLASS (clutter_widget_parent_class)->paint (_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_set_property ( GObject      *_object,
                                          guint         _prop_id,
                                          const GValue *_value,
                                          GParamSpec   *_pspec )
{
    ClutterWidget *widget = CLUTTER_WIDGET(_object);

    switch (_prop_id) {

    case PROP_BORDER_COLOR:
        clutter_widget_set_border_color (widget, g_value_get_boxed (_value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (_object, _prop_id, _pspec);
        break;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_get_property ( GObject    *_object,
                                          guint       _prop_id,
                                          GValue     *_value,
                                          GParamSpec *_pspec )
{
    ClutterWidget *widget = CLUTTER_WIDGET(_object);
    ClutterColor color;

    switch (_prop_id) {

    case PROP_BORDER_COLOR:
        clutter_widget_get_border_color (widget, &color);
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

static void clutter_widget_finalize ( GObject *_object ) {
    G_OBJECT_CLASS (clutter_widget_parent_class)->finalize (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_dispose ( GObject *_object ) {
    ClutterWidgetPrivate *priv;

    priv = CLUTTER_WIDGET_GET_PRIVATE (_object);

    G_OBJECT_CLASS (clutter_widget_parent_class)->dispose (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_class_init ( ClutterWidgetClass *_klass ) {

    GObjectClass *gobject_class = G_OBJECT_CLASS (_klass);
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (_klass);
    GParamSpec *pspec;

    /* Provide implementations for ClutterActor vfuncs: */
    actor_class->get_paint_volume = clutter_widget_get_paint_volume;
    actor_class->paint = clutter_widget_paint;

    gobject_class->finalize     = clutter_widget_finalize;
    gobject_class->dispose      = clutter_widget_dispose;
    gobject_class->set_property = clutter_widget_set_property;
    gobject_class->get_property = clutter_widget_get_property;

    // ClutterWidget:color:
    // The color of the widget.
    pspec = g_param_spec_boxed ( "border-color",
                                 "Border Color",
                                 "The border color of widget",
                                 CLUTTER_TYPE_COLOR,
                                 G_PARAM_READABLE|G_PARAM_WRITABLE );
    g_object_class_install_property ( gobject_class,
                                      PROP_BORDER_COLOR,
                                      pspec );

    //
    g_type_class_add_private (gobject_class, sizeof (ClutterWidgetPrivate));
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_widget_init ( ClutterWidget *_self ) {
    ClutterWidgetPrivate *priv;
    ClutterAction *action;
    ClutterState *state;

    _self->priv = priv = CLUTTER_WIDGET_GET_PRIVATE (_self);

    priv->borderColor = *CLUTTER_COLOR_LightGray;
    clutter_actor_set_reactive ( CLUTTER_ACTOR(_self), TRUE );

    // TODO: only in drag-bar { 
    action = clutter_drag_action_new ();
    clutter_drag_action_set_drag_threshold (CLUTTER_DRAG_ACTION (action),
                                            0,
                                            0);
    clutter_drag_action_set_drag_axis ( CLUTTER_DRAG_ACTION (action), CLUTTER_DRAG_X_AXIS&CLUTTER_DRAG_Y_AXIS );
    g_signal_connect (action, "drag-begin", G_CALLBACK (on_drag_begin), NULL);
    g_signal_connect (action, "drag-end", G_CALLBACK (on_drag_end), NULL);
    clutter_actor_add_action (CLUTTER_ACTOR(_self), action);
    clutter_actor_add_effect_with_name (CLUTTER_ACTOR(_self), "disable", clutter_desaturate_effect_new (0.0));
    // } TODO end 

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

ClutterActor *clutter_widget_new (void) {
    ClutterActor *actor = g_object_new (CLUTTER_TYPE_WIDGET, NULL);
    return actor;
}

// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// ClutterActor *clutter_widget_new_with_color ( const ClutterColor *_color ) {
//     ClutterActor *actor = g_object_new ( CLUTTER_TYPE_WIDGET,
//                                          "color", _color,
//                                          NULL );
//     return actor;
// }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_widget_get_border_color ( ClutterWidget *_widget,
                                       ClutterColor *_color )
{
    ClutterWidgetPrivate *priv;

    g_return_if_fail (CLUTTER_IS_WIDGET (_widget));
    g_return_if_fail (_color != NULL);

    priv = _widget->priv;

    _color->red = priv->borderColor.red;
    _color->green = priv->borderColor.green;
    _color->blue = priv->borderColor.blue;
    _color->alpha = priv->borderColor.alpha;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_widget_set_border_color ( ClutterWidget *_widget,
                                       const ClutterColor *_color )
{
    ClutterWidgetPrivate *priv;

    g_return_if_fail (CLUTTER_IS_WIDGET (_widget));
    g_return_if_fail (_color != NULL);

    g_object_ref (_widget);

    priv = _widget->priv;

    priv->borderColor.red   = _color->red;
    priv->borderColor.green = _color->green;
    priv->borderColor.blue  = _color->blue;
    priv->borderColor.alpha = _color->alpha;

    if ( CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (_widget)) )
        clutter_actor_queue_redraw (CLUTTER_ACTOR (_widget));

    g_object_notify (G_OBJECT (_widget), "border-color");
    g_object_unref (_widget);
}

