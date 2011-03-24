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
extern int luaopen_ex ( lua_State * );
extern int luaopen_time ( lua_State * );
extern int luaopen_angf ( lua_State * );
extern int luaopen_vec2f ( lua_State * );
extern int luaopen_mat33f ( lua_State * );

extern int luaopen_luagl ( lua_State * );
extern int luaopen_luaglu ( lua_State * );

extern int luaopen_object ( lua_State * );
extern int luaopen_world ( lua_State * );
extern int luaopen_entity ( lua_State * );
extern int luaopen_component ( lua_State * );
extern int luaopen_trans2d ( lua_State * );
extern int luaopen_behavior ( lua_State * );
extern int luaopen_lua_behavior ( lua_State * );
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

    // init graphics wraps
    lua_settop ( __L, 0 ); // clear the stack
    luaopen_luagl (__L);
    luaopen_luaglu (__L);

    // we create global ex table if it not exists.
    ex_lua_global_module ( __L, "ex" );

    // init core wraps
    luaopen_ex (__L);
        luaopen_time (__L);
        luaopen_angf (__L);
        luaopen_vec2f (__L);
        luaopen_mat33f (__L);

        // init engine wraps
        luaopen_object (__L);
            luaopen_world (__L);
            luaopen_entity (__L);
            luaopen_component (__L);
                luaopen_trans2d (__L);
                luaopen_behavior (__L);
                    luaopen_lua_behavior (__L);

    __initialized = true;

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void luaclose_ex ();
extern void luaclose_time ();
extern void luaclose_angf ();
extern void luaclose_vec2f ();
extern void luaclose_mat33f ();

extern void luaclose_object ();
extern void luaclose_world ();
extern void luaclose_entity ();
extern void luaclose_component ();
extern void luaclose_trans2d ();
extern void luaclose_behavior ();
extern void luaclose_lua_behavior ();
// ------------------------------------------------------------------ 

