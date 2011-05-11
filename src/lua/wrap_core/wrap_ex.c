// ======================================================================================
// File         : wrap_ex.c
// Author       : Wu Jie 
// Last Change  : 03/18/2011 | 10:25:32 AM | Friday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/component/lua_behavior.h"
#include "../../graphics/graphics_inc.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

extern int __lua_index( lua_State *, int );

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
// Desc: 
// local function class_index ( _t, _k )
//     -- NOTE: the _t can only be object instance, 
//     --       we can garantee this, case if it is a class, 
//     --       it never use class_index as __index method. 
//     --       it use meta_class.__index
//     -- speical case
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
//     -- return
//     return nil
// end
// ------------------------------------------------------------------ 

static int __class_index ( lua_State *_l ) {
    // check if the metatable have the key
    // local mt = getmetatable(_t) 
    if ( lua_getmetatable(_l,1) == 0 ) {
        return luaL_error ( _l, "can't find the metatable of _t" );
    }
    // v = rawget(mt,_k)
    lua_pushvalue(_l,2); // leaves for copy_to
    lua_pushvalue(_l,2);
    lua_rawget(_l,-3);

    // if v ~= nil then 
    if ( lua_isnil(_l,-1) == 0 ) {
        return ex_lua_deepcopy_to(_l,1);
    }
    lua_pop(_l,2); // pops k,v

    // return nil
    lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// local function class_newindex ( _t, _k, _v )
//     -- check if the metatable have the key
//     local mt = getmetatable(_t) 
//     assert( mt, "can't find the metatable of _t" )
//     local v = rawget(mt,_k)
//     if v ~= nil then 
//         rawset(_t,_k,_v)
//         return
//     end
//     -- 
//     assert( false, "can't find the key " .. _k )
//     return
// end
// ------------------------------------------------------------------ 

static int __class_newindex ( lua_State *_l ) {
    // check if the metatable have the key
    // local mt = getmetatable(_t) 
    if ( lua_getmetatable(_l,1) == 0 ) {
        return luaL_error ( _l, "can't find the metatable of _t" );
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

    return luaL_error ( _l, "can't find the key %s", lua_tostring(_l,2) );
}

// ------------------------------------------------------------------ 
// Desc: 
// local function instanceof(_object, _class)
//     return typeof(_object) == _class
// end
// ------------------------------------------------------------------ 

static int __instanceof ( lua_State *_l ) {
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
    int my_idx = __lua_index(_l,_my_idx);
    int super_idx = __lua_index(_l,_super_idx);

    // check if the super have the key
    // local super = rawget(_myclass,"__super")
    lua_pushstring(_l,"__super");
    lua_rawget(_l,my_idx);

    // while super ~= nil do
    while ( lua_isnil(_l,-1) == 0 ) {
        // if super == _superclass then 
        if ( lua_rawequal( _l, -1, super_idx ) ) {
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

    r =  __internal_childof( _l, 2, -1 );
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

    r =  __internal_childof( _l, -1, 2 );
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
    r =  __internal_childof( _l, -1, 2 );
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

static int __screen_print ( lua_State *_l ) {
    int x = luaL_checknumber( _l, 1 );
    int y = luaL_checknumber( _l, 2 );
    const char *text = luaL_checkstring( _l, 3 );

    ex_screen_print ( x, y, text );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __draw_text ( lua_State *_l ) {
    float x = luaL_checknumber( _l, 1 );
    float y = luaL_checknumber( _l, 2 );
    float z = luaL_checknumber( _l, 3 );
    const char *text = luaL_checkstring( _l, 4 );

    ex_draw_text ( x, y, z, text );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __draw_point ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    float x, y;

    if ( nargs == 1 ) {
        ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);
        x = v->x;
        y = v->y;
    }
    else {
        x = luaL_checknumber( _l, 1 );
        y = luaL_checknumber( _l, 2 );
    }

    ex_draw_point ( x, y );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __draw_line ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    ex_vec2f_t *s, *e;
    const ex_color4f_t *c;

    if ( nargs == 2 ) {
        s = ex_lua_checkvec2f(_l,1);
        e = ex_lua_checkvec2f(_l,2);
        c = &ex_color4f_white;
    }
    else if ( nargs == 3 ) {
        s = ex_lua_checkvec2f(_l,1);
        e = ex_lua_checkvec2f(_l,2);
        c = ex_lua_checkcolor4f(_l,3);
    }
    else {
        return luaL_error( _l, "invalid number of arguments." );
    }

    ex_draw_line ( s, e, c );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __draw_poly ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    int i, num;
    ex_vec2f_t *verts;
    const ex_color4f_t *c;
    bool close;

    // check if the first arguments is table
    luaL_checktype(_l, 1, LUA_TTABLE);
    num = luaL_getn(_l,1);
    verts = (ex_vec2f_t *)ex_malloc( sizeof(ex_vec2f_t) * num );

    for ( i = 0; i < num; ++i ) {
        lua_rawgeti(_l, 1, i+1);
        verts[i] = *ex_lua_tovec2f(_l,-1);
        lua_remove(_l, -1);
    }

    //
    c = &ex_color4f_white;
    close = true;
    if ( nargs == 2 ) {
        c = ex_lua_checkcolor4f(_l,2);
    }
    else if ( nargs == 3 ) {
        c = ex_lua_checkcolor4f(_l,2);
        close = lua_toboolean(_l,3);
    }

    //
    ex_draw_poly ( verts, num, c, close );
    ex_free( verts );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __draw_circle ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    ex_vec2f_t *center;
    float r, a;
    int segs;
    const ex_color4f_t *c;
    bool lineToCenter;

    // check if the first arguments is table
    center = ex_lua_checkvec2f(_l,1);
    r = luaL_checknumber(_l,2);
    a = luaL_checknumber(_l,3);

    segs = 32;
    c = &ex_color4f_white;
    lineToCenter = true;

    //
    if ( nargs == 4 ) {
        segs = luaL_checknumber(_l,4);
    }
    else if ( nargs == 5 ) {
        segs = luaL_checknumber(_l,4);
        c = ex_lua_checkcolor4f(_l,5);
    }
    else if ( nargs == 6 ) {
        segs = luaL_checknumber(_l,4);
        c = ex_lua_checkcolor4f(_l,5);
        lineToCenter = lua_toboolean(_l,6);
    }

    //
    ex_draw_circle ( center, r, a, segs, c, lineToCenter );
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
    const char *msg = NULL;

    ex_lua_tostring(_l,1);
    msg = luaL_checkstring ( _l, -1 );
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
// ------------------------------------------------------------------ 

static int __isclass ( lua_State *_l ) {
    lua_pushboolean( _l, ex_lua_isclass (_l,1) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __isbuiltin ( lua_State *_l ) {
    lua_pushboolean( _l, ex_lua_isbuiltin(_l,1) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __typename ( lua_State *_l ) {
    return ex_lua_typename(_l,1);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

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
//         assert( type(super) == "table", "the super parameter must be a table" )
//         assert( getmetatable(super) == meta_class, "the super parameter must be a class" )
//         rawset(base, "__super", super)
//     end
//
//     base.__index = class_index
//     base.__newindex = class_newindex
//     base.instanceof = instanceof
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
    bool derived_from_builtin = false;

    if ( nargs == 0 ) {
        return luaL_error ( _l, "can't find any input arguments" );
    }
    if ( nargs == 1 ) {
        lua_pushnil(_l);
    }

    // check if super is builtin
    lua_pushvalue(_l,-1); // push super
    while ( lua_isnil(_l,-1) == false ) {
        if ( ex_lua_isbuiltin(_l,-1) ) {
            derived_from_builtin = true;
            lua_pop(_l,1); // pops pushvalue
            break;
        }

        // super = super.__super
        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }

    // push the suitable meta table
    if ( derived_from_builtin ) {
        lua_pushstring(_l,"__metaclass");
        lua_rawget(_l,-2);
        if ( lua_isnil(_l,-1) ) {
            return luaL_error ( _l, "can't find __metaclass in the super class." );
        }
    }
    else {
        luaL_getmetatable(_l,"ex.class.meta");
    }
    lua_remove(_l,-2); // pops pushvalue

    return ex_lua_class( _l, 1, 2, -1, false );
}

static int __derive ( lua_State *_l ) {
    int nargs = lua_gettop(_l);
    bool derived_from_builtin = false;

    if ( nargs != 1 ) {
        return luaL_error ( _l, "only 1 arguments accept in derive function" );
    }

    // DEBUG { 
    // ex_lua_global_module ( _l, "ex" );
    // lua_getfield(_l, -1, "debug");
    // lua_getfield(_l, -1, "dump");
    // lua_pushvalue(_l,1);
    // lua_call( _l, 1, 0 );
    // } DEBUG end 
    lua_getfield(_l,1,"_NAME");
    // NOTE: the _NAME only defined when we call derive in module(...).
    if ( lua_isnil(_l,-1) == false ) {
        lua_pushstring(_l,"__typename");
        lua_pushvalue(_l,-2);
        lua_rawset(_l,1);
    }
    else {
        lua_pop(_l,1);
    }

    // check if super is builtin
    lua_pushvalue(_l,lua_upvalueindex(1)); // push super
    while ( lua_isnil(_l,-1) == false ) {
        if ( ex_lua_isbuiltin(_l,-1) ) {
            derived_from_builtin = true;
            lua_pop(_l,1); // pops pushvalue
            break;
        }

        lua_pushstring(_l,"__super");
        lua_rawget(_l,-2);
        lua_remove(_l,-2); // pops prev rawget
    }

    // push the suitable meta table
    if ( derived_from_builtin ) {
        lua_pushstring(_l,"__metaclass");
        lua_rawget(_l,-2);
        if ( lua_isnil(_l,-1) ) {
            return luaL_error ( _l, "can't find __metaclass in the super class." );
        }
    }
    else {
        luaL_getmetatable(_l,"ex.class.meta");
    }
    lua_remove(_l,-2); // pops pushvalue

    return ex_lua_class( _l, 1, lua_upvalueindex(1), -1, false );
}

int ex_lua_class ( lua_State *_l, 
                   int _base_idx,
                   int _super_idx,
                   int _meta_idx,
                   bool _isbuiltin ) {

    const char *typename = NULL;
    const char *super_typename = NULL;

    int base_idx = __lua_index(_l,_base_idx);
    int super_idx = __lua_index(_l,_super_idx);
    int meta_idx = __lua_index(_l,_meta_idx);

    // local base,super = ...
    // assert( type(base) == "table", "the first parameter must be a table" )
    if ( lua_istable(_l,base_idx) == 0 ) {
        return luaL_error ( _l, "the first parameter must be a table" );
    }

    // if you don't define the __typename, report error!
    lua_pushstring(_l,"__typename");
    lua_rawget(_l,base_idx);
    if ( lua_isnil(_l,-1) ) {
        return luaL_error ( _l, "please define __typename for your class!" );
    }
    typename = luaL_checkstring(_l,-1);
    lua_pop(_l,1); // pos field __typename

    // if super == nil then
    if ( lua_isnil(_l,super_idx) ) {
        // rawset(base, "__super", nil)
        lua_pushstring(_l,"__super");
        lua_pushnil(_l);
        lua_rawset ( _l, base_idx );
    }
    else {
        bool is_class = false;
        bool is_super_builtin = false;

        // assert( type(super) == "table", "the super parameter must be a table" )
        // assert( rawget(_super,"__isclass"), "the super parameter must be a class" )
        if ( lua_istable(_l,super_idx) ) {
            // local r = rawget(super, "__isclass")
            lua_pushstring(_l,"__isclass");
            lua_rawget ( _l, super_idx );

            // if r ~= nil and r == true then 
            if ( lua_isnil(_l,-1) == 0 && lua_toboolean(_l,-1) ) {
                lua_pop(_l,1); // pops r
                is_class = true;
            }
        }
        if ( is_class == false ) {
            return luaL_error( _l, "super is not a class" );
        }

        // get super.__typename
        // if you don't define the __typename, report error!
        lua_pushstring(_l,"__typename");
        lua_rawget(_l,super_idx);
        if ( lua_isnil(_l,-1) ) {
            return luaL_error ( _l, "can't find __typename in super class!" );
        }
        super_typename = luaL_checkstring(_l,-1);
        lua_pop(_l,1); // pos field __typename

        // local is_super_builtin = rawget(tp, "__isbuiltin")
        lua_pushstring(_l,"__isbuiltin");
        lua_rawget ( _l, super_idx );
        is_super_builtin = lua_toboolean(_l,-1);
        lua_pop(_l,1); // pops is_super_builtin

        // NOTE: we only allow lua class inherit from ex.lua_object or ex.lua_behavior if super class is builtin.
        if ( is_super_builtin && !_isbuiltin ) {
            if ( strcmp ( super_typename, "ex.lua_object" ) != 0 &&
                 strcmp ( super_typename, "ex.lua_behavior" ) != 0 ) 
            {
                return luaL_error ( _l, 
                                    "lua class can only inherit from lua classes," 
                                    "ex.lua_object and ex.lua_behavior. [%s : %s]", 
                                    typename, super_typename );
            }
        }

        // rawset(base, "__super", super)
        lua_pushstring(_l,"__super");
        lua_pushvalue(_l,super_idx);
        lua_rawset (_l,base_idx);

        // ADD, TESTME { 
        // recursively merge member from super
        lua_pushvalue(_l,super_idx); // push super
        while ( lua_isnil(_l,-1) == 0 ) {
            // merge member from super
            ex_lua_merge ( _l, base_idx, -1 );

            // get super's super from super's metatable
            // super = rawget(super,"__super")
            lua_pushstring(_l,"__super");
            lua_rawget(_l,-2);
            lua_remove(_l,-2); // pops prev rawget
        }
        lua_pop(_l,1); // pops rawget
        // } ADD end 
    }

    // register the base table in metatable
    lua_pushvalue(_l,base_idx);
    lua_setfield( _l, LUA_REGISTRYINDEX, typename );

    // rawset(base,"__isclass", true)
    lua_pushstring(_l,"__isclass");
    lua_pushboolean(_l,true);
    lua_rawset(_l,base_idx);

    // rawset(base,"__isbuiltin", _isbuiltin)
    lua_pushstring(_l,"__isbuiltin");
    lua_pushboolean(_l,_isbuiltin);
    lua_rawset(_l,base_idx);

    // if this is not builtin type, add our custom index, newindex
    if ( !_isbuiltin ) {
        // rawset(base,"__index", class_index)
        lua_pushstring(_l,"__index");
        lua_pushcfunction(_l,__class_index);
        lua_rawset(_l,base_idx);

        // rawset(base,"__newindex", class_newindex)
        lua_pushstring(_l,"__newindex");
        lua_pushcfunction(_l,__class_newindex);
        lua_rawset(_l,base_idx);
    }

    // TODO: must check if base have __serialize, if not use the default one { 
    // // base.__serialize = class_serialize
    // lua_pushcfunction(_l,class_serialize);
    // lua_setfield(_l,base_idx,"__serialize");
    // } TODO end 

    // rawset(base,"instanceof", instanceof)
    lua_pushstring(_l,"instanceof");
    lua_pushcfunction(_l,__instanceof);
    lua_rawset(_l,base_idx);

    // rawset(base,"superof", superof)
    lua_pushstring(_l,"superof");
    lua_pushcfunction(_l,__superof);
    lua_rawset(_l,base_idx);

    // rawset(base,"childof", childof)
    lua_pushstring(_l,"childof");
    lua_pushcfunction(_l,__childof);
    lua_rawset(_l,base_idx);

    // rawset(base,"isa", isa)
    lua_pushstring(_l,"isa");
    lua_pushcfunction(_l,__isa);
    lua_rawset(_l,base_idx);

    // derive_func = function (_t)
    //     return class( _t, base )
    // end
    // rawset(base,"derive", derive_func)
    lua_pushstring(_l,"derive");
    lua_pushvalue(_l,base_idx);
    lua_pushcclosure(_l,__derive,1);
    lua_rawset(_l,base_idx);

    // return setmetatable(base, meta)
    lua_pushvalue(_l,meta_idx);
    lua_setmetatable(_l,base_idx);
    lua_pushvalue(_l,base_idx);

    // dynamically register the new class to rtti
    if ( !_isbuiltin ) {
        // TODO { 
        // strid_t typeID = ex_strid(typename);
        // ex_rtti_register_class ( typeID, 
        //                          ex_rtti_t *_super, 
        //                          size_t _typeSize,
        //                          ex_create_pfn _pfn_create,
        //                          ex_serialize_pfn _pfn_serialize,
        //                          ex_tostring_pfn _pfn_tostring
        //                        );
        // } TODO end 

        // HACK: the create method should change to some closure like function,
        //       also, builtin also include other derived type, not just lua_behavior { 
        strid_t typeID = ex_strid(typename);
        ex_rtti_t *super_rtti = NULL;
        if ( super_typename ) {
            super_rtti = ex_rtti_get( ex_strid(super_typename) );
            ex_assert( super_rtti, "can't find super rtti for type %s", super_typename );
        }

        ex_rtti_register_class ( typeID, 
                                 super_rtti,
                                 0,
                                 // how can I create script class that inherit from c-type 
                                 // in runtime (answer: call the script class' constructor)
                                 __ex_create_ex_lua_behavior_t,
                                 __ex_serialize_ex_lua_behavior_t,
                                 __ex_tostring_ex_lua_behavior_t
                               );
        // } HACK end 
    }
    // TODO: also think about dynamically unregister type.

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex functions
static const luaL_Reg __ex_funcs[] = {
    // debug
    { "dump_stack", __dump_stack },     // for DEBUG:
    { "screen_print", __screen_print }, // for DEBUG:
    { "draw_text", __draw_text },       // for DEBUG:
    { "draw_point", __draw_point },     // for DEBUG:
    { "draw_line", __draw_line },       // for DEBUG:
    { "draw_poly", __draw_poly },       // for DEBUG:
    { "draw_circle", __draw_circle },   // for DEBUG:
    { "log", __log },
    // randome
    { "range_rand", __range_rand }, // TODO: move to ex.random.range
    // class op
    { "deepcopy", __deepcopy },
    { "typeof", __type_of }, // NOTE: c don't allow function named __typeof or typeof
    { "isclass", __isclass },
    { "isbuiltin", __isbuiltin },
    { "typename", __typename },
    { "class", __class },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
extern int wrap_enums ( lua_State * );
// ------------------------------------------------------------------ 

int luaopen_ex ( lua_State *_l ) {

    // ======================================================== 
    // register metatable ex.class.meta
    // ======================================================== 

    luaL_newmetatable(_l, "ex.class.meta"); // NOTE: this store a table in LUA_REGISTRYINDEX
    lua_pushstring(_l,"__call");
    lua_pushcfunction(_l, __class_new);
    lua_rawset(_l,-3);
    lua_pop(_l, 1); // pops ex.class.meta

    // ======================================================== 
    // register ex
    // ======================================================== 

    ex_lua_global_module ( _l, "ex" );
    luaL_register( _l, 0, __ex_funcs );
    wrap_enums(_l);
    lua_pop(_l, 1); // pops ex.

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_ex () {
}
