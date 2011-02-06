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

// NOTE: this will prevent include lua.h in the header file.
typedef struct lua_State lua_state_t;

///////////////////////////////////////////////////////////////////////////////
// core
///////////////////////////////////////////////////////////////////////////////

extern int ex_lua_init ();
extern void ex_lua_deinit ();
extern bool ex_lua_initialized ();

extern lua_state_t *ex_lua_default_state ();

///////////////////////////////////////////////////////////////////////////////
// lua api extend
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_add_path ( lua_state_t *_l, const char *_path );
extern int ex_lua_add_cpath ( lua_state_t *_l, const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
// global_module get the _key in global, check if it is a table, 
// if not, it pop-up the key and create a new table, set the global 
// _key field leave it at the top of the stack.
// [-0, +1, -]
// ------------------------------------------------------------------ 

extern int ex_lua_global_module ( lua_state_t *_l, const char *_key );

// ------------------------------------------------------------------ 
// Desc: 
// module actually do the same thing as global_module, 
// instead it operate the table at _idx
// [-0, +1, -]
// ------------------------------------------------------------------ 

extern int ex_lua_module ( lua_state_t *_l, int _idx, const char *_key );

// ------------------------------------------------------------------ 
// Desc: 
// load the module from file in specific directory
// NOTE: base_path must end with "/"
// ------------------------------------------------------------------ 

extern int ex_lua_load_modules ( lua_state_t *_l, const char *_base_path );
extern int ex_lua_load_module_byfile ( lua_state_t *_l, const char *_file_name );
extern int ex_lua_load_module_byfile2 ( lua_state_t *_l, const char *_file_name, const char *_module_name );

#if 0
extern int ex_lua_get_module ( lua_state_t *_l, const char *_moduleName );
extern int ex_lua_get_function ( lua_state_t *_l, const char *_moduleName, const char *_funcName );

// wrap helpers
// Usage: { "__gc", ex_lua__gc },
extern int ex_lua__gc ( lua_state_t *_l );
#endif

///////////////////////////////////////////////////////////////////////////////
// interpreter
///////////////////////////////////////////////////////////////////////////////

extern int ex_lua_dofile ( lua_state_t *_l, const char *_filepath );
extern int ex_lua_dostring ( lua_state_t *_l, const char *_fmt, ... );

///////////////////////////////////////////////////////////////////////////////
// DEBUG:
///////////////////////////////////////////////////////////////////////////////

extern void ex_lua_alert ( lua_state_t *_l );
extern void ex_lua_dump_stack ( lua_state_t *_l );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_GLOBAL_H_1289788019
// #################################################################################


