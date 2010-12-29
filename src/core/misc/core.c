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

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_register_classes ();
// ------------------------------------------------------------------ 

bool ex_core_init ()
{
    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "core already inited" );
        return true;
    }

    //
    ex_log ("init memory");
    if ( ex_mem_init() == false ) {
        ex_log ("failed to init memory");
        return false;
    }

    ex_log ("init string ID table");
    if ( ex_strid_init(65536) == false ) {
        ex_log ("failed to init string ID table");
        return false;
    }

    ex_log ("init rtti classes");
    if ( ex_rtti_init() == false ) {
        ex_log ("failed to init rtti table");
        return false;
    }

    // after we init rtti, we can register all the classes we used in the app
    ex_register_classes ();

    // TODO: ex_log ("init lua");

    //
    ex_log ("ex_core inited");
    __initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_core_deinit ()
{
    if ( __initialized ) {
        ex_log ("deinit rtti table");
        ex_rtti_deinit();

        ex_log ("deinit string ID table");
        ex_strid_deinit();

        ex_log ("deinit memory");
        ex_mem_deinit();

        //
        ex_log ("ex_core deinitied");
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_core_initialized () { return __initialized; }
