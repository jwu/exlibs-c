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
// for developing
///////////////////////////////////////////////////////////////////////////////

#if (EX_PLATFORM == EX_WIN32)
    const char *exsdk_dev_path = "e:/project/dev/exsdk/";
#else
    const char *exsdk_dev_path = "/Users/Johnny/dev/projects/exdev/exsdk/";
#endif

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_register_builtin_types ();
extern void ex_register_classes ();
extern void ex_uid_init ();
// ------------------------------------------------------------------ 

// success: 0
// already inited: 1
// failed: -1
int ex_core_init ()
{
    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "core already inited" );
        return 1;
    }

    // NOTE: we don't allowed already-initialized in these sub-system.
    //       they are strongly initialized by order.

    // before we start, we need to change the rand seed 
    ex_srand(time(0));

    //
    ex_log ("init timer");
    if ( ex_timer_init() != 0 ) {
        ex_log ("failed to init timer");
        return -1;
    }
    ex_uid_init (); // init uid variables after timer

    //
    ex_log ("init fsys");
    if ( ex_fsys_init() != 0 ) {
        ex_log ("failed to init fsys");
        return -1;
    }

    //
    ex_log ("init memory");
    if ( ex_mem_init() != 0 ) {
        ex_log ("failed to init memory");
        return -1;
    }

    ex_log ("init string ID table");
    if ( ex_strid_init(65536) != 0 ) {
        ex_log ("failed to init string ID table");
        return -1;
    }

    ex_log ("init rtti classes");
    if ( ex_rtti_init() != 0 ) {
        ex_log ("failed to init rtti table");
        return -1;
    }

    // after we init rtti, we can register all the classes we used in the app
    ex_register_builtin_types ();
    ex_register_classes ();

    // TODO: ex_log ("init lua");

    //
    ex_log ("ex_core inited");
    __initialized = true;
    return 0;
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

        ex_log ("deinit fsys");
        ex_fsys_init();

        ex_log ("deinit timer");
        ex_timer_deinit();

        //
        ex_log ("ex_core deinitied");
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_core_initialized () { return __initialized; }
