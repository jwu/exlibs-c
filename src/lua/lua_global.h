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
// enhance 
///////////////////////////////////////////////////////////////////////////////

// global_module get the _key in global, check if it is a table, 
// if not, it pop-up the key and create a new table, set the global _key field 
// leave it at the top of the stack
extern int ex_lua_global_module ( lua_State *_l, const char *_key );

// table actually do the same thing as global_module, instead it operate the table at _idx
extern int ex_lua_module ( lua_State *_l, int _idx, const char *_key );

// usage: ex_lua_parse ( ex_lua_state(), luaL_loadfile(ex_lua_state(), filename) );
extern int ex_lua_parse ( lua_State *_l, int status ); 
extern void ex_lua_check ( lua_State *_l, int status );

///////////////////////////////////////////////////////////////////////////////
// engine level
///////////////////////////////////////////////////////////////////////////////

extern void ex_lua_init ();
extern void ex_lua_deinit ();
extern lua_State *ex_lua_state ();

// the load module will get base_path as the base directory for searching modules.
// if the base directory is NULL, it will to the current-working path for searching.
extern int ex_lua_load_module_byfile ( lua_State *_l, const wchar_t *_fullpath );
extern int ex_lua_load_module ( lua_State *_l, const char *_module );
extern int ex_lua_get_module ( lua_State *_l, const char *_moduleName );
extern int ex_lua_get_function ( lua_State *_l, const char *_moduleName, const char *_funcName );

// wrap helpers
// Usage: { "__gc", ex_lua__gc },
extern int ex_lua__gc ( lua_State *_l );

///////////////////////////////////////////////////////////////////////////////
// DEBUG:
///////////////////////////////////////////////////////////////////////////////

extern void ex_lua_dump_stack ( lua_State *_l );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_GLOBAL_H_1289788019
// #################################################################################


