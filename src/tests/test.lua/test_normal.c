// ======================================================================================
// File         : test_normal.c
// Author       : Wu Jie 
// Last Change  : 07/06/2010 | 16:05:20 PM | Tuesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

#include "../../ext/lua-5.1.4/src/lua.h"
#include "../../ext/lua-5.1.4/src/lauxlib.h"
#include "../../ext/lua-5.1.4/src/lualib.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

const char* media_file = "e:/project/dev/exsdk/res/lua/";

/* Declare the Lua libraries we wish to use. */
/* Note: If you are opening and running a file containing Lua code */
/* using 'lua_dofile(l, "myfile.lua") - you must delcare all the libraries */
/* used in that file here also. */
static const luaL_reg lualibs[] =
{
    { "base",       luaopen_base },
    { NULL,         NULL }
};

/* A function to open up all the Lua libraries you declared above. */
static void openlualibs(lua_State *l)
{
    const luaL_reg *lib;

    for (lib = lualibs; lib->func != NULL; lib++)
    {
        lib->func(l);
        lua_settop(l, 0);
    }
}

void test_normal ()
{
    lua_State *l;
    char path[MAX_PATH];

    /* Declare a Lua State, open the Lua State and load the libraries (see above). */
    l = lua_open();
    openlualibs(l);

    /* You can do what you want here. Note: Remember to update the libraries used (see above) */
    /* if you add to your program and use new Lua libraries. */
    /* In the lines below, I load and run the Lua code contained in the file */
    /* "script.lua". */
    /* Plus print some text directly from C. */
    printf("This line in directly from C\n\n");

    strncpy ( path, media_file, MAX_PATH );
    strcat ( path, "script.lua" );
    if ( luaL_dofile( l, path ) != 0 ) 
        fprintf ( stderr,"%s\n", lua_tostring(l,-1) );

    printf("\nBack to C again\n\n");

    /* Remember to destroy the Lua State */
    lua_close(l);
}

