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
static char *__media_path = NULL;
static const char *__default_media_path = NULL;

// version
static int __ver_major = 1;
static int __ver_minor = 1;
static int __ver_patch = 0;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __print_help ( FILE *_out ) {
    fprintf ( _out, "exsdk v%d.%d.%d\n", __ver_major, __ver_minor, __ver_patch );
    fprintf ( _out, "Usage:\n" );
    fprintf ( _out, "  exsdk [OPTION...]\n\n" );
    fprintf ( _out, "Help Options:\n" );
    fprintf ( _out, "  -h, --help, -?                            Show this help message\n\n" );
    fprintf ( _out, "Utility Options:\n" );
    fprintf ( _out, "  --project=file path                       The path of the project\n" );
    fprintf ( _out, "  -v, --version                             Print version informations\n" );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __print_version ( FILE *_out ) {
    fprintf ( _out, "exsdk v%d.%d.%d\n", __ver_major, __ver_minor, __ver_patch );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __parse_args ( int *_argc_p, char ***_argv_p ) {
    int argc = *_argc_p;
    char **argv = *_argv_p;
    int i, e;

    // process arguments
    for ( i = 1; i < argc; ++i ) {
        if ( strcmp ( "--project", argv[i] ) == 0 ||
             strncmp ( "--project=", argv[i], 10 ) == 0 ) 
        {
            char *equal = argv[i] + 9;

            if ( *equal == '=' ) {
                int slen = strlen(equal+1);
                __media_path = malloc ( slen + 1 ); // NOTE: we do the free in ex_core_init
                strcpy ( __media_path, equal+1 ); 
            }
            else if ( i + 1 < argc ) {
                __media_path = malloc ( strlen(argv[i+1]) + 1 ); // NOTE: we do the free in ex_core_init
                strcpy ( __media_path, argv[i+1] ); 

                argv[i] = NULL;
                i += 1;
            }
            argv[i] = NULL;
        }
        else if ( strcmp ("-h", argv[i]) == 0 ||
                  strcmp ("-?", argv[i]) == 0 ||
                  strcmp ("--help", argv[i]) == 0 )
        {
            __print_help (stderr);
            argv[i] = NULL;
            exit (0);
        }
        else if ( strcmp ("-v", argv[i]) == 0 ||
                  strcmp ("--version", argv[i]) == 0 )
        {
            __print_version (stderr);
            argv[i] = NULL;
            exit (0);
        }
    }

    // reset the argv and argc
    e = 0;
    for (i = 1; i < argc; i++) {
        if (e) {
            if (argv[i]) {
                argv[e++] = argv[i];
                argv[i] = NULL;
            }
        }
        else if (!argv[i])
            e = i;
    }
    if (e)
        *_argc_p = e;

    // if we still don't find media_path, use default media_path
    if ( !__media_path && __default_media_path ) {
        __media_path = malloc ( strlen(__default_media_path) + 1 ); // NOTE: we do the free in ex_core_init
        strcpy( __media_path, __default_media_path );
    }
}

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
int ex_core_init ( int *_argc_p, char ***_argv_p ) {

    // parse arguments
    __parse_args ( _argc_p, _argv_p );

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
    if ( ex_fsys_init( __media_path ) != 0 ) {
        ex_log ("fatal error: failed to init fsys");
        return -1;
    }
    free(__media_path); // free the unused __media_path. 

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
    // for development
    if ( ex_fsys_mount( exsdk_dev_path, "builtin/", true ) == 0 ) {

        char path[MAX_PATH];
        strcpy( path, exsdk_dev_path );
        strcat( path, "builtin/" );

        ex_log("mount dir: %s", exsdk_dev_path );
        ex_lua_add_path( ex_lua_main_state(), path );
        ex_lua_add_cpath( ex_lua_main_state(), path );
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
    }
#else
    {
        // NOTE: we've already mount the app/builtin/ in ex_fsys_init
        char path[MAX_PATH];
        strcpy( path, ex_fsys_app_dir() );
        strcat( path, "builtin/" );

        ex_lua_add_path( ex_lua_main_state(), path );
        ex_lua_add_cpath( ex_lua_main_state(), path );
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
    }
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_core_set_default_path ( const char *_media_path ) {
    __default_media_path = _media_path;
}
