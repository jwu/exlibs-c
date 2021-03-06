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
#include "editor_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void __init_window_table();
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

    // init asset database
    ex_log ("init asset database...");
    if ( ex_asset_db_init() != 0 ) {
        ex_log ("fatal error: failed to init asset database");
        return -1;
    }

    // init windows array
    __init_window_table();

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
extern void __deinit_window_table ();
// ------------------------------------------------------------------ 

void ex_editor_deinit () {
    if ( __initialized ) {
        //
        ex_asset_db_deinit ();
        ex_object_gc();
        ex_lua_deinit();

        //
        __deinit_ref_table();
        __deinit_window_table();

        //
        ex_log ( "ex_editor deinitied" );
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_editor_initialized () { return __initialized; }

