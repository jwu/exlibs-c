// ======================================================================================
// File         : app.c
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 13:35:03 PM | Friday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "app.h"
#include "../engine/engine_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void __start_engine_time ();
// ------------------------------------------------------------------ 

int ex_app_init () {
    // we can't init ex_engine before ex_core initialized.
    if ( ex_core_initialized () == false ) {
        ex_warning ( "ex_core haven't initialed." );
        return -1;
    }

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "ex_engine already inited" );
        return 1;
    }

    // start engine time
    __start_engine_time ();

    //
    ex_log ("ex_engine inited");
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_app_deinit () {
    if ( __initialized ) {
        ex_log ( "ex_engine deinitied" );
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_app_initialized () { return __initialized; }

