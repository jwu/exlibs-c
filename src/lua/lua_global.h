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

extern struct lua_State *ex_lua_main_state ();

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

extern int ex_lua_dofile ( struct lua_State *_l, const char *_filepath, const char *_modname );
extern int ex_lua_dostring ( struct lua_State *_l, const char *_fmt, ... );
extern int ex_lua_clear_refs ( struct lua_State *_l );
extern int ex_lua_gc ( struct lua_State *_l );

///////////////////////////////////////////////////////////////////////////////
// lua module functions in c
///////////////////////////////////////////////////////////////////////////////

//
extern int ex_lua_tostring ( struct lua_State *_l, int _idx );
extern bool ex_lua_isclass ( struct lua_State *_l, int _idx );
extern bool ex_lua_isbuiltin ( struct lua_State *_l, int _idx );
extern int ex_lua_typename ( struct lua_State *_l, int _idx );
extern int ex_lua_deepcopy ( struct lua_State *_l, int _idx );
// Desc: this function use -1 value, -2 key, return vv on the top of the stack
extern int ex_lua_deepcopy_to ( struct lua_State *_l, int _idx );
extern int ex_lua_merge ( struct lua_State *_l, int _idx_to, int _idx_from );

//
extern int ex_lua_yield ( struct lua_State *_l, 
                          ex_ref_t *_self, 
                          int _lua_threadID, 
                          strid_t _nameID );

//
extern int ex_lua_class ( struct lua_State *_l, 
                          int _base_idx,
                          int _super_idx,
                          int _meta_idx,
                          bool _isbuiltin );

extern int ex_lua_type_meta_newindex ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );
extern int ex_lua_type_meta_index ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );

extern int ex_lua_meta_newindex ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );
extern int ex_lua_meta_index ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );

extern int ex_lua_child_meta_newindex ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );
extern int ex_lua_child_meta_index ( struct lua_State *_l, ex_hashmap_t *_key_to_getset );

///////////////////////////////////////////////////////////////////////////////
// lua debug op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_lua_alert ( struct lua_State *_l );
extern void ex_lua_dump_stack ( struct lua_State *_l );
extern int ex_lua_totoal_memory ( struct lua_State *_l );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_GLOBAL_H_1289788019
// #################################################################################


