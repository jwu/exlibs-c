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
        EX_REGISTER_CLASS(ex_object_t);
            EX_REGISTER_CLASS(ex_world_t);
                EX_REGISTER_CLASS(ex_entity_t);
                EX_REGISTER_CLASS(ex_component_t);
                    EX_REGISTER_CLASS(ex_trans2d_t);
                    EX_REGISTER_CLASS(ex_camera_t);
                    EX_REGISTER_CLASS(ex_behavior_t);
                    EX_REGISTER_CLASS(ex_debug2d_t);
}

