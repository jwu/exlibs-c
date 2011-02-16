// ======================================================================================
// File         : wrap_core.c
// Author       : Wu Jie 
// Last Change  : 11/15/2010 | 13:36:19 PM | Monday,November
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
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// local function class_new ( _self, ... )
//     local table = ...
//     return setmetatable( table or {}, _self )
// end
// ------------------------------------------------------------------ 

static int __class_new ( lua_State *_l ) {
    lua_pushvalue(_l,1); // [-0,+1,-]
    if ( lua_isnil(_l,2) != 0 ) // if 2nd args is nil
        lua_newtable(_l); // [-0,+1,m]
    else
        lua_pushvalue(_l,2); // [-0,+1,-]
    lua_setmetatable(_l,-2); // [-1,+0,-] 
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __range_rand ( lua_State *_l ) {
    float low = luaL_checknumber(_l,1); 
    float high = luaL_checknumber(_l,2); 

    lua_pushnumber(_l, ex_range_randf(low,high) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __log ( lua_State *_l ) {
    const char *msg = luaL_checkstring ( _l, 1 );
    ex_log ( "%s", msg );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// typeof = getmetatable
// ------------------------------------------------------------------ 

static inline int __type_of ( lua_State *_l ) {
    if ( lua_getmetatable(_l,1) == 0 )
        lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function isclass (_class)
//     if type(_class) ~= "table" then 
//         return false
//     end
//     local mt = getmetatable(_class)
//     if mt == _R["ex.class.meta"] then return true end
//     return false
// end
// ------------------------------------------------------------------ 

bool ex_lua_isclass ( lua_State *_l, int _idx ) {
    // if type(_class) ~= "table" then return false end
    if ( lua_istable( _l, _idx ) == 0 ) {
        return false;
    }

    // local mt = getmetatable(_class)
    // if mt == nil then return false end
    if ( lua_getmetatable(_l,_idx) == 0 ) {
        return false;
    }

    // if mt == _R["ex.class.meta"] then return true end
    // return false
    luaL_getmetatable(_l,"ex.class.meta");
    if ( lua_topointer(_l,-1) == lua_topointer(_l,-2) ) {
        lua_pop(_l, 2); // pops mt, ex.class.meta
        return true;
    }

    lua_pop(_l, 2); // pops mt, ex.class.meta
    return false;
}

static inline int __isclass ( lua_State *_l ) {
    lua_pushboolean( _l, ex_lua_isclass (_l,1) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function isbuiltin (_class)
//     if type(_class) ~= "table" then 
//         return false 
//     end 
//     local r = rawget(_class, "__builtin")
//     if r == nil then return false end
//     return r
// end
// ------------------------------------------------------------------ 

bool ex_lua_isbuiltin ( lua_State *_l, int _idx ) {
    bool r;

    // if type(_class) ~= "table" then return false end
    if ( lua_istable( _l, _idx ) == 0 ) {
        return false;
    }

    // local r = rawget(_class, "__builtin")
    lua_pushstring(_l,"__builtin");
    lua_rawget ( _l, _idx );

    // if r == nil then return false end
    if ( lua_isnil(_l,-1) ) {
        lua_pop(_l,1);
        return false;
    }

    // return r
    r = lua_toboolean(_l,-1);
    lua_pop(_l,1);
    return r;
}

static inline int __isbuiltin ( lua_State *_l ) {
    lua_pushboolean( _l, ex_lua_isbuiltin(_l,1) );
    return 1;
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
        lua_pop(_l,1); // pop tp

        // local name = rawget(typeof(_object), "__typename")
        if ( lua_getmetatable(_l,_idx) == 0 ) {
            ex_error ("can't find metatable in the class.");
            lua_pushnil(_l);
            return 1;
        }
        lua_pushstring(_l,"__typename");
        lua_rawget(_l,-2);
        lua_pop(_l,1); // pop mt

        // assert ( name ~= nil, "can't find __typename define in your class." )
        if ( lua_isnil(_l,-1) ) {
            ex_error ("can't find __tpename define in the class.");
        }

        // return name
        return 1;
    }

    // return tp
    return 1;
}

static inline int __typename ( lua_State *_l ) {
    return ex_lua_typename(_l,1);
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
    // if isbuiltin( typeof(_obj) ) then
    if ( lua_getmetatable(_l,OBJ_IDX) ) {
        if ( ex_lua_isbuiltin(_l, lua_gettop(_l)) ) {
            lua_pop(_l,1); // pops mt

            // assert(_obj.copy, "please provide copy function for builtin type: " .. typename(_obj) )
            lua_getfield(_l,OBJ_IDX,"copy");
            if ( lua_isnil(_l,-1) ) {
                ex_lua_typename(_l,OBJ_IDX);
                ex_error ( "please provide copy function for builtin type: %s", lua_tostring(_l,-1) );
                lua_pop(_l,2); // pops typename, nil 
                lua_pushnil(_l);
                return 1;
            }
            // return _obj:copy()
            lua_pushvalue(_l,OBJ_IDX);
            lua_call(_l,1,1);
            return 1;
        }
        lua_pop(_l,1); // pops mt
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

static int __deepcopy ( lua_State *_l ) {
    // local lookup_table = {}
    lua_newtable(_l);

    // def local functon _copy, and associate it with lookup table
    lua_pushcclosure(_l,__copy,1);

    // return _copy(_obj)
    lua_pushvalue(_l,1); // push first args to stack
    lua_call(_l,1,1); // call _copy with 1 args, 1 result.
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function class(...)
//     local base,super = ...
//     assert( type(base) == "table", "the first parameter must be a table" )
//
//     if super == nil then
//         rawset(base, "__super", nil)
//     else
//         assert( isclass(super), "super is not a class" )
//         rawset(base, "__super", super)
//     end
//
//     base.__index = class_index
//     base.__newindex = class_newindex
//     base.classof = classof
//     base.superof = superof
//     base.childof = childof
//     base.isa = isa
//     base.derive = function (_t)
//         return class( _t, base )
//     end
//     return setmetatable(base,class_meta)
// end
// ------------------------------------------------------------------ 

int ex_lua_class ( lua_State *_l, int _base_idx, int _super_idx ) {
    // local base,super = ...
    // assert( type(base) == "table", "the first parameter must be a table" )
    if ( lua_istable(_l,_base_idx) == 0 ) {
        ex_error ( "the first parameter must be a table" );
        lua_pushnil(_l);
        return 1;
    }

    // if super == nil then
    if ( lua_isnil(_l,2) ) {
        // rawset(base, "__super", nil)
        lua_pushstring(_l,"__super");
        lua_pushnil(_l);
        lua_rawset ( _l, _base_idx );
    }
    else {
        bool r = false;
        
        // assert( isclass(super), "super is not a class" )
        if ( ex_lua_isclass(_l,_super_idx) ) {
            ex_error( "super is not a class" );
            lua_pop(_l,1); // pop __isclass push
            lua_pushnil(_l);
            return 1;
        }
        lua_pop(_l,1); // pop __isclass push

        // rawset(base, "__super", super)
        lua_pushstring(_l,"__super");
        lua_pushvalue(_l,_super_idx);
        lua_rawset (_l,_base_idx);
    }

    // base.__index = class_index
    // base.__newindex = class_newindex
    // base.classof = classof
    // base.superof = superof
    // base.childof = childof
    // base.isa = isa
    // base.derive = function (_t)
    //     return class( _t, base )
    // end
    // return setmetatable(base,class_meta)
}

static inline int __class ( lua_State *_l ) {
    ex_lua_class(_l,1,2);
}


///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_core ( lua_State *_l ) {
    // ex.class.meta functions
    static const luaL_Reg __meta_funcs[] = {
        { "__call", __class_new },
        { NULL, NULL },
    };
    // ex functions
    static const luaL_Reg __core_funcs[] = {
        { "dumpstack", ex_lua_dump_stack }, // DEBUG:
        { "range_rand", __range_rand }, // TODO: go to ex.random.range
        { "log", __log },
        { "deepcopy", __deepcopy },
        { "typeof", __type_of }, // NOTE: c don't allow function named __typeof or typeof
        { "isclass", __isclass },
        { "isbuiltin", __isbuiltin },
        { "typename", __typename },
        { "class", __class },
        { 0, 0 }
    };

    //
    ex_lua_global_module ( _l, "ex" ); // [-0,+1,-]

    // register metatable ex.class.meta
    luaL_newmetatable(_l, "ex.class.meta"); // [-0,+1,m] // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, __meta_funcs); // [-1,+1,m]
    lua_pop(_l, 1); // pops ex.class.meta [-1,+0,-]

    //
    luaL_register( _l, 0, __core_funcs ); // [-1,+1,m]
    lua_pop(_l, 1); // pops ex. [-1,+0,-]

    return 0;
}

//
void luaclose_core () {
}
