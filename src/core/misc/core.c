// ======================================================================================
// File         : core.c
// Author       : Wu Jie 
// Last Change  : 07/01/2010 | 07:50:05 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "core.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void core_init ()
{
    ex_log ("init memory");
    mem_init();

    ex_log ("init string ID table");
    sid_init(65536);

    //
    ex_log ("exsdk inited");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void core_deinit ()
{
    ex_log ("deinit string ID table");
    sid_deinit();

    ex_log ("deinit memory");
    mem_deinit();

    //
    ex_log ("exsdk deinitied");
}

