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

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __init_error_stack ();
extern void ex_register_builtin_types ();
extern void ex_register_classes ();
extern void ex_uid_init ();
extern const char *exsdk_dev_path;
// ------------------------------------------------------------------ 

// success: 0
// already inited: 1
// failed: -1
int ex_core_init ( const char *_media_path ) {

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "core already inited" );
        return 1;
    }

    // NOTE: we don't allowed already-initialized in these sub-system.
    //       they are strongly initialized by order.

    // before we start, we need to change the rand seed 
    ex_srand((uint)time(0));
    __init_error_stack();

    //
    ex_log ("ex_core initializing...");
    ex_log ("|- init timer...");
    if ( ex_timer_init() != 0 ) {
        ex_log ("fatal error: failed to init timer");
        return -1;
    }
    ex_uid_init (); // init uid variables after timer

    //
    ex_log ("|- init fsys...");
    if ( ex_fsys_init() != 0 ) {
        ex_log ("fatal error: failed to init fsys");
        return -1;
    }
    // NOTE: this must be done before ex_log_init, so that log.txt can be open in the corrent path.
    if ( _media_path ) {
        // set write dir
        if ( ex_fsys_set_write_dir(_media_path) != 0 )
            return -1;
        ex_log("set write dir: %s", _media_path );

        // mount the write dir 
        if ( ex_fsys_mount( _media_path, "/", true ) != 0 )
            return -1;
        ex_log("mount dir: %s", _media_path );
    }

    //
    ex_log ("|- init log system...");
    if ( ex_log_init() != 0 ) {
        ex_log ("fatal error: failed to init log system");
        return -1;
    }

    //
    ex_log ("|- init memory...");
    if ( ex_mem_init() != 0 ) {
        ex_log ("fatal error: failed to init memory");
        return -1;
    }

    //
    ex_log ("|- init string ID table...");
    if ( ex_strid_init(65536) != 0 ) {
        ex_log ("fatal error: failed to init string ID table");
        return -1;
    }

    //
    ex_log ("|- init rtti table...");
    if ( ex_rtti_init() != 0 ) {
        ex_log ("fatal error: failed to init rtti table");
        return -1;
    }

    // after we init rtti, we can register all the classes we used in the app
    ex_log ("|- register builtin types...");
    ex_register_builtin_types ();
    ex_log ("|- register runtime classes...");
    ex_register_classes ();

    //
    ex_log ("|- init lua interpreter...");
    if ( ex_lua_init () != 0 ) {
        ex_log ("fatal error: failed to init lua interpreter");
        return -1;
    }
    // TODO: parse .exrc by lua
#if 0
    ex_lua_load_modules( ex_current_lua_state(), "builtin/" );
#else
    // for development
    ex_fsys_mount( exsdk_dev_path, "/", true );
    {
        char path[256];
        strcpy( path, exsdk_dev_path );
        strcat( path, "builtin/" );
            
        ex_lua_add_path( ex_current_lua_state(), path );
        ex_lua_add_cpath( ex_current_lua_state(), path );
    }

    ex_lua_load_modules( ex_current_lua_state(), "builtin" );
#endif

    //
    ex_log ("ex_core initialized!");
    ex_log ("");
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __deinit_error_stack ();
// ------------------------------------------------------------------ 

void ex_core_deinit () {

    if ( __initialized ) {
        ex_log ("");
        ex_log ("ex_core de-initializing...");

        ex_log ("|- deinit lua interpreter...");
        ex_lua_deinit();

        ex_log ("|- deinit rtti table...");
        ex_rtti_deinit();

        ex_log ("|- deinit string ID table...");
        ex_strid_deinit();

        ex_log ("|- deinit memory...");
        ex_mem_deinit();

        ex_log ("|- deinit log system...");
        ex_log_deinit();

        ex_log ("|- deinit fsys...");
        ex_fsys_deinit();

        ex_log ("|- deinit timer...");
        ex_timer_deinit();

        //
        __deinit_error_stack ();

        //
        ex_log ("ex_core de-initialized!");
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_core_initialized () { return __initialized; }
