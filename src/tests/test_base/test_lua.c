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

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    char path[256];
    struct lua_State *l = ex_lua_default_state();

    // test call from require
    strcpy( path, ex_fsys_realpath("lua/modules") );
    strcat( path, "lua/modules/" );
    ex_log("add path %s", path);
    ex_lua_add_path( l, path );
    ex_lua_add_cpath( l, path );
    ex_lua_dofile( l, "lua/test_require.lua" );

    // test call modules
    ex_lua_load_modules( l, "lua/modules" );
    ex_lua_dofile( l, "lua/test_module.lua" );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void wrap () {
    struct lua_State *l = ex_lua_default_state();

    ex_lua_dofile( l, "lua/test_rapid.lua" );
    ex_lua_dofile( l, "lua/unit_tests/test_class.lua" );
    ex_lua_dofile( l, "lua/unit_tests/test_deepcopy.lua" );
    ex_lua_dofile( l, "lua/unit_tests/test_vec2f.lua" );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_lua () {
    // normal();
    wrap();
}
