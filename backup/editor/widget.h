// ======================================================================================
// File         : widget.h
// Author       : Wu Jie 
// Last Change  : 05/08/2011 | 12:50:37 PM | Sunday,May
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef WIDGET_H_1304830239
#define WIDGET_H_1304830239
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include <clutter/clutter.h>

G_BEGIN_DECLS

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

#define CLUTTER_TYPE_WIDGET                 (clutter_widget_get_type ())
#define CLUTTER_WIDGET(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_WIDGET, ClutterWidget))
#define CLUTTER_WIDGET_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_WIDGET, ClutterWidgetClass))
#define CLUTTER_IS_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_WIDGET))
#define CLUTTER_IS_WIDGET_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_WIDGET))
#define CLUTTER_WIDGET_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_WIDGET, ClutterWidgetClass))

typedef struct _ClutterWidget        ClutterWidget;
typedef struct _ClutterWidgetClass   ClutterWidgetClass;
typedef struct _ClutterWidgetPrivate ClutterWidgetPrivate;

//
struct _ClutterWidget {
    ClutterGroup parent;
    ClutterWidgetPrivate *priv;
}; 

//
struct _ClutterWidgetClass {
    ClutterGroupClass parent_class;
};

//
GType clutter_widget_get_type (void) G_GNUC_CONST;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

ClutterActor *clutter_widget_new (void);
// ClutterActor *clutter_widget_new_with_color ( const ClutterColor *_color );

void clutter_widget_get_border_color ( ClutterWidget *_widget, ClutterColor *_color );
void clutter_widget_set_border_color ( ClutterWidget *_widget, const ClutterColor *_color );

G_END_DECLS

// #################################################################################
#endif // END WIDGET_H_1304830239
// #################################################################################
