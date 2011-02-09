// ======================================================================================
// File         : lua_global.cpp
// Author       : Wu Jie 
// Last Change  : 08/29/2010 | 17:11:50 PM | Sunday,August
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// quick reference
///////////////////////////////////////////////////////////////////////////////

/**

-------------------------------------------
The index of the stack:
-------------------------------------------

 Top <--- current
        | el(n)     | idx: -1
        | el(n-1)   | idx: -2
        | el(n-2)   | idx: -3
        | ......... | ...
        | ......... | ...
        | ......... | ...
        | el(2)     | idx: 2
 Bottom | el(1)     | idx: 1

 #define lua_pop(L,n) lua_settop(L, -(n) - 1)
 so lua_pop(L,1) means lua_settop(L,-2) which will set current to -2 ( aka pop the -1 ) 

-------------------------------------------
Example:
-------------------------------------------

 int main (void) {
     lua_State *L = luaL_newstate();

                                 // bottome <----------------------------> top
                                 //  1   | 2  | 3    | 4       | 5    | 6   |
                                 // -----+----+------+---------+------+-----|
     lua_pushboolean(L, 1);      // true |    |      |         |      |     |                    
     lua_pushnumber(L, 10);      // true | 10 |      |         |      |     |      
     lua_pushnil(L);             // true | 10 | nil  |         |      |     |         
     lua_pushstring(L, "hello"); // true | 10 | nil  | ’hello’ |      |     |            
     lua_pushvalue(L, -4);       // true | 10 | nil  | ’hello’ | true |     |           
     lua_replace(L, 3);          // true | 10 | true | ’hello’ |      |     |              
     lua_settop(L, 6);           // true | 10 | true | ’hello’ | nil  | nil |           
     lua_remove(L, -3);          // true | 10 | true |  nil    | nil  |     |            
     lua_settop(L, -5);          // true |    |      |         |      |     |          
                                 // -----+----+------+---------+------+-----+
     lua_close(L); 

     return 0;
 }

-------------------------------------------
API examplaint:
-------------------------------------------

Here we list all functions and types from the C API in alphabetical order. 
Each function has an indicator like this:

 [-o, +p, x]

The first field, o, is how many elements the function pops from the stack. 

The second field, p, is how many elements the function pushes onto the stack. 
(Any function always pushes its results after popping its arguments.) 
A field in the form x|y means the function can push (or pop) x or y elements, 
depending on the situation; an interrogation mark '?' means that we cannot know 
how many elements the function pops/pushes by looking only at its arguments 
(e.g., they may depend on what is on the stack). 

The third field, x, tells whether the function may throw errors: 
'-' means the function never throws any error; 
'm' means the function may throw an error only due to not enough memory; 
'e' means the function may throw other kinds of errors; 
'v' means the function may throw an error on purpose

 */

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

#define BUF_SIZE 1024

static lua_State *__L = NULL;
static bool __initialized = false;

///////////////////////////////////////////////////////////////////////////////
// lua core
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern int luaopen_core ( lua_State * );
extern int luaopen_vec2f ( lua_State * );
extern int luaopen_luagl ( lua_State * );
extern int luaopen_luaglu ( lua_State * );
// ------------------------------------------------------------------ 

int ex_lua_init () {
    // if the core already initialized, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "the lua-interpreter already initialized." );
        return 1;
    }
    ex_assert_return( __L == NULL, 1, "the lua status already opened." );

    __L = lua_open();
    luaL_openlibs(__L); // open default libs

    // OPTME { 
    // clear the package.path and package.cpath
    ex_lua_dostring ( __L, "package.path = \"\"" );
    ex_lua_dostring ( __L, "package.cpath = \"\"" );

    // NOTE: we don't need any search path. 
    // in exsdk, require("module") is deprecated all script load as module.
#if 0
    // clear the package.path and package.cpath
    ex_lua_dostring ( __L, "package.path = \"./?.lua\"" );
    ex_lua_dostring ( __L, "package.cpath = \"./?.so;./?.dll\"" );
    {
        char **mounts = ex_fsys_mounts();
        char **i;
        for ( i = mounts; *i != NULL; ++i  ) {
            ex_lua_add_path( __L, *i );
            ex_lua_add_cpath( __L, *i );
        }
        ex_fsys_free_list(mounts);
    }
#endif
    // } OPTME end 

    // we create global ex table if it not exists.
    ex_lua_global_module ( __L, "ex" ); // [-0,+1,-]

    // init ex_core wraps
    luaopen_core (__L); // [-0,+0,-]
    luaopen_vec2f (__L); // [-0,+0,-]

    // init graphics wraps
    lua_settop ( __L, 0 ); // clear the stack
    luaopen_luagl (__L);
    luaopen_luaglu (__L);

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void luaclose_core ();
extern void luaclose_vec2f ();
// ------------------------------------------------------------------ 

