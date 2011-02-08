// ======================================================================================
// File         : lua_wrap_helper.h
// Author       : Wu Jie 
// Last Change  : 02/08/2011 | 12:32:37 PM | Tuesday,February
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LUA_WRAP_HELPER_H_1297139559
#define LUA_WRAP_HELPER_H_1297139559
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_getset_t {
    const char *key;
    int (*get) ( struct lua_State * );
    int (*set) ( struct lua_State * );
} ex_getset_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_LUA_BUILTIN_TYPE_2(_typename,_type) \
    typedef struct _typename##_proxy_t { \
        bool readonly; \
        strid_t typeid; \
        _type val; \
    } _typename##_proxy_t; \
    extern _typename##_proxy_t *ex_lua_check##_typename ( struct lua_State *_l, int _idx ); \
    extern _typename##_proxy_t *ex_lua_push##_typename ( struct lua_State *_l, bool _readonly );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DECL_LUA_BUILTIN_TYPE_2(vec2f,ex_vec2f_t)

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_WRAP_HELPER_H_1297139559
// #################################################################################
