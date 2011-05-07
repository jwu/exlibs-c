// ======================================================================================
// File         : view2d.h
// Author       : Wu Jie 
// Last Change  : 05/06/2011 | 11:03:30 AM | Friday,May
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef VIEW2D_H_1304651011
#define VIEW2D_H_1304651011
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include <clutter/clutter.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

G_BEGIN_DECLS

#define CLUTTER_TYPE_VIEW2D                 (clutter_view2d_get_type ())
#define CLUTTER_VIEW2D(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_VIEW2D, ClutterView2D))
#define CLUTTER_VIEW2D_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_VIEW2D, ClutterView2DClass))
#define CLUTTER_IS_VIEW2D(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_VIEW2D))
#define CLUTTER_IS_VIEW2D_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_VIEW2D))
#define CLUTTER_VIEW2D_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_VIEW2D, ClutterView2DClass))

typedef struct _ClutterView2D        ClutterView2D;
typedef struct _ClutterView2DClass   ClutterView2DClass;
typedef struct _ClutterView2DPrivate ClutterView2DPrivate;

//
struct _ClutterView2D {
    ClutterActor parent;
    ClutterView2DPrivate *priv;
}; 

//
struct _ClutterView2DClass {
    ClutterActorClass parent_class;
};

//
GType clutter_view2d_get_type (void) G_GNUC_CONST;

ClutterActor *clutter_view2d_new (void);
ClutterActor *clutter_view2d_new_with_color ( const ClutterColor *_color );

void clutter_view2d_get_color ( ClutterView2D *_view2d, ClutterColor *_color );
void clutter_view2d_set_color ( ClutterView2D *_view2d, const ClutterColor *_color );

G_END_DECLS

// #################################################################################
#endif // END VIEW2D_H_1304651011
// #################################################################################
