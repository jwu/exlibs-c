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
    int nargs = lua_gettop(_l);

    if ( nargs == 1 )  // if 2nd args is nil
        lua_newtable(_l); // push {}
    else
        lua_pushvalue(_l,2); // push 2nd args

    lua_pushvalue(_l,-1); // push the table to set
    lua_pushvalue(_l,1); // push _self

    lua_setmetatable(_l,-2); // setmetatable( new_table, _self )
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: NOTE: this function only used in __class_index 
// if v ~= nil then 
//     local vv = v
//     if type(vv) == "table" and getmetatable(vv) == nil then
//         vv = deepcopy(v)
//     end
//     rawset(_t,_k,vv)
//     return vv
// end
// ------------------------------------------------------------------ 

static inline int __copy_v ( lua_State *_l ) {
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
    // rawset(_t,_k,vv)
    lua_pushvalue(_l,2); // push key
    lua_pushvalue(_l,-2); // push vv
    lua_rawset ( _l, 1 );

    // return vv
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function class_index ( _t, _k )
//     -- NOTE: the _t can only be object instance, 
//     --       we can garantee this, case if it is a class, 
//     --       it never use class_index as __index method. 
//     --       it use meta_class.__index
//     -- speical case
//     if _k == "super" then
//         local mt = getmetatable(_t) 
//         assert( mt, "can't find the metatable of _t" )
//         -- NOTE: in class_newindex, it will check if table have __readonly, and prevent setting things.
//         return setmetatable( { __readonly = true }, rawget(mt,"__super") )
//     end
//     -- check if the metatable have the key
//     local mt = getmetatable(_t) 
//     assert( mt, "can't find the metatable of _t" )
//     local v = rawget(mt,_k)
//     if v ~= nil then 
//         local vv = v
//         if type(vv) == "table" and getmetatable(vv) == nil then
//             vv = deepcopy(v)
//         end
//         rawset(_t,_k,vv)
//         return vv
//     end
//     -- check if the super have the key
//     local super = rawget(mt,"__super")
//     while super ~= nil do
//         -- get key from super's metatable
//         v = rawget(super,_k)
//         --
//         if v ~= nil then 
//             local vv = v
//             if type(vv) == "table" and getmetatable(vv) == nil then
//                 vv = deepcopy(v)
//             end
//             rawset(_t,_k,vv)
//             return vv
//         end
//         -- get super's super from super's metatable
//         super = rawget(super,"__super")
//     end
//     -- return
//     return nil
// end
// ------------------------------------------------------------------ 

static int __class_index ( lua_State *_l ) {
    const char *key = luaL_checkstring(_l,2);

    // if _k == "super" then
    if ( strcmp( key, "super" ) == 0 ) {
        // local mt = getmetatable(_t) 
        if ( lua_getmetatable(_l,1) == 0 ) {
            ex_error ( "can't find the metatable of _t" );
            lua_pushnil(_l);
            return 1;
        }

        // new_table = { __readonly = true }
        lua_newtable(_l);
        lua_pushboolean(_l,true);
        lua_setfield(_l,-2,"__readonly");

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

    // check if the metatable have the key
    // local mt = getmetatable(_t) 
    if ( lua_getmetatable(_l,1) == 0 ) {
        ex_error ( "can't find the metatable of _t" );
        lua_pushnil(_l);
        return 1;
    }
    // v = rawget(mt,_k)
    lua_pushvalue(_l,2);
    lua_rawget(_l,-2);

    // if v ~= nil then 
    if ( lua_isnil(_l,-1) == 0 ) {
        return __copy_v(_l);
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
            return __copy_v(_l);
        }
        lua_pop(_l,1); // pops v, pops rawget

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
// Desc: 
// local function class_newindex ( _t, _k, _v )
//     -- NOTE: the _t can only be object instance, 
//     --       we can garantee this, case if it is a class, 
//     --       it never use class_index as __index method. 
//     --       it use meta_class.__index
//     -- make sure only get __readonly in table _t, not invoke __index method.
//     local is_readonly = rawget(_t,"__readonly")
//     if is_readonly then -- this equals to (is_readonly ~= nil and is_readonly == true)
//         assert ( false, "the table is readonly" )
//         return
//     end
//     -- check if the metatable have the key
//     local mt = getmetatable(_t) 
//     assert( mt, "can't find the metatable of _t" )
//     local v = rawget(mt,_k)
//     if v ~= nil then 
//         rawset(_t,_k,_v)
//         return
//     end
//     -- check if the super have the key
//     local super = rawget(mt,"__super")
//     while super ~= nil do
//         -- get key from super's metatable
//         v = rawget(super,_k)
//         --
//         if v ~= nil then 
//             rawset(_t,_k,_v)
//             return
//         end
//         -- get super's super from super's metatable
//         super = rawget(super,"__super")
//     end
//     -- 
//     assert( false, "can't find the key " .. _k )
//     return
// end
// ------------------------------------------------------------------ 

static int __class_newindex ( lua_State *_l ) {
    // make sure only get __readonly in table _t, not invoke __index method.
    // local is_readonly = rawget(_t,"__readonly")
    lua_pushstring(_l,"__readonly");
    lua_rawget(_l,1);
    // if is_readonly then -- this equals to (is_readonly ~= nil and is_readonly == true)
    if ( lua_isboolean(_l,-1) && lua_toboolean(_l,-1) ) {
        ex_error ("the table is readonly");
        return 0;
    }
    lua_pop(_l,1); // pop is_readonly

    // check if the metatable have the key
    // local mt = getmetatable(_t) 
    if ( lua_getmetatable(_l,1) == 0 ) {
        ex_error ( "can't find the metatable of _t" );
        return 0;
    }
    // v = rawget(mt,_k)
    lua_pushvalue(_l,2);
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
        lua_pop(_l,1); // pops v, pops rawget

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget

    ex_error ( "can't find the key %s", lua_tostring(_l,2) );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function classof(_object, _class)
//     return typeof(_object) == _class
// end
// ------------------------------------------------------------------ 

static int __classof ( lua_State *_l ) {
    bool r;

    // typeof(_object)
    if ( lua_getmetatable(_l,1) == 0 ) {
        lua_pushboolean(_l,false);
        return 1;
    }

    // return typeof(_object) == _class
    r = lua_rawequal( _l, -1, 2 );
    lua_pushboolean(_l,r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function __childof(_myclass,_superclass)
//     local super = rawget(_myclass,"__super")
//     while super ~= nil do
//         if super == _superclass then 
//             return true 
//         end
//         super = rawget(super,"__super")
//     end
//     return false
// end
// ------------------------------------------------------------------ 

static inline bool __internal_childof ( lua_State *_l, int _my_idx, int _super_idx ) {
    // check if the super have the key
    // local super = rawget(_myclass,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,_my_idx);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // if super == _superclass then 
        if ( lua_rawequal( _l, -1, _super_idx ) ) {
            lua_pop(_l,1); // pops rawget
            return true;
        }

        // get super's super from super's metatable
        // super = rawget(super,"__super")
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }
    lua_pop(_l,1); // pops rawget
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function superof(_object, _subclass)
//     return __childof(_subclass,typeof(_object))
// end
// ------------------------------------------------------------------ 

static int __superof ( lua_State *_l ) {
    bool r;

    // typeof(_object)
    if ( lua_getmetatable(_l,1) == 0 ) {
        lua_pushboolean(_l,false);
        return 1;
    }

    r =  __internal_childof( _l, 2, lua_gettop(_l) );
    lua_pushboolean(_l,r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function childof(_object, _superclass)
//     return __childof(typeof(_object),_superclass)
// end
// ------------------------------------------------------------------ 

static int __childof ( lua_State *_l ) {
    bool r;

    // typeof(_object)
    if ( lua_getmetatable(_l,1) == 0 ) {
        lua_pushboolean(_l,false);
        return 1;
    }

    r =  __internal_childof( _l, lua_gettop(_l), 2 );
    lua_pushboolean(_l,r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function isa(_object, _class)
//     local cls = typeof(_object)
//     return cls == _class or __childof(cls,_class)
// end
// ------------------------------------------------------------------ 

static int __isa ( lua_State *_l ) {
    bool r;

    // typeof(_object)
    if ( lua_getmetatable(_l,1) == 0 ) {
        lua_pushboolean(_l,false);
        return 1;
    }

    // typeof(_object) == _class
    if ( lua_rawequal(_l,-1,2) ) {
        lua_pushboolean(_l,true);
        return 1;
    }

    // __childof(typeof(_object),_class)
    r =  __internal_childof( _l, lua_gettop(_l), 2 );
    lua_pushboolean(_l,r);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __dump_stack ( lua_State *_l ) {
    ex_lua_dump_stack(_l);
    return 0;
}

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

static int __type_of ( lua_State *_l ) {
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
    if ( lua_rawequal(_l,-1,-2) ) {
        lua_pop(_l, 2); // pops mt, ex.class.meta
        return true;
    }

    lua_pop(_l, 2); // pops mt, ex.class.meta
    return false;
}

static int __isclass ( lua_State *_l ) {
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

static int __isbuiltin ( lua_State *_l ) {
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
        // local name = rawget(typeof(_object), "__typename")
        if ( lua_getmetatable(_l,_idx) == 0 ) {
            ex_error ("can't find metatable in the class.");
            lua_pushnil(_l);
            return 1;
        }
        lua_pushstring(_l,"__typename");
        lua_rawget(_l,-2);

        // assert ( name ~= nil, "can't find __typename define in your class." )
        if ( lua_isnil(_l,-1) ) {
            ex_error ("can't find __tpename define in the class.");
        }

        // return name
        lua_remove(_l, -2); // remove mt
        return 1;
    }

    // return type(_object)
    lua_pushstring(_l, luaL_typename(_l,_idx) );
    return 1;
}

static int __typename ( lua_State *_l ) {
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

static int ex_lua_deepcopy ( lua_State *_l, int _idx ) {
    // local lookup_table = {}
    lua_newtable(_l);

    // def local functon _copy, and associate it with lookup table
    lua_pushcclosure(_l,__copy,1);

    // return _copy(_obj)
    lua_pushvalue(_l,_idx); // push first args to stack
    lua_call(_l,1,1); // call _copy with 1 args, 1 result.
    return 1;
}
static int __deepcopy ( lua_State *_l ) {
    return ex_lua_deepcopy(_l,1);
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

static int __class ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    if ( nargs == 0 ) {
        ex_error ( "can't find any input arguments" );
        lua_pushnil(_l);
        return 1;
    }
    if ( nargs == 1 ) {
        lua_pushnil(_l);
    }
    return ex_lua_class(_l,1,2);
}

static int __derive ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    if ( nargs != 1 ) {
        ex_error ( "only 1 arguments accept in derive function" );
        lua_pushnil(_l);
        return 1;
    }
    return ex_lua_class(_l,1,lua_upvalueindex(1));
}

int ex_lua_class ( lua_State *_l, int _base_idx, int _super_idx ) {
    // local base,super = ...
    // assert( type(base) == "table", "the first parameter must be a table" )
    if ( lua_istable(_l,_base_idx) == 0 ) {
        ex_error ( "the first parameter must be a table" );
        lua_pushnil(_l);
        return 1;
    }

    // if super == nil then
    if ( lua_isnil(_l,_super_idx) ) {
        // rawset(base, "__super", nil)
        lua_pushstring(_l,"__super");
        lua_pushnil(_l);
        lua_rawset ( _l, _base_idx );
    }
    else {
        // assert( isclass(super), "super is not a class" )
        if ( !ex_lua_isclass(_l,_super_idx) ) {
            ex_error( "super is not a class" );
            lua_pop(_l,1); // pop __isclass push
            lua_pushnil(_l);
            return 1;
        }

        // rawset(base, "__super", super)
        lua_pushstring(_l,"__super");
        lua_pushvalue(_l,_super_idx);
        lua_rawset (_l,_base_idx);
    }

    // base.__index = class_index
    lua_pushcfunction(_l,__class_index);
    lua_setfield(_l,_base_idx,"__index");

    // base.__newindex = class_newindex
    lua_pushcfunction(_l,__class_newindex);
    lua_setfield(_l,_base_idx,"__newindex");

    // base.classof = classof
    lua_pushcfunction(_l,__classof);
    lua_setfield(_l,_base_idx,"classof");

    // base.superof = superof
    lua_pushcfunction(_l,__superof);
    lua_setfield(_l,_base_idx,"superof");

    // base.childof = childof
    lua_pushcfunction(_l,__childof);
    lua_setfield(_l,_base_idx,"childof");

    // base.isa = isa
    lua_pushcfunction(_l,__isa);
    lua_setfield(_l,_base_idx,"isa");

    // base.derive = function (_t)
    //     return class( _t, base )
    // end
    lua_pushvalue(_l,_base_idx);
    lua_pushcclosure(_l,__derive,1);
    lua_setfield(_l,_base_idx,"derive");

    // return setmetatable(base,ex.class.meta)
    luaL_getmetatable(_l,"ex.class.meta");
    lua_setmetatable(_l,_base_idx);
    lua_pushvalue(_l,_base_idx);
    return 1;
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
        { "dump_stack", __dump_stack }, // DEBUG:
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
