// ======================================================================================
// File         : simple_behavior.h
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 14:50:04 PM | Thursday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef SIMPLE_BEHAVIOR_H_1296111006
#define SIMPLE_BEHAVIOR_H_1296111006
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../engine/engine_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define ROT_AROUND_SELF 0
#define ROT_AROUND_WORLD 1
#define ROT_AROUND_PARENT 2

EX_DECL_CLASS_SUPER_BEGIN(ex_simple_t,ex_behavior_t)
    ex_vec2f_t move_dir;
    float move_speed;
    float rot_speed;
    int rot_around;
EX_DECL_CLASS_SUPER_END(ex_simple_t,ex_behavior_t)

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END SIMPLE_BEHAVIOR_H_1296111006
// #################################################################################

