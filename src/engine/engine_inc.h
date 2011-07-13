// ======================================================================================
// File         : engine_inc.h
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 13:53:16 PM | Friday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ENGINE_INC_H_1296193998
#define ENGINE_INC_H_1296193998
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

#include "macro.h"
#include "time.h"
#include "ref.h"
#include "object.h"
    #include "object/texture.h"
        #include "object/texture2d.h"

#include "world.h"
    #include "entity.h"
    #include "component/component.h"
        #include "component/trans2d.h"
        #include "component/sprite.h"
        #include "component/camera.h"
        #include "component/debug2d.h"
        #include "component/behavior.h"
            #include "component/lua_behavior.h"

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END ENGINE_INC_H_1296193998
// #################################################################################
