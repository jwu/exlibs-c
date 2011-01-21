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

// TEMP: should be in one header file { 
#include "../entity/entity.h"
#include "../entity/component.h"
#include "../entity/trans2d.h"
#include "../entity/debug2d.h"
#include "../entity/camera.h"
// } TEMP end 

///////////////////////////////////////////////////////////////////////////////
// register classes 
///////////////////////////////////////////////////////////////////////////////

void ex_register_classes () {
    // ex core
    EX_REGISTER_CLASS(ex_class_t); // the very base class for any other classes

    // ex engine
    EX_REGISTER_CLASS(ex_entity_t);
    EX_REGISTER_CLASS(ex_component_t);
    EX_REGISTER_CLASS(ex_trans2d_t);
    EX_REGISTER_CLASS(ex_debug2d_t);
    EX_REGISTER_CLASS(ex_camera_t);
}

