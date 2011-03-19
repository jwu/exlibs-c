// ======================================================================================
// File         : test_lua.c
// Author       : Wu Jie 
// Last Change  : 02/06/2011 | 22:53:10 PM | Sunday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../lua/lua_inc.h"
#include "../../app/app.h"

///////////////////////////////////////////////////////////////////////////////
// wrap test class
///////////////////////////////////////////////////////////////////////////////

#if 0
typedef struct foobar {
    char *text;
} foobar;
void say_hello ( foobar *_f ) {
    ex_log ( "a foobar object say hello: %s", _f->text );
}

int lua_say_hello ( lua_State *_l ) {
    // TODO:
    // say_hello ( f );
}
int register_foobar ( lua_State *_l ) {
    static const luaL_Reg __funcs[] = {
        { "say_hello", say_hello },
        { NULL, NULL },
    };

    ex_lua_global_module ( _l, "ex" );
    lua_newtable(_l);
    luaL_register(_l, NULL, __funcs);
    lua_setfield(_l,-2,"foobar");
    lua_pop(_l, 1); // pops ex
    return 0;
}
#endif


///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    char path[256];
    struct lua_State *l = ex_current_lua_state();

    // test call from require
    strcpy( path, ex_fsys_realpath("lua/modules") );
    strcat( path, "lua/modules/" );
    ex_log("add path %s", path);
    ex_lua_add_path( l, path );
    ex_lua_add_cpath( l, path );
    ex_lua_dofile( l, "lua/test_require.lua", NULL );

    // test call modules
    ex_lua_load_modules( l, "lua/modules" );
    ex_lua_dofile( l, "lua/test_module.lua", NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void wrap () {
    struct lua_State *l = ex_current_lua_state();

    ex_lua_dofile( l, "lua/test_rapid.lua", NULL );
    ex_lua_dofile( l, "lua/unit_tests/test_class.lua", NULL );
    ex_lua_dofile( l, "lua/unit_tests/test_deepcopy.lua", NULL );
    ex_lua_dofile( l, "lua/unit_tests/test_vec2f.lua", NULL );
    ex_lua_dofile( l, "lua/unit_tests/test_angf.lua", NULL );

    // since object is an runtime class, if we would like to test it, we have to start engine
    ex_app_init();
    ex_lua_dofile( l, "lua/unit_tests/test_object.lua", NULL );
    ex_lua_deinit();
    ex_app_deinit();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_lua () {
    // normal();
    wrap();
}
