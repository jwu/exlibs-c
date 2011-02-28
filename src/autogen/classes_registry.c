// ======================================================================================
// File         : classes_registry.c
// Author       : Wu Jie 
// Last Change  : 11/30/2010 | 12:17:41 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../engine/engine_inc.h"

///////////////////////////////////////////////////////////////////////////////
// register classes 
///////////////////////////////////////////////////////////////////////////////

void ex_register_classes () {
    // ex core
    EX_REGISTER_CLASS(ex_class_t); // the very base class for any other classes

        // ex engine
        EX_REGISTER_CLASS_2(ex_object_t,"ex.object");
            EX_REGISTER_CLASS_2(ex_world_t,"ex.world");
                EX_REGISTER_CLASS_2(ex_entity_t,"ex.entity");
                EX_REGISTER_CLASS_2(ex_component_t,"ex.component");
                    EX_REGISTER_CLASS_2( ex_trans2d_t, "ex.trans2d" );
                    EX_REGISTER_CLASS_2( ex_camera_t, "ex.camera" );
                    EX_REGISTER_CLASS_2( ex_behavior_t, "ex.behavior" );
                    EX_REGISTER_CLASS_2( ex_debug2d_t, "ex.debug2d" );
}

