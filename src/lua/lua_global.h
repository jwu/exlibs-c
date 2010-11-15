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
int ex_lua_global_module ( lua_State* _l, const char* _key );

// table actually do the same thing as global_module, instead it operate the table at _idx
int ex_lua_module ( lua_State* _l, int _idx, const char* _key );

// usage: ex_lua_parse ( ex_lua_state(), luaL_loadfile(ex_lua_state(), filename) );
int ex_lua_parse ( lua_State* _l, int status ); 
void ex_lua_check ( lua_State* _l, int status );

///////////////////////////////////////////////////////////////////////////////
// engine level
///////////////////////////////////////////////////////////////////////////////

void ex_lua_init ();
void ex_lua_deinit ();
lua_State* ex_lua_state ();

// the load module will get base_path as the base directory for searching modules.
// if the base directory is NULL, it will to the current-working path for searching.
int ex_lua_load_module_byfile ( lua_State* _l, const wchar_t* _fullpath );
int ex_lua_load_module ( lua_State* _l, const char* _module );
int ex_lua_get_module ( lua_State* _l, const char* _moduleName );
int ex_lua_get_function ( lua_State* _l, const char* _moduleName, const char* _funcName );

// wrap helpers
// Usage: { "__gc", ex_lua__gc },
int ex_lua__gc ( lua_State* _l );

///////////////////////////////////////////////////////////////////////////////
// DEBUG:
///////////////////////////////////////////////////////////////////////////////

void ex_lua_dump_stack ( lua_State* _l );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_GLOBAL_H_1289788019
// #################################################################################


