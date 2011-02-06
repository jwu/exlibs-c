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
// core
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern int luaopen_core ( lua_State *_l );
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
    // } OPTME end 

    // we create global ex table if it not exists.
    ex_lua_global_module ( __L, "ex" );

    // init exlibs wraps
    luaopen_core (__L);

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_deinit () {
    if ( __initialized ) {
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

lua_state_t *ex_lua_default_state () { return __L; }

///////////////////////////////////////////////////////////////////////////////
// lua api extend
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_add_path ( lua_state_t *_l, const char *_path ) {
    return ex_lua_dostring ( _l, 
                             "package.path = package.path .. \";%s?.lua\"", 
                             _path );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_add_cpath ( lua_state_t *_l, const char *_path ) {
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

int ex_lua_load_modules ( lua_State *_l, const char *_base_path ) {
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_load_module_byfile ( lua_State *_l, const char *_file_name ) {
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_load_module_byfile2 ( lua_State *_l, const char *_file_name, const char *_module_name ) {
    return -1;

    // TODO:
    // ex_array_t splits;

    // // if this is a NULL or an empty string, just return NULL reference.
    // if ( _module_path == NULL || strcmp( _module_path, "" ) == 0 ) {
    //     ex_error ( "the module name can't be NULL or empty" );
    //     return -1;
    // }

    // // split name list by "/" and recursively search them.
    // ex_array_init( &splits, EX_STRID_NULL, sizeof(char *), 8,
    //                __ex_array_alloc,
    //                __ex_array_realloc,
    //                __ex_array_dealloc );
    // ex_str_split_into_array( &splits, ".", _module );

    // //
    // ex_array_each ( &splits, char *, sub_name ) {
    // } ex_array_each_end











    // Array<string_t> str_list;
    // ex::str::splitIntoArray ( ".", _module, &str_list ); 
    // 
    // string_t rel_path;
    // if ( str_list.size() > 1 ) {
    //     for ( Array<string_t>::iterator iter = str_list.begin(); iter != str_list.end()-1; ++iter ) {
    //         rel_path = rel_path + *iter + '/';
    //     }
    // }
    // rel_path = rel_path + *(str_list.end()-1) + ".lua";
    // const char *rel_path_cstr = rel_path.c_str(); 
    // wchar rel_wpath[1024];
    // ex::mem::zero( rel_wpath, 1024 );
    // char_to_wchar<1024> ( rel_path_cstr, rel_wpath );

    // wpath_t full_path ( lua::base_path() );
    // full_path += rel_wpath;

    // // now load the buffer
    // IFile::smart_ptr_t spFile = futil::file::readonly<PhysicalFile>(full_path);
    // if ( spFile == NULL ) {
    //     ex_error ( "can't find the ex_lua_module %s by file", _module );
    //     return 0;
    // }

    // // now load the file to lua for execute
    // DataHolder dataHolder;
    // dataHolder.alloc ( size_t(spFile->size()) );
    // spFile->read( dataHolder.data(), spFile->size() );

    // //
    // lua::ex_lua_global_module( _l, (*str_list.begin()).c_str() );
    // for ( Array<string_t>::iterator iter = str_list.begin()+1; iter != str_list.end(); ++iter ) {
    //     lua::ex_lua_module( _l, -1, (*iter).c_str() );
    // }

    // // lua: setmetatable(M, {__index = _G}) { 
    // // luaL_newmetatable ( _l, (*str_list.end()).c_str() );
    // lua_newtable (_l);
    // lua_getglobal ( _l, "_G" );
    // lua_setfield ( _l, -2, "__index" );
    // lua_setmetatable ( _l, -2 );
    // // } lua end 

    // // now load the buffer, this will create a function and put it at the top of the stack.
    // int status = luaL_loadbuffer( _l,
    //                               (const char *)(dataHolder.data()), 
    //                               dataHolder.size(), 
    //                               _module );
    // ex_lua_alert(_l, status);

    // // lua: setfenv(1,M) { 
    // lua_pushvalue( _l, -2 ); // push the last table as the environment table.
    // lua_setfenv( _l, -2 ); // pop the last value on the stack, and set it as the function environment.
    // // } lua end 

    // status = lua_pcall(_l, 0, LUA_MULTRET, 0);  /* call main */
    // ex_lua_alert(_l, status);

    // // after this, we will create a global link for faster access
    // lua_setglobal( _l, _module );

    // // now we can clear stack
    // lua_settop( _l, 0 );

    // return 1;
}

#if 0
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int get_module ( lua_State *_l, const char *_moduleName )
{
    lua_getglobal( ex_lua_state(), _moduleName );
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // Pop the non-table.
        return 0;
    }
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int get_function ( lua_State *_l, const char *_moduleName, const char *_funcName )
{
    lua_getglobal( _l, _moduleName );
    lua_getfield( _l, -1, _funcName );
    if ( lua_isnil( _l, -1 ) ) {
        lua_pop(_l, 1); // remove nil
        ex_error ( "can't find function %s in ex_lua_module %s", _funcName, _moduleName );
        return 0;
    }
    lua_remove(_l, -2); // remove mod
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int w__gc ( lua_State *_l ) {
    Proxy *p = (Proxy *)lua_touserdata(_l, 1);
    Object *obj = (Object *)p->user_data;
    if ( p->own_by_gc )
        obj->release();
    return 0;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// interpreter
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_dofile ( lua_state_t *_l, const char *_filepath ) {
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

int ex_lua_dostring ( lua_state_t *_l, const char *_fmt, ... ) {
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
// DEBUG
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