void ex_lua_deinit () {
    if ( __initialized ) {
        // before close modules, force a complete garbage collection in case of memory leak
        lua_gc(__L, LUA_GCCOLLECT, 0);

        luaclose_ex ();
            luaclose_time ();
            luaclose_angf ();
            luaclose_vec2f ();
            luaclose_mat33f ();

            luaclose_object ();
                luaclose_world ();
                luaclose_entity ();
                luaclose_component ();
                    luaclose_trans2d ();
                    luaclose_behavior ();
                        luaclose_lua_behavior ();

        lua_close(__L);
        __L = NULL;
        __initialized = false;
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

lua_State *ex_lua_main_state () { return __L; }

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
    lua_getglobal(_l, _key);

    // create if necessary.
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // pop the non-table
        lua_newtable(_l);

        // cause lua_setglobal will pop the top of the stack, 
        // and we will loose the table after the funciton call.
        // To prevent this, we duplicate the value at the top of the stack.

        lua_pushvalue(_l, -1);
        lua_setglobal(_l, _key);
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_module ( lua_State *_l, int _idx, const char *_key ) {
    lua_pushstring(_l,_key);
    lua_rawget(_l, _idx);

    // create if necessary.
    if( !lua_istable(_l, -1) ) {
        lua_pop(_l, 1); // pop the non-table
        lua_newtable(_l);
        lua_pushstring(_l,_key);
        lua_pushvalue(_l, -1); // duplicate the table to leave on top.
        // lua_setfield(_l, -2-_idx, _key); // _key[_idx] = table
        lua_rawset(_l, -3-_idx); // _key[_idx] = table
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __load_modules ( lua_State *_l, const char *_base, const char *_dir ) {
    char **file_list, **i;
    char full_path[256];
    int dir_len, fname_len;

    //
    dir_len = strlen(_dir);
    if ( dir_len+2 > 256 ) {
        ex_error ( "directory path is too long! %s", _dir );
        return -1;
    }

    // append '/' at the end of the path if not exists.
    strncpy ( full_path, _dir, dir_len  );
    if ( full_path[dir_len-1] != '/' ) {
        full_path[dir_len] = '/';
        full_path[dir_len+1] = '\0';
        dir_len += 1;
    }
    else {
        full_path[dir_len] = '\0';
    }

    //
    file_list = ex_fsys_files_in(_dir);
    for ( i = file_list; *i != NULL; ++i ) {
        fname_len = strlen(*i);
        if ( dir_len + fname_len + 1 > 256 ) {
            ex_error ( "file path is too long! %s%s", _dir, *i );
            continue;
        }

        // skip the hidden directory or file
        if ( **i == '.' ) {
            continue;
        }

        // ignore file name or directory name start with '__', 
        // this can help me temporary disable loading specific modules.
        if ( strncmp(*i,"__",2) == 0 ) {
            // ex_log("skip loading %s%s", _dir, *i); // for DEBUG
            continue;
        }

        // get the full path
        full_path[dir_len] = '\0'; // the easist way to reset the full_path to base_path
        strncat ( full_path, *i, fname_len  );
        full_path[dir_len+fname_len] = '\0';

        // if this is a directory
        if ( ex_fsys_isdir( full_path ) ) {
            __load_modules ( _l, _base, full_path );
        }
        // NOTE: it is possible that its a symbolic link
        else if ( ex_fsys_isfile( full_path ) ) {
            // if this is a file, check if it is a lua file.
            if ( strncmp (*i+fname_len-4, ".lua", 4 ) == 0 ) {
                char modname[256];
                int base_len = strlen(_base);
                int modname_len = dir_len+fname_len-4-base_len;
                char *j = modname;

                strncpy(modname,full_path+base_len,modname_len);
                modname[modname_len] = '\0';
                while ( *j != '\0' ) {
                    if ( *j == '/' )
                        *j = '.';
                    ++j;
                }

                // check if the module already loaded ( by require )
                int loaded = lua_gettop(_l) + 1;  /* index of _LOADED table */
                lua_getfield(_l, LUA_REGISTRYINDEX, "_LOADED");
                lua_getfield(_l, loaded, modname);  /* get _LOADED[modname] */
                if ( !lua_istable(_l,-1) ) 
                    ex_lua_dofile( _l, full_path, modname );
                // TODO: since require will change the order of module loading, 
                // I think I need a correct way report module loading 
                ex_log ("load lua module %s from file %s", modname, full_path);
            }
        }
    }
    ex_fsys_free_list(file_list);
    return 0;
}

int ex_lua_load_modules ( lua_State *_l, const char *_dir ) {
    char dir[256];
    int dir_len;

    // NOTE: this will ensure that our dir always end with "/" { 
    dir_len = strlen(_dir);
    if ( dir_len+2 > 256 ) {
        ex_error ( "directory path is too long! %s", _dir );
        return -1;
    }

    // append '/' at the end of the path if not exists.
    strncpy ( dir, _dir, dir_len  );
    if ( dir[dir_len-1] != '/' ) {
        dir[dir_len] = '/';
        dir[dir_len+1] = '\0';
        dir_len += 1;
    }
    else {
        dir[dir_len] = '\0';
    }
    // } NOTE end 

    return __load_modules ( _l, dir, dir );
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
    if ( ex_lua_dofile(_l, _filepath, _module_name ) != 0 ) {
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

int ex_lua_dofile ( lua_State *_l, const char *_filepath, const char *_modname ) {
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
    if ( _modname ) {
        lua_pushstring(_l,_modname);
        status = lua_pcall(_l, 1, LUA_MULTRET, 0);
    }
    else {
        status = lua_pcall(_l, 0, LUA_MULTRET, 0);
    }
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_clear_refs ( lua_State *_l ) {
    // get global table "_G"
    lua_getglobal(_l, "_G");

    // first key
    lua_pushnil(_l);
    while ( lua_next(_l, -2) != 0 ) {

        // TODO: should recursively check the table....

        if ( ex_lua_isclass( _l, lua_gettop(_l) ) ) {
            lua_pushvalue(_l,-3); // push table
            lua_pushvalue(_l,-3); // push key
            lua_pushnil(_l); // push key
            lua_settable(_l,-3);
            lua_pop(_l,1); // pops table
        }

        // removes 'value'; keeps 'key' for next iteration
        lua_pop(_l, 1);
    }
    lua_gc(__L, LUA_GCCOLLECT, 0);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// lua module functions in c
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_tostring ( lua_State *_l, int _idx ) {
    luaL_checkany( _l, _idx );
    if ( luaL_callmeta(_l, _idx, "__tostring") )  /* is there a metafield? */
        return 1;  /* use its value */
    switch ( lua_type(_l, _idx) ) {
    case LUA_TNUMBER:
        lua_pushstring(_l, lua_tostring(_l, _idx));
        break;
    case LUA_TSTRING:
        lua_pushvalue(_l, _idx);
        break;
    case LUA_TBOOLEAN:
        lua_pushstring(_l, (lua_toboolean(_l, _idx) ? "true" : "false"));
        break;
    case LUA_TNIL:
        lua_pushliteral(_l, "nil");
        break;
    default:
        lua_pushfstring(_l, "%s: %p", luaL_typename(_l, _idx), lua_topointer(_l, _idx));
        break;
    }
    return 1;
}


// ------------------------------------------------------------------ 
// Desc: 
// function isclass (_object)
//     local tp = typeof(_object)
//     if type(tp) ~= "table" then 
//         return false
//     end
//     local mt = getmetatable(tp)
//     if mt == meta_class then 
//         return true 
//     end
//     return false
// end
// ------------------------------------------------------------------ 

bool ex_lua_isclass ( lua_State *_l, int _idx ) {
    bool r;

    // local tp = typeof(_object)
    if ( lua_getmetatable(_l,_idx) == 0 )
        return false;

    // if type(tp) ~= "table" then return false end
    if ( lua_istable( _l, -1 ) == 0 ) {
        lua_pop(_l, 1); // pops tp
        return false;
    }

    // local r = rawget(tp, "__isclass")
    lua_pushstring(_l,"__isclass");
    lua_rawget ( _l, -2 );

    // return r
    r = lua_toboolean(_l,-1);
    lua_pop(_l,2); // pops tp, r
    return r;
}

// ------------------------------------------------------------------ 
// Desc: 
// function isbuiltin (_object)
//     local tp = typeof(_object)
//     if type(tp) ~= "table" then 
//         return false 
//     end 
//     local r = rawget(tp, "__isbuiltin")
//     if r == nil then 
//         return false 
//     end
//     return r
// end
// ------------------------------------------------------------------ 

bool ex_lua_isbuiltin ( lua_State *_l, int _idx ) {
    bool r;

    // local tp = typeof(_object)
    if ( lua_getmetatable(_l,_idx) == 0 )
        return false;

    // if type(tp) ~= "table" then return false end
    if ( lua_istable( _l, -1 ) == 0 ) {
        lua_pop(_l,1); // pops tp
        return false;
    }

    // local r = rawget(tp, "__isbuiltin")
    lua_pushstring(_l,"__isbuiltin");
    lua_rawget ( _l, -2 );

    // return r
    r = lua_toboolean(_l,-1);
    lua_pop(_l,2); // pops tp, r
    return r;
}

// ------------------------------------------------------------------ 
// Desc: 
// function typename(_object)
//     local tp = type(_object) 
//     if tp == "userdata" or tp == "table" then 
//         local name = rawget(typeof(_object), "__typename")
//         assert ( name ~= nil, "can't find __typename define in your class." )
//         return name
//     end
//     return tp
// end
// ------------------------------------------------------------------ 

int ex_lua_typename ( lua_State *_l, int _idx ) {
    // local tp = type(_object) 
    // if tp == "userdata" or tp == "table" then 
    if ( lua_isuserdata(_l,_idx) || lua_istable(_l,_idx) ) {
        // local name = rawget(typeof(_object), "__typename")
        if ( lua_getmetatable(_l,_idx) == 0 ) {
            return luaL_error (_l,"can't find metatable in the class.");
        }
        lua_pushstring(_l,"__typename");
        lua_rawget(_l,-2);

        // assert ( name ~= nil, "can't find __typename define in your class." )
        if ( lua_isnil(_l,-1) ) {
            return luaL_error (_l,"can't find __tpename define in the class.");
        }

        // return name
        lua_remove(_l, -2); // remove mt
        return 1;
    }

    // return type(_object)
    lua_pushstring(_l, luaL_typename(_l,_idx) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function deepcopy (_obj)
//     local lookup_table = {}
//     local function _copy(_obj)
//         if isbuiltin( typeof(_obj) ) then
//             assert(_obj.copy, "please provide copy function for builtin type: " .. typename(_obj) )
//             return _obj:copy()
//         elseif type(_obj) ~= "table" then
//             return _obj
//         elseif lookup_table[_obj] then
//             return lookup_table[_obj]
//         end
//         local new_table = {}
//         lookup_table[_obj] = new_table
//         for index, value in pairs(_obj) do
//             new_table[_copy(index)] = _copy(value)
//         end
//         return setmetatable(new_table, getmetatable(_obj))
//     end
//     return _copy(_obj)
// end
// ------------------------------------------------------------------ 

#define OBJ_IDX 1
static int __copy ( lua_State *_l ) {
    // if isbuiltin( _obj ) then
    if ( ex_lua_isbuiltin(_l, OBJ_IDX) ) {
        lua_pop(_l,1); // pops mt

        // assert(_obj.copy, "please provide copy function for builtin type: " .. typename(_obj) )
        lua_getfield(_l,OBJ_IDX,"copy");
        if ( lua_isnil(_l,-1) ) {
            ex_lua_typename(_l,OBJ_IDX);
            return luaL_error ( _l, "please provide copy function for builtin type: %s", lua_tostring(_l,-1) );
            lua_pop(_l,2); // pops typename, nil 
            lua_pushnil(_l);
            return 1;
        }
        // return _obj:copy()
        lua_pushvalue(_l,OBJ_IDX);
        lua_call(_l,1,1);
        return 1;
    }

    // elseif type(_obj) ~= "table" then
    if ( lua_istable(_l,OBJ_IDX) == 0 ) {
        // return _obj
        lua_pushvalue(_l,OBJ_IDX);
        return 1;
    }

    // elseif lookup_table[_obj] then
    lua_pushvalue(_l,OBJ_IDX);
    lua_gettable(_l,lua_upvalueindex(1));
    if ( lua_isnil(_l,-1) == 0 ) {
        // return lookup_table[_obj]
        return 1;
    }
    lua_pop(_l,1); // pops lookup_table[_obj] 

    // local new_table = {}
    lua_newtable(_l);
    // lookup_table[_obj] = new_table
    lua_pushvalue(_l,OBJ_IDX); // k
    lua_pushvalue(_l,-2); // v
    lua_settable(_l,lua_upvalueindex(1));
    // for key, value in pairs(_obj) do
    //     new_table[_copy(key)] = _copy(value)
    // end
    lua_pushnil(_l); /* first key */
    while ( lua_next(_l,OBJ_IDX) != 0 ) {
        /* uses 'key' (at index -2) and 'value' (at index -1) */

        // top
        // v
        // k
        // DEBUG { 
        // ex_log ( "should be:\nv\nk" );
        // ex_lua_dump_stack(_l);
        // ex_log ( "" );
        // } DEBUG end 

        // ======================================================== 
        // _copy(key)
        // ======================================================== 

        lua_pushvalue(_l,lua_upvalueindex(1));
        lua_pushcclosure(_l,__copy,1);
        // top
        // f
        // v
        // k
        // DEBUG { 
        // ex_log ( "should be:\nf\nv\nk" );
        // ex_lua_dump_stack(_l);
        // ex_log ( "" );
        // } DEBUG end 

        lua_pushvalue(_l,-3); // key
        lua_call(_l,1,1);

        // ======================================================== 
        // _copy(value)
        // ======================================================== 

        lua_pushvalue(_l,lua_upvalueindex(1));
        lua_pushcclosure(_l,__copy,1);
        // top
        // f
        // r (result)
        // v
        // k
        // DEBUG { 
        // ex_log ( "should be:\nf\nr\nv\nk" );
        // ex_lua_dump_stack(_l);
        // ex_log ( "" );
        // } DEBUG end 

        lua_pushvalue(_l,-3); // value
        lua_call(_l,1,1);

        // ======================================================== 
        // new_table[_copy(key)] = _copy(value)
        // ======================================================== 

        // top
        // r (result)
        // r (result)
        // v
        // k
        // DEBUG { 
        // ex_log ( "should be:\nr\nr\nv\nk" );
        // ex_lua_dump_stack(_l);
        // ex_log ( "" );
        // } DEBUG end 
        lua_settable(_l,-5);

        // DEBUG { 
        // ex_log ( "done!" );
        // ex_lua_dump_stack(_l);
        // ex_log ( "" );
        // } DEBUG end 
        /* removes 'value'; keeps 'key' for next iteration */
        lua_pop(_l, 1);
    }

    // return setmetatable(new_table, getmetatable(_obj))
    if ( lua_getmetatable(_l,OBJ_IDX) ) {
        lua_setmetatable(_l,-1);
    }

    // return new_table
    return 1;
}
#undef OBJ_IDX

int ex_lua_deepcopy ( lua_State *_l, int _idx ) {
    // local lookup_table = {}
    lua_newtable(_l);

    // def local functon _copy, and associate it with lookup table
    lua_pushcclosure(_l,__copy,1);

    // return _copy(_obj)
    lua_pushvalue(_l,_idx); // push first args to stack
    lua_call(_l,1,1); // call _copy with 1 args, 1 result.
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_type_meta_newindex ( lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }
    key = luaL_checkstring(_l, 2);
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    // if this is not nil
    if ( lua_isnil( _l, -1 ) == 0 ) {
        return luaL_error(_l,"the key %s is readonly", key);
    }

    // get and check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset == NULL ) {
        return luaL_error(_l,"key %s not found", key);
    }
    if ( getset->set == NULL ) {
        return luaL_error(_l,"the key %s is readonly", key);
    }

    // NOTE: we don't go for super, 
    //       cause we belive people who use type_meta already known where the member belongs to.

    // process
    return getset->set(_l);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_type_meta_index ( lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }
    lua_pushvalue ( _l, 2 );
    lua_rawget ( _l, -2 );
    if ( lua_isnil( _l, -1 ) == 0 ) // if this is not nil
        return 1;
    lua_pop(_l,2);

    // check the key
    key = luaL_checkstring(_l, 2);
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset == NULL ) {
        return luaL_error(_l, "key %s not found", key);
    }
    if ( getset->get == NULL ) {
        return luaL_error(_l, "the key %s is writeonly", key);
    }

    // NOTE: we don't go for super, 
    //       cause we belive people who use type_meta already known where the member belongs to.

    // process
    return getset->get(_l);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_meta_newindex ( struct lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }
    key = luaL_checkstring(_l, 2);
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    // if this is not nil
    if ( lua_isnil( _l, -1 ) == 0 ) {
        return luaL_error(_l,"the key %s is readonly", key);
    }
    lua_pop(_l,1); // pops v

    // get and check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset ) {
        if ( getset->set == NULL ) {
            return luaL_error(_l,"the key %s is readonly", key);
        }
        // process
        return getset->set(_l);
    }
    
    // process super

    // check if the super have the key
    // local super = rawget(mt,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,-2);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // get key from super's metatable
        // v = rawget(super,_k)
        lua_pushvalue(_l,2); // push key
        lua_rawget(_l,-2);
        // if v ~= nil then 
        if ( lua_isnil(_l,-1) == 0 ) {
            // rawset(_t,_k,_v)
            lua_pushvalue(_l,2); // push _k
            lua_pushvalue(_l,3); // push _v
            lua_rawset (_l,1);
            return 0;
        }
        lua_pop(_l,1); // pops v

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget

    return luaL_error ( _l, "can't find the key %s", lua_tostring(_l,2) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_meta_index ( struct lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }
    lua_pushvalue ( _l, 2 );
    lua_rawget ( _l, -2 );
    if ( lua_isnil( _l, -1 ) == 0 ) // if this is not nil
        return 1;
    lua_pop(_l,1); // pops v

    // check the key
    key = luaL_checkstring(_l, 2);
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset ) {
        if ( getset->get == NULL ) {
            return luaL_error(_l, "the key %s is writeonly", key);
        }
        // process
        return getset->get(_l);
    }

    // process in super

    // check if the super have the key
    // local super = rawget(mt,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,-2);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // get key from super's metatable
        // v = rawget(super,_k)
        lua_pushvalue(_l,2); // push key
        lua_rawget(_l,-2);
        // if v ~= nil then 
        if ( lua_isnil(_l,-1) == 0 ) {
            return 1;
        }
        lua_pop(_l,1); // pops v

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget

    // return nil
    lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: NOTE: this function only used in __class_index 
// if v ~= nil then 
//     local vv = v
//     if type(vv) == "table" and getmetatable(vv) == nil then
//         vv = deepcopy(v)
//     end
//     rawset(mt,_k,vv)
//     return vv
// end
// ------------------------------------------------------------------ 

static inline int __copy_v ( lua_State *_l, int _idx ) {
    // if type(v) == "table" and getmetatable(v) == nil then
    if ( lua_istable(_l,-1) ) {
        if ( lua_getmetatable(_l,-1) == 0 ) {
            // vv = deepcopy(v)
            ex_lua_deepcopy(_l, lua_gettop(_l) );
            lua_remove(_l,-2); // remove v
        }
        else {
            // local vv = v
            lua_pop(_l,1); // pops mt
        }
    }
    // rawset(mt,_k,vv)
    lua_pushvalue(_l,2); // push key
    lua_pushvalue(_l,-2); // push vv
    lua_rawset ( _l, _idx );

    // return vv
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_child_meta_newindex ( lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // ======================================================== 
    // process in meta 
    // ======================================================== 

    // DISABLE: the __readonly table should never come to here I guess { 
    // // make sure only get __readonly in table _t, not invoke __index method.
    // // local is_readonly = rawget(_t,"__readonly")
    // lua_pushstring(_l,"__readonly");
    // lua_rawget(_l,1);
    // // if is_readonly then -- this equals to (is_readonly ~= nil and is_readonly == true)
    // if ( lua_isboolean(_l,-1) && lua_toboolean(_l,-1) ) {
    //     return luaL_error (_l,"the table is readonly");
    // }
    // lua_pop(_l,1); // pop is_readonly
    // } DISABLE end 

    // assume the userdata must have the metatable
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    lua_pushvalue(_l,2);
    lua_rawget ( _l, -2 );
    // if this is not nil
    if ( lua_isnil( _l, -1 ) == 0 ) {
        lua_pop(_l,1); // pops rawget
        lua_pushvalue(_l,2);
        lua_pushvalue ( _l, 3 );
        lua_rawset ( _l, -3 );
        return 0;
    }
    lua_pop(_l,2);

    // get and check the key
    key = luaL_checkstring(_l, 2);
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset ) {
        if ( getset->set == NULL ) {
            return luaL_error(_l, "the key %s is readonly", key);
        }
        // process
        return getset->set(_l);
    }

    // ======================================================== 
    // process in meta-meta 
    // ======================================================== 

    // check if the metatable have the key
    // local mt = getmetatable(mt) 
    lua_getmetatable( _l, 1 );
    if ( lua_getmetatable(_l,-1) == 0 ) {
        return luaL_error ( _l, "can't find the meta-meta-table of _t" );
    }
    // v = rawget(mt,_k)
    lua_pushvalue(_l,2);
    lua_rawget(_l,-2);

    // if v ~= nil then 
    if ( lua_isnil(_l,-1) == 0 ) {
        lua_pop(_l,1); // pops v
        lua_pushvalue(_l,2);
        lua_pushvalue ( _l, 3 );
        lua_rawset (_l,-4);
        return 0;
    }
    lua_pop(_l,1); // pops v

    //
    // check if the super have the key
    // local super = rawget(mt,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,-2);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // get key from super's metatable
        // v = rawget(super,_k)
        lua_pushvalue(_l,2); // push key
        lua_rawget(_l,-2);
        // if v ~= nil then 
        if ( lua_isnil(_l,-1) == 0 ) {
            // rawset(_t,_k,_v)
            lua_pop(_l,1); // pops v
            lua_pushvalue(_l,2); // push _k
            lua_pushvalue(_l,3); // push _v
            lua_rawset (_l,-5);
            return 0;
        }
        lua_pop(_l,1); // pops v

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget

    return luaL_error ( _l, "can't find the key %s", lua_tostring(_l,2) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_child_meta_index ( lua_State *_l, ex_hashmap_t *_key_to_getset ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // ======================================================== 
    // process in meta
    // ======================================================== 

    // assume the userdata must have the metatable
    // local mt = getmetatable(_t) 
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        return luaL_error ( _l, "fatal error: can't find the metatable!" );
    }

    // new an userdata, setup and return it.
    // if _k == "super" then
    key = luaL_checkstring(_l, 2);
    if ( strcmp( key, "super" ) == 0 ) {
        if ( lua_getmetatable(_l,-1) == 0 ) {
            return luaL_error ( _l, "can't find the meta-meta-table of _t" );
        }

        // new_table = { __readonly = true }
        lua_newtable(_l);
        lua_pushstring(_l,"__readonly");
        lua_pushboolean(_l,true);
        lua_rawset(_l,-3);

        // rawget(mt,"__super")
        lua_pushstring(_l,"__super");
        // top <--
        // "__super"
        // new_table
        // mt
        lua_rawget(_l,-3);

        // top <--
        // rawget(mt,"__super")
        // new_table
        // mt
        // return setmetatable( { __readonly = true }, rawget(mt,"__super") )
        lua_setmetatable(_l, -2 );
        return 1;
    }

    // first use rawget check if we have value in metatable.
    lua_pushvalue ( _l, 2 );
    lua_rawget ( _l, -2 );
    if ( lua_isnil( _l, -1 ) == 0 ) // if this is not nil
        return 1;
    lua_pop(_l,2);

    // check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( _key_to_getset, &key_id, NULL );
    if ( getset ) {
        if ( getset->get == NULL ) {
            return luaL_error(_l, "the key %s is writeonly", key);
        }

        // process
        return getset->get(_l);
    }

    // ======================================================== 
    // process in meta-meta
    // ======================================================== 

    // check if the metatable have the key
    // local mt = getmetatable(mt) 
    lua_getmetatable( _l, 1 );
    if ( lua_getmetatable(_l,-1) == 0 ) {
        return luaL_error ( _l, "can't find the meta-meta-table of _t" );
    }
    // v = rawget(mt,_k)
    lua_pushvalue(_l,2);
    lua_rawget(_l,-2);

    // if v ~= nil then 
    if ( lua_isnil(_l,-1) == 0 ) {
        return __copy_v(_l,lua_gettop(_l)-2);
    }
    lua_pop(_l,1); // pops v

    // check if the super have the key
    // local super = rawget(mt,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,-2);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // get key from super's metatable
        // v = rawget(super,_k)
        lua_pushvalue(_l,2); // push key
        lua_rawget(_l,-2);
        // if v ~= nil then 
        if ( lua_isnil(_l,-1) == 0 ) {
            return __copy_v(_l,lua_gettop(_l)-3);
        }
        lua_pop(_l,1); // pops v

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget

    // return nil
    lua_pushnil(_l);
    return 1;
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
        // luaL_error( _l, "%s\n", lua_tostring(_l, -2) ); // don't do this in unprotected scene
        ex_warning( "%s\n", lua_tostring(_l, -2) );
        lua_pop(_l, 2);  /* remove error message and _ALERT */
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_dump_stack ( lua_State *_l ) {
    int i;
    int top = lua_gettop(_l);
    ex_log("dump lua stack:");
    ex_log("+_________");
    ex_log("|-> TOP");
    for ( i = top; i >= 0; --i ) {
        int t = lua_type(_l, i);
        switch (t) {
        case LUA_TSTRING: 
            ex_log("| %d: '%s'", i, lua_tostring(_l, i));
            break;

        case LUA_TBOOLEAN: 
            ex_log( "| %d: %s", i, lua_toboolean(_l, i) ? "true" : "false");
            break;

        case LUA_TNUMBER: 
            ex_log("| %d: %g", i, lua_tonumber(_l, i));
            break;

            // other type ( userdata, function, table ... )
        default: 
            ex_log("| %d: %s", i, lua_typename(_l, t));
            break;
        }
    }
    ex_log("|__________");
}
