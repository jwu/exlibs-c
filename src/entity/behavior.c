// ======================================================================================
// File         : behavior.c
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 11:49:24 AM | Thursday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "behavior.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_behavior_t)
    // ex_component_t
    NULL, // owner
    true, // active
    NULL, // init
    NULL, // deinit

    // ex_behavior_t
    EX_BEHAVIOR_STATE_NEW, // state

    NULL, // level_start
    NULL, // start
    NULL, // update
    NULL, // post_update
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_behavior_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_behavior_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_behavior_t)
EX_DEF_TOSTRING_END