void ex_lua_deinit () {
    if ( __initialized ) {
        luaclose_core ();
        luaclose_vec2f ();

        lua_close(__L);
        __L = NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_lua_initialized () { 
    return __initialized;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

lua_State *ex_lua_default_state () { return __L; }

///////////////////////////////////////////////////////////////////////////////
// lua module op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_add_path ( lua_State *_l, const char *_path ) {
    return ex_lua_dostring ( _l, 
                             "package.path = package.path .. \";%s?.lua\"", 
                             _path );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_add_cpath ( lua_State *_l, const char *_path ) {
    return ex_lua_dostring ( _l, 
                             "package.cpath = package.cpath .. \";%s?.so;%s?.dll\"", 
                             _path, _path );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_global_module ( lua_State *_l, const char *_key ) {
    lua_getglobal(_l, _key); // [-0,+1,e]

    // create if necessary.
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // [-1,+0,-] // pop the non-table
        lua_newtable(_l); // [-0,+1,m]

        // cause lua_setglobal will pop the top of the stack, 
        // and we will loose the table after the funciton call.
        // To prevent this, we duplicate the value at the top of the stack.

        lua_pushvalue(_l, -1); // [-0,+1,-]
        lua_setglobal(_l, _key); // [-1,+0,e]
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_module ( lua_State *_l, int _idx, const char *_key ) {
    lua_getfield(_l, _idx, _key); // [-0,+1,e]

    // create if necessary.
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // [-1,+0,-] // pop the non-table
        lua_newtable(_l); // [-0,+1,m]
        lua_pushvalue(_l, -1); // [-0,+1,-] // duplicate the table to leave on top.
        lua_setfield(_l, -2-_idx, _key); // [-1,+0,e] // _key[_idx] = table
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_load_modules ( lua_State *_l, const char *_dir ) {
    char **file_list, **i;
    char full_path[256];
    int base_len, fname_len;

    //
    base_len = strlen(_dir);
    if ( base_len+2 > 256 ) {
        ex_error ( "directory path is too long! %s", _dir );
        return -1;
    }

    // append '/' at the end of the path if not exists.
    strncpy ( full_path, _dir, base_len  );
    if ( full_path[base_len-1] != '/' ) {
        full_path[base_len] = '/';
        full_path[base_len+1] = '\0';
        base_len += 1;
    }
    else {
        full_path[base_len] = '\0';
    }

    //
    file_list = ex_fsys_files_in(_dir);
    for ( i = file_list; *i != NULL; ++i ) {
        fname_len = strlen(*i);
        if ( base_len + fname_len + 1 > 256 ) {
            ex_error ( "file path is too long! %s%s", _dir, *i );
            continue;
        }

        // skip the hidden directory or file
        if ( **i == '.' ) {
            continue;
        }

        // get the full path
        full_path[base_len] = '\0'; // the easist way to reset the full_path to base_path
        strncat ( full_path, *i, fname_len  );
        full_path[base_len+fname_len] = '\0';

        // if this is a directory
        if ( ex_fsys_isdir( full_path ) ) {
            ex_lua_load_modules ( _l, full_path );
        }
        // NOTE: it is possible that its a symbolic link
        else if ( ex_fsys_isfile( full_path ) ) {
            // if this is a file, check if it is a lua file.
            if ( strncmp (*i+fname_len-4, ".lua", 4 ) == 0 ) {
                ex_lua_dofile(_l,full_path);
                ex_log ("load lua module from file %s", full_path);
            }
        }
    }
    ex_fsys_free_list(file_list);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_load_module_byfile ( lua_State *_l, const char *_base_path, const char *_file_name ) {
    char full_path[256];
    char modname[128];
    int modname_len;
    int i, base_len, fname_len;

    //
    base_len = strlen(_base_path);
    fname_len = strlen(_file_name);
    modname_len = fname_len - 4;
    if ( strncmp (_file_name + modname_len, ".lua", 4 ) != 0 ) {
        ex_error ( "failed to load lua module by file %s%s, not a valid lua file", _base_path, _file_name );
        return -1;
    }
    if ( modname_len + 1 > 128 ) {
        ex_error ( "failed to load lua module by file %s, module file name too long!", _file_name );
        return -1;
    }
    if ( base_len + fname_len + 1 > 256 ) {
        ex_error ( "failed to load lua module by file %s%s, file name too long!", _base_path, _file_name );
        return -1;
    }
    strncpy ( modname, _file_name, modname_len );
    modname[modname_len] = '\0';

    strncpy ( full_path, _base_path, base_len );
    strncat ( full_path, _file_name, fname_len );
    full_path[base_len+fname_len] = '\0';

    // substitue '/' by '.'
    for ( i = 0; i < modname_len; ++i ) {
        if ( modname[i] == '/' )
            modname[i] = '.';
    }

    //
    ex_lua_load_module_byfile2 ( _l, full_path, modname );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_load_module_byfile2 ( lua_State *_l, const char *_filepath, const char *_module_name ) {
    ex_array_t splits;

    // if this is a NULL or an empty string, just return NULL reference.
    if ( _module_name == NULL || strcmp( _module_name, "" ) == 0 ) {
        ex_error ( "the module name can't be NULL or empty" );
        return -1;
    }

    // split name list by "/" and recursively search them.
    ex_array_init( &splits, EX_STRID_NULL, sizeof(char *), 8,
                   __ex_array_alloc,
                   __ex_array_realloc,
                   __ex_array_dealloc );
    ex_str_split_into_array( &splits, ".", _module_name );

    //
    ex_lua_global_module ( _l, *((char **)(splits.data)) );
    {
        char *sub_name;
        size_t i = 1;
        while ( i < splits.count ) {
            sub_name = *( (char **) (splits.data) + i );
            ex_lua_module ( _l, -1, sub_name );
            ++i;
        }
    }

    // lua: setmetatable(M, {__index = _G}) { 
    // luaL_newmetatable ( _l, (*splits.last).c_str() );
    lua_newtable (_l);
    lua_getglobal ( _l, "_G" );
    lua_setfield ( _l, -2, "__index" );
    lua_setmetatable ( _l, -2 );
    // } lua end 

    // lua: setfenv(1,M) { 
    lua_pushvalue( _l, -2 ); // push the last table as the environment table.
    lua_setfenv( _l, -2 ); // pop the last value on the stack, and set it as the function environment.
    // } lua end 

    // now load the buffer
    if ( ex_lua_dofile(_l, _filepath ) != 0 ) {
        return -1;
    }

    // after this, we will create a global link for faster access
    lua_setglobal( _l, _module_name );

    // now we can clear stack
    lua_settop( _l, 0 );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_get_module ( lua_State *_l, const char *_moduleName )
{
    // FIXME: this is wrong, should recursively load the module by . { 
    lua_getglobal( _l, _moduleName );
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // Pop the non-table.
        return -1;
    }
    return 0;
    // } FIXME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_get_function ( lua_State *_l, const char *_moduleName, const char *_funcName )
{
    // FIXME: same as above { 
    lua_getglobal( _l, _moduleName );
    lua_getfield( _l, -1, _funcName );
    if ( lua_isnil( _l, -1 ) ) {
        lua_pop(_l, 1); // remove nil
        ex_error ( "can't find function %s in ex_lua_module %s", _funcName, _moduleName );
        return -1;
    }
    lua_remove(_l, -2); // remove mod
    return 0;
    // } FIXME end 
}

///////////////////////////////////////////////////////////////////////////////
// lua interpreter op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_dofile ( lua_State *_l, const char *_filepath ) {
    int status;
    ex_file_t *file;
    size_t buf_size;
    void *buffer;

    // open the file
    file = ex_fopen_r(_filepath);
    if ( file == NULL ) {
        ex_error ( "can't find the file %s", _filepath );
        return -1;
    }

    // get the file to the buffer we allocated.
    buf_size = ex_fsize (file);
    buffer = ex_malloc (buf_size);
    ex_fread (file, buffer, buf_size );
    ex_fclose(file);

    // parse the buffer by lua interpreter
    status = luaL_loadbuffer( _l, (const char *)buffer, buf_size, _filepath );
    if ( status ) {
        ex_lua_alert(_l);
        goto PARSE_FAILED;
    }

    // call the script 
    status = lua_pcall(_l, 0, LUA_MULTRET, 0);
    if ( status ) {
        ex_lua_alert(_l);
        goto PARSE_FAILED;
    }

PARSE_FAILED:
    ex_free(buffer);
    return -1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_dostring ( lua_State *_l, const char *_fmt, ... ) {
    int result = -1;
    int status;
    char buf[BUF_SIZE];
    char *buffer = NULL;

    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _fmt, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';

    // do lua script!
    status = luaL_dostring(_l, buffer);

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );

    // check the result.
    if ( status ) {
        ex_lua_alert(_l);
        return -1;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// lua debug op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_alert ( lua_State *_l ) {
    lua_getglobal(_l, "_ALERT");
    if ( lua_isfunction(_l, -1) ) {
        lua_insert(_l, -2);
        lua_call(_l, 1, 0);
    }
    else {  /* no _ALERT function; print it on stderr */
        ex_error( "%s\n", lua_tostring(_l, -2) );
        lua_pop(_l, 2);  /* remove error message and _ALERT */
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_dump_stack ( lua_State *_l ) {
    int i;
    int top = lua_gettop(_l);
    ex_log("-- stack: top --\n");
    for ( i = top; i >= 0; --i ) { /* repeat for each level */
        int t = lua_type(_l, i);
        switch (t) {
        case LUA_TSTRING: 
            { /* strings */
                ex_log("|- '%s'\n", lua_tostring(_l, i));
                break;
            }
        case LUA_TBOOLEAN: 
            { /* booleans */
                ex_log(lua_toboolean(_l, i) ? "|- true\n" : "|- false\n");
                break;
            }
        case LUA_TNUMBER: 
            { /* numbers */
                ex_log("|- %g\n", lua_tonumber(_l, i));
                break;
            }
        default: 
            { /* other values */
                ex_log("|- %s\n", lua_typename(_l, t));
                break;
            }
        }
    }
    ex_log("-- stack: bottom -- \n");
}
