// ======================================================================================
// File         : lua_global.h
// Author       : Wu Jie 
// Last Change  : 11/15/2010 | 10:26:56 AM | Monday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LUA_GLOBAL_H_1289788019
#define LUA_GLOBAL_H_1289788019
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// lua core
///////////////////////////////////////////////////////////////////////////////

extern int ex_lua_init ();
extern void ex_lua_deinit ();
extern bool ex_lua_initialized ();

extern struct lua_State *ex_lua_default_state ();

///////////////////////////////////////////////////////////////////////////////
// lua module op
///////////////////////////////////////////////////////////////////////////////

// DEPRECATED { 
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_add_path ( struct lua_State *_l, const char *_path );
extern int ex_lua_add_cpath ( struct lua_State *_l, const char *_path );
// } DEPRECATED end 

// ------------------------------------------------------------------ 
// Desc: 
// global_module get the _key in global, check if it is a table, 
// if not, it pop-up the key and create a new table, set the global 
// _key field leave it at the top of the stack.
// [-0, +1, -]
// ------------------------------------------------------------------ 

extern int ex_lua_global_module ( struct lua_State *_l, const char *_key );

// ------------------------------------------------------------------ 
// Desc: 
// module actually do the same thing as global_module, 
// instead it operate the table at _idx
// [-0, +1, -]
// ------------------------------------------------------------------ 

extern int ex_lua_module ( struct lua_State *_l, int _idx, const char *_key );

// ------------------------------------------------------------------ 
// Desc: 
// load the module from file in specific directory
// NOTE: base_path must end with "/"
// ------------------------------------------------------------------ 

extern int ex_lua_load_modules ( struct lua_State *_l, const char *_dir );
extern int ex_lua_load_module_byfile ( struct lua_State *_l, const char *_base_path, const char *_file_name );
extern int ex_lua_load_module_byfile2 ( struct lua_State *_l, const char *_filepath, const char *_module_name );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_get_module ( struct lua_State *_l, const char *_moduleName );
extern int ex_lua_get_function ( struct lua_State *_l, const char *_moduleName, const char *_funcName );

///////////////////////////////////////////////////////////////////////////////
// lua interpreter op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_dofile ( struct lua_State *_l, const char *_filepath );
extern int ex_lua_dostring ( struct lua_State *_l, const char *_fmt, ... );

///////////////////////////////////////////////////////////////////////////////
// lua debug op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_lua_alert ( struct lua_State *_l );
extern void ex_lua_dump_stack ( struct lua_State *_l );

///////////////////////////////////////////////////////////////////////////////
// lua register op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_register_builtin ( struct lua_State *_l, const char *_typeName, const void *_funcs );
extern int ex_lua_register_class ( struct lua_State *_l, const char *_typeName, const void *_funcs );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_GLOBAL_H_1289788019
// #################################################################################


