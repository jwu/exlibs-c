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

static bool _initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool core_init ()
{
    // if the core already inited, don't init it second times.
    if ( _initialized ) {
        ex_warning ( "core already inited" );
        return true;
    }

    //
    ex_log ("init memory");
    if ( mem_init() == false ) {
        ex_log ("failed to init memory");
        return false;
    }

    ex_log ("init string ID table");
    if ( str_id_init(65536) == false ) {
        ex_log ("failed to init string ID table");
        return false;
    }

    // TODO: ex_log ("init lua");

    //
    ex_log ("exsdk inited");
    _initialized = true;

    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void core_deinit ()
{
    if ( _initialized ) {
        ex_log ("deinit string ID table");
        str_id_deinit();

        ex_log ("deinit memory");
        mem_deinit();

        //
        ex_log ("exsdk deinitied");
        _initialized = false;
    }
}

