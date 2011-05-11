// ======================================================================================
// File         : view2d.c
// Author       : Wu Jie 
// Last Change  : 05/06/2011 | 11:03:42 AM | Friday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"

#include "view2d.h"
#include <cogl/cogl.h>

extern float win_width;
extern float win_height;
extern ex_ref_t *g_world;
extern char *g_world_path;

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

G_DEFINE_TYPE (ClutterView2D, clutter_view2d, CLUTTER_TYPE_ACTOR);

enum {
    PROP_0,
    PROP_COLOR
};

#define CLUTTER_VIEW2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_VIEW2D, ClutterView2DPrivate))

struct _ClutterView2DPrivate {
    ClutterColor color;
    CoglHandle texture_id;
    CoglHandle offscreen_id;
};

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void clutter_render_world ( ex_ref_t *_world );
// ------------------------------------------------------------------ 

static void clutter_view2d_paint (ClutterActor *_self) {
    ClutterGeometry geom;
    CoglColor cogl_bg_color;

    ClutterView2DPrivate *priv = CLUTTER_VIEW2D(_self)->priv;
    CoglHandle material;
    
    // TODO: a way to update world { 
    // ClutterTimeline *timeline;
    // timeline = clutter_timeline_new (6000);
    // clutter_timeline_set_loop (timeline, TRUE);
    // g_signal_connect (timeline, "new-frame", G_CALLBACK (tick), view2d);
    // } TODO end 
    // update the world
    ex_world_update(g_world);

    //
    if ( priv->offscreen_id == COGL_INVALID_HANDLE ||
         priv->texture_id == COGL_INVALID_HANDLE )
        return;

    // render the frame-buffer
    clutter_actor_get_geometry (_self, &geom);
    cogl_push_framebuffer (priv->offscreen_id);
        cogl_color_set_from_4ub ( &cogl_bg_color, 
                                  priv->color.red, 
                                  priv->color.green, 
                                  priv->color.blue, 
                                  priv->color.alpha );
        cogl_clear ( &cogl_bg_color, COGL_BUFFER_BIT_COLOR|COGL_BUFFER_BIT_DEPTH );

        // do the world rendering
        clutter_render_world (g_world);
    cogl_pop_framebuffer ();

    cogl_push_matrix ();
        cogl_path_round_rectangle ( 0, 0, geom.width, geom.height, 10, 5 );
        cogl_clip_push_from_path();
            material = cogl_material_new ();
            cogl_material_set_color4ub (material, 0xff, 0xff, 0xff, priv->color.alpha);
            cogl_material_set_layer (material, 0, priv->texture_id);
            cogl_material_set_layer_filters ( material, 0, COGL_MATERIAL_FILTER_NEAREST, COGL_MATERIAL_FILTER_NEAREST );
            cogl_set_source (material);
            cogl_rectangle_with_texture_coords ( 0, 0, geom.width, geom.height,
                                                 0, 0, 1, 1 );
        cogl_clip_pop();

        cogl_path_round_rectangle ( 0, 0, geom.width, geom.height, 10, 5 );
        cogl_set_source_color4ub (0, 0, 0, 255);
        cogl_path_stroke ();
    cogl_pop_matrix();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_view2d_set_property ( GObject      *_object,
                                          guint         _prop_id,
                                          const GValue *_value,
                                          GParamSpec   *_pspec )
{
    ClutterView2D *view2d = CLUTTER_VIEW2D(_object);

    switch (_prop_id) {

    case PROP_COLOR:
        clutter_view2d_set_color (view2d, g_value_get_boxed (_value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (_object, _prop_id, _pspec);
        break;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_view2d_get_property ( GObject    *_object,
                                          guint       _prop_id,
                                          GValue     *_value,
                                          GParamSpec *_pspec )
{
    ClutterView2D *view2d = CLUTTER_VIEW2D(_object);
    ClutterColor color;

    switch (_prop_id) {

    case PROP_COLOR:
        clutter_view2d_get_color (view2d, &color);
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

static void clutter_view2d_finalize ( GObject *_object ) {
    G_OBJECT_CLASS (clutter_view2d_parent_class)->finalize (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_view2d_dispose ( GObject *_object ) {
    ClutterView2DPrivate *priv;

    priv = CLUTTER_VIEW2D_GET_PRIVATE (_object);

    cogl_handle_unref (priv->texture_id);
    cogl_handle_unref (priv->offscreen_id);

    G_OBJECT_CLASS (clutter_view2d_parent_class)->dispose (_object);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_view2d_class_init ( ClutterView2DClass *_klass ) {

    GObjectClass *gobject_class = G_OBJECT_CLASS (_klass);
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (_klass);
    GParamSpec *pspec;

    /* Provide implementations for ClutterActor vfuncs: */
    actor_class->paint = clutter_view2d_paint;

    gobject_class->finalize     = clutter_view2d_finalize;
    gobject_class->dispose      = clutter_view2d_dispose;
    gobject_class->set_property = clutter_view2d_set_property;
    gobject_class->get_property = clutter_view2d_get_property;

    // ClutterView2D:color:
    // The color of the view2d.
    pspec = g_param_spec_boxed ( "color",
                                 "Color",
                                 "The background color of view2d",
                                 CLUTTER_TYPE_COLOR,
                                 G_PARAM_READABLE|G_PARAM_WRITABLE );
    g_object_class_install_property ( gobject_class,
                                      PROP_COLOR,
                                      pspec );

    //
    g_type_class_add_private (gobject_class, sizeof (ClutterView2DPrivate));
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void clutter_view2d_init ( ClutterView2D *_self ) {
    ClutterView2DPrivate *priv;

    _self->priv = priv = CLUTTER_VIEW2D_GET_PRIVATE (_self);

    priv->color.red = 0xff;
    priv->color.green = 0xff;
    priv->color.blue = 0xff;
    priv->color.alpha = 0xff;

    priv->texture_id = COGL_INVALID_HANDLE;
    priv->offscreen_id = COGL_INVALID_HANDLE;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void on_actor_size_change ( GObject *_object, GParamSpec *_param_spec ) {

    gfloat width, height;
    ClutterView2DPrivate *priv;
    ClutterView2D *view2d;
    
    view2d = CLUTTER_VIEW2D(_object);
    priv = view2d->priv;

    // unref the old texture_id and offscreen_id if they are valid.
    if ( priv->texture_id != COGL_INVALID_HANDLE )
        cogl_handle_unref (priv->texture_id);
    if ( priv->offscreen_id != COGL_INVALID_HANDLE )
        cogl_handle_unref (priv->offscreen_id);

    //
    clutter_actor_get_size ( CLUTTER_ACTOR(view2d), &width, &height );

    // creating texture with size
    priv->texture_id = cogl_texture_new_with_size ( width, height,
                                                    COGL_TEXTURE_NONE,
                                                    COGL_PIXEL_FORMAT_RGB_888 );
    if ( priv->texture_id == COGL_INVALID_HANDLE )
        ex_error ("Failed creating texture with size!");

    // creating offscreen
    priv->offscreen_id = cogl_offscreen_new_to_texture ( priv->texture_id );
    if ( priv->offscreen_id == COGL_INVALID_HANDLE )
        ex_error ("Failed creating offscreen to texture!");

    // DISABLE { 
    // cogl_push_framebuffer (priv->offscreen_id);
    // cogl_set_viewport (0, 0, 800, 600);
    // setup_viewport (stage_width, stage_height,
    //                 perspective.fovy,
    //                 perspective.aspect,
    //                 perspective.z_near,
    //                 perspective.z_far);
    // cogl_pop_framebuffer ();
    // } DISABLE end 
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ClutterActor *clutter_view2d_new (void) {
    ClutterActor *actor = g_object_new (CLUTTER_TYPE_VIEW2D, NULL);
    g_signal_connect ( actor,
                       "notify::width",
                       G_CALLBACK(on_actor_size_change),
                       NULL );
    g_signal_connect ( actor,
                       "notify::height",
                       G_CALLBACK(on_actor_size_change),
                       NULL );
    return actor;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ClutterActor *clutter_view2d_new_with_color ( const ClutterColor *_color ) {
    ClutterActor *actor = g_object_new ( CLUTTER_TYPE_VIEW2D,
                                         "color", _color,
                                         NULL );
    g_signal_connect ( actor,
                       "notify::width",
                       G_CALLBACK(on_actor_size_change),
                       NULL );
    g_signal_connect ( actor,
                       "notify::height",
                       G_CALLBACK(on_actor_size_change),
                       NULL );
    return actor;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_view2d_get_color ( ClutterView2D *_view2d,
                                ClutterColor *_color )
{
    ClutterView2DPrivate *priv;

    g_return_if_fail (CLUTTER_IS_VIEW2D (_view2d));
    g_return_if_fail (_color != NULL);

    priv = _view2d->priv;

    _color->red = priv->color.red;
    _color->green = priv->color.green;
    _color->blue = priv->color.blue;
    _color->alpha = priv->color.alpha;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_view2d_set_color ( ClutterView2D *_view2d,
                                const ClutterColor *_color )
{
    ClutterView2DPrivate *priv;

    g_return_if_fail (CLUTTER_IS_VIEW2D (_view2d));
    g_return_if_fail (_color != NULL);

    g_object_ref (_view2d);

    priv = _view2d->priv;

    priv->color.red = _color->red;
    priv->color.green = _color->green;
    priv->color.blue = _color->blue;
    priv->color.alpha = _color->alpha;

    clutter_actor_set_opacity (CLUTTER_ACTOR (_view2d),
                               priv->color.alpha);

    if (CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (_view2d)))
        clutter_actor_queue_redraw (CLUTTER_ACTOR (_view2d));

    g_object_notify (G_OBJECT (_view2d), "color");
    g_object_unref (_view2d);
}

