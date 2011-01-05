// ======================================================================================
// File         : engine.c
// Author       : Wu Jie 
// Last Change  : 12/27/2010 | 09:56:51 AM | Monday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine.h"
#include "trans2d.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_engine_init () {
    // we can't init ex_engine before ex_core initialized.
    if ( ex_core_initialized () == false ) {
        ex_warning ( "ex_core haven't initialed." );
        return false;
    }

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "ex_engine already inited" );
        return true;
    }

    // 
    ex_trans2d_internal_init ();

    //
    ex_log ("ex_engine inited");
    __initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_engine_deinit () {
    if ( __initialized ) {
        ex_trans2d_internal_deinit();

        ex_log ( "ex_engine deinitied" );
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_engine_initialized () { return __initialized; }
