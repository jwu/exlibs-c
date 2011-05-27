// ======================================================================================
// File         : editor.c
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 13:36:40 PM | Friday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void __start_engine_time ();
extern void __init_ref_table ();
// ------------------------------------------------------------------ 

int ex_editor_init () {
    // we can't init ex_editor before ex_core initialized.
    if ( ex_core_initialized () == false ) {
        ex_warning ( "ex_core haven't initialed." );
        return -1;
    }

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "ex_editor already inited" );
        return 1;
    }

    // start editor timer
    __init_ref_table ();
    __start_engine_time ();

    //
    ex_log ("ex_editor inited");
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __deinit_ref_table ();
// ------------------------------------------------------------------ 

void ex_editor_deinit () {
    if ( __initialized ) {
        __deinit_ref_table();

        ex_log ( "ex_editor deinitied" );
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_editor_initialized () { return __initialized; }

