// ======================================================================================
// File         : button.h
// Author       : Wu Jie 
// Last Change  : 05/11/2011 | 09:14:05 AM | Wednesday,May
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BUTTON_H_1305076459
#define BUTTON_H_1305076459
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include <clutter/clutter.h>

G_BEGIN_DECLS

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

#define CLUTTER_TYPE_BUTTON                 (clutter_button_get_type ())
#define CLUTTER_BUTTON(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_BUTTON, ClutterButton))
#define CLUTTER_BUTTON_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_BUTTON, ClutterButtonClass))
#define CLUTTER_IS_BUTTON(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_BUTTON))
#define CLUTTER_IS_BUTTON_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_BUTTON))
#define CLUTTER_BUTTON_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_BUTTON, ClutterButtonClass))

typedef struct _ClutterButton        ClutterButton;
typedef struct _ClutterButtonClass   ClutterButtonClass;
typedef struct _ClutterButtonPrivate ClutterButtonPrivate;

//
struct _ClutterButton {
    ClutterActor parent;
    ClutterButtonPrivate *priv;
}; 

//
struct _ClutterButtonClass {
    ClutterActorClass parent_class;
};

//
GType clutter_button_get_type (void) G_GNUC_CONST;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

ClutterActor *clutter_button_new (void);

void clutter_button_get_border_color ( ClutterButton *_button, ClutterColor *_color );
void clutter_button_set_border_color ( ClutterButton *_button, const ClutterColor *_color );

G_END_DECLS

// #################################################################################
#endif // END BUTTON_H_1305076459
// #################################################################################
