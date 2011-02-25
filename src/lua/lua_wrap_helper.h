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

typedef struct ref_proxy_t { 
    strid_t typeid;
    bool readonly; 
    ex_ref_t *val; 
    // bool gc_owner; // TODO: do we really need this ??? 
} ref_proxy_t; 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// EX_DECL_LUA_BUILTIN_TYPE, EX_DECL_LUA_BUILTIN_TYPE_2
#define EX_DECL_LUA_BUILTIN_TYPE(_type) EX_DECL_LUA_BUILTIN_TYPE_2(_type,_type)
#define EX_DECL_LUA_BUILTIN_TYPE_2(_typename,_type) \
    typedef struct _typename##_proxy_t { \
        strid_t typeid; \
        bool readonly; \
        _type val; \
    } _typename##_proxy_t; \
    extern _typename##_proxy_t *ex_lua_push##_typename ( struct lua_State *_l, bool _readonly ); \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// EX_DECL_LUA_BUILTIN_CLASS, EX_DECL_LUA_BUILTIN_CLASS_2
#define EX_DECL_LUA_BUILTIN_CLASS(_type) EX_DECL_LUA_BUILTIN_TYPE_2(_type,_type)
#define EX_DECL_LUA_BUILTIN_CLASS_2(_typename,_type) \
    typedef struct _typename##_proxy_t { \
        strid_t typeid; \
        bool readonly; \
        _type *val; \
    } _typename##_proxy_t; \
    extern _typename##_proxy_t *ex_lua_push##_typename ( struct lua_State *_l, bool _readonly ); \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// EX_DECL_LUA_BUILTIN_REF, EX_DECL_LUA_BUILTIN_CLASS_2
#define EX_DECL_LUA_BUILTIN_REF(_type) EX_DECL_LUA_BUILTIN_CLASS_2(_type,_type)
#define EX_DECL_LUA_BUILTIN_REF_2(_typename,_type) \
    extern ref_proxy_t *ex_lua_push##_typename ( struct lua_State *_l, bool _readonly ); \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern ex_ref_t *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern ex_ref_t *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// defines
EX_DECL_LUA_BUILTIN_TYPE_2(vec2f,ex_vec2f_t)

EX_DECL_LUA_BUILTIN_REF_2(object,ex_object_t)
    EX_DECL_LUA_BUILTIN_REF_2(world,ex_world_t)
    EX_DECL_LUA_BUILTIN_REF_2(entity,ex_entity_t)

// undef macros
#undef EX_DECL_LUA_BUILTIN_TYPE
#undef EX_DECL_LUA_BUILTIN_TYPE_2
#undef EX_DECL_LUA_BUILTIN_CLASS
#undef EX_DECL_LUA_BUILTIN_CLASS_2
#undef EX_DECL_LUA_BUILTIN_REF
#undef EX_DECL_LUA_BUILTIN_REF_2

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_LUA_BUILTIN_REF(_type,_typename,_lua_typename) \
    static ex_hashmap_t __key_to_type_meta_getset; \
    static ex_hashmap_t __key_to_meta_getset; \
    static const char *__typename = _lua_typename; \
    static int __type_meta_newindex ( lua_State *_l ) { \
        return ex_lua_type_meta_newindex( _l, &__key_to_type_meta_getset ); \
    } \
    static int __type_meta_index ( lua_State *_l ) { \
        return ex_lua_type_meta_index( _l, &__key_to_type_meta_getset ); \
    } \
    static int __meta_newindex ( lua_State *_l ) { \
        return ex_lua_meta_newindex( _l, &__key_to_meta_getset ); \
    } \
    static int __meta_index ( lua_State *_l ) { \
        return ex_lua_meta_index( _l, &__key_to_meta_getset ); \
    } \
    static int __child_meta_newindex ( lua_State *_l ) { \
        return ex_lua_child_meta_newindex( _l, &__key_to_meta_getset ); \
    } \
    static int __child_meta_index ( lua_State *_l ) { \
        return ex_lua_child_meta_index( _l, &__key_to_meta_getset ); \
    } \
    ref_proxy_t *ex_lua_push##_typename ( lua_State *_l, bool _readonly ) { \
        ref_proxy_t *u; \
        luaL_newmetatable( _l, __typename ); /* NOTE: this find a table in LUA_REGISTRYINDEX */ \
        u = ex_lua_pushref ( _l, lua_gettop(_l), _readonly ); \
        lua_remove(_l,-2); \
        return u; \
    } \
    bool ex_lua_is##_typename ( lua_State *_l, int _idx ) { \
        if ( ex_lua_isref(_l,_idx) ) { \
            ex_ref_t *r = ex_lua_toref(_l,_idx); \
            if ( ex_isa( _type, r->ptr ) ) \
                return true; \
        } \
        return false; \
    } \
    ex_ref_t *ex_lua_to##_typename ( lua_State *_l, int _idx ) { \
        return ex_lua_toref(_l,_idx); \
    } \
    ex_ref_t *ex_lua_check##_typename ( lua_State *_l, int _idx ) { \
        if ( ex_lua_is##_typename(_l,_idx) == false ) { \
            luaL_error( _l, "invalid parameter type, expected %s", __typename ); \
        } \
        return ex_lua_to##_typename(_l,_idx); \
    }

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

extern bool ex_lua_is_readonly ( struct lua_State *_l, int _idx );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ref_proxy_t *ex_lua_pushref ( struct lua_State *_l, int _meta_idx, bool _readonly );
extern bool ex_lua_isref ( struct lua_State *_l, int _idx );
extern ex_ref_t *ex_lua_toref ( struct lua_State *_l, int _idx );
extern ex_ref_t *ex_lua_checkref ( struct lua_State *_l, int _idx );
extern void ex_lua_check_nullref ( struct lua_State *_l, ex_ref_t *_ref );

extern int ex_lua_ref_gc ( struct lua_State *_l );
extern int ex_lua_ref_tostring ( struct lua_State *_l );
extern int ex_lua_ref_eq ( struct lua_State *_l );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_register_builtin ( struct lua_State *_l, 
                                     const char *_field, 
                                     const char *_typename, 
                                     const void *_meta_funcs,
                                     const void *_type_meta_funcs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_register_class ( struct lua_State *_l, 
                                   const char *_field, 
                                   const char *_supername, 
                                   const char *_typename, 
                                   const void *_meta_funcs,
                                   const void *_type_meta_funcs,
                                   const void *_metacall_for_child );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_WRAP_HELPER_H_1297139559
// #################################################################################
