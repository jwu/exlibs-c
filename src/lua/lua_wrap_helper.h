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
        _type val; \
    } _typename##_proxy_t; \
    extern _typename##_proxy_t *ex_lua_push##_typename ( struct lua_State *_l ); \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// EX_DECL_LUA_BUILTIN_CLASS, EX_DECL_LUA_BUILTIN_CLASS_2
#define EX_DECL_LUA_BUILTIN_CLASS(_type) EX_DECL_LUA_BUILTIN_TYPE_2(_type,_type)
#define EX_DECL_LUA_BUILTIN_CLASS_2(_typename,_type) \
    typedef struct _typename##_proxy_t { \
        strid_t typeid; \
        _type *val; \
    } _typename##_proxy_t; \
    extern _typename##_proxy_t *ex_lua_push##_typename ( struct lua_State *_l ); \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern _type *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// EX_DECL_LUA_BUILTIN_REF, EX_DECL_LUA_BUILTIN_CLASS_2
#define EX_DECL_LUA_BUILTIN_REF(_type) EX_DECL_LUA_BUILTIN_CLASS_2(_type,_type)
#define EX_DECL_LUA_BUILTIN_REF_2(_typename,_type) \
    extern bool ex_lua_is##_typename ( struct lua_State *_l, int _idx ); \
    extern ex_ref_t *ex_lua_to##_typename ( struct lua_State *_l, int _idx ); \
    extern ex_ref_t *ex_lua_check##_typename ( struct lua_State *_l, int _idx );

// defines
EX_DECL_LUA_BUILTIN_TYPE_2(angf,ex_angf_t)
EX_DECL_LUA_BUILTIN_TYPE_2(vec2f,ex_vec2f_t)
EX_DECL_LUA_BUILTIN_TYPE_2(mat33f,ex_mat33f_t)
EX_DECL_LUA_BUILTIN_TYPE_2(color3f,ex_color3f_t)
EX_DECL_LUA_BUILTIN_TYPE_2(color4f,ex_color4f_t)

EX_DECL_LUA_BUILTIN_REF_2(object,ex_object_t)
    EX_DECL_LUA_BUILTIN_REF_2(world,ex_world_t)
    EX_DECL_LUA_BUILTIN_REF_2(texture,ex_texture_t)
        EX_DECL_LUA_BUILTIN_REF_2(texture2d,ex_texture2d_t)
    EX_DECL_LUA_BUILTIN_REF_2(entity,ex_entity_t)
    EX_DECL_LUA_BUILTIN_REF_2(component,ex_component_t)
        EX_DECL_LUA_BUILTIN_REF_2(trans2d,ex_trans2d_t)
        EX_DECL_LUA_BUILTIN_REF_2(behavior,ex_behavior_t)
            EX_DECL_LUA_BUILTIN_REF_2(lua_behavior,ex_lua_behavior_t)

// undef macros
#undef EX_DECL_LUA_BUILTIN_TYPE
#undef EX_DECL_LUA_BUILTIN_TYPE_2
#undef EX_DECL_LUA_BUILTIN_CLASS
#undef EX_DECL_LUA_BUILTIN_CLASS_2
#undef EX_DECL_LUA_BUILTIN_REF
#undef EX_DECL_LUA_BUILTIN_REF_2

// ------------------------------------------------------------------ 
// Desc: EX_DEF_LUA_BUILTIN_REF
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
    static int __##_typename##_get_null ( lua_State *_l ) { \
        ex_lua_newobject(_l,__typename); \
        return 1; \
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

#define EX_DEF_LUA_BUILTIN_TYPE(_type,_typename,_lua_typename) \
    extern int __lua_index( lua_State *, int ); \
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
    bool ex_lua_is##_typename ( lua_State *_l, int _idx ) { \
        _typename##_proxy_t *u; \
        if ( lua_isuserdata(_l, _idx) == 0 ) \
            return false; \
        u = (_typename##_proxy_t *)lua_touserdata(_l,_idx); \
        if ( u->typeid != EX_TYPEID(_typename) ) \
            return false; \
        return true; \
    } \
    _type *ex_lua_to##_typename ( lua_State *_l, int _idx ) { \
        _typename##_proxy_t *u = (_typename##_proxy_t *)lua_touserdata(_l,_idx); \
        return &(u->val); \
    } \
    _type *ex_lua_check##_typename ( lua_State *_l, int _idx ) { \
        if ( ex_lua_is##_typename(_l,_idx) == false ) \
            luaL_error( _l, "invalid parameter type, expected %s", __typename ); \
        return ex_lua_to##_typename(_l,_idx); \
    } \
    static _typename##_proxy_t *__push##_typename ( lua_State *_l, int _meta_idx ) { \
        _typename##_proxy_t *u; \
        int meta_idx = __lua_index(_l,_meta_idx); \
        u = (_typename##_proxy_t *)lua_newuserdata(_l, sizeof(_typename##_proxy_t)); \
        u->typeid = EX_TYPEID(_typename); \
        lua_pushvalue(_l,meta_idx); \
        lua_setmetatable(_l,-2); \
        return u; \
    } \
    _typename##_proxy_t *ex_lua_push##_typename ( lua_State *_l ) { \
        _typename##_proxy_t *u; \
        luaL_newmetatable( _l, __typename ); /*NOTE: this find a table in LUA_REGISTRYINDEX*/ \
        u = __push##_typename ( _l, -1 ); \
        lua_remove(_l,-2); \
        return u; \
    } \
    static int __##_typename##_tostring ( lua_State *_l ) { \
        ex_string_t *str = ex_string_alloc("",256); \
        _type *v = ex_lua_check##_typename(_l,1); \
        EX_RTTI(_typename)->tostring(str, v); \
        lua_pushstring(_l, str->text); \
        ex_string_free(str); \
        return 1; \
    } \
    static int __##_typename##_concat ( lua_State *_l ) { \
        const char *lhs = luaL_checkstring(_l, 1); \
        ex_string_t *str = ex_string_alloc(lhs,256); \
        _type *rhs = ex_lua_check##_typename(_l,2); \
        /*push*/ \
        EX_RTTI(_typename)->tostring(str, rhs); \
        lua_pushstring(_l, str->text); \
        ex_string_free(str); \
        return 1; \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_LUA_BUILTIN_MODULE() \
    static ex_hashmap_t __key_to_type_meta_getset; \
    static int __type_meta_newindex ( lua_State *_l ) { \
        return ex_lua_type_meta_newindex( _l, &__key_to_type_meta_getset ); \
    } \
    static int __type_meta_index ( lua_State *_l ) { \
        return ex_lua_type_meta_index( _l, &__key_to_type_meta_getset ); \
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

extern ref_proxy_t *ex_lua_pushref ( struct lua_State *_l, int _meta_idx );
extern bool ex_lua_isref ( struct lua_State *_l, int _idx );
extern ex_ref_t *ex_lua_toref ( struct lua_State *_l, int _idx );
extern ex_ref_t *ex_lua_checkref ( struct lua_State *_l, int _idx );
extern void ex_lua_check_nullref ( struct lua_State *_l, ex_ref_t *_ref );

extern int ex_lua_ref_gc ( struct lua_State *_l );
extern int ex_lua_ref_tostring ( struct lua_State *_l );
extern int ex_lua_ref_concat ( struct lua_State *_l );
extern int ex_lua_ref_eq ( struct lua_State *_l );

//
extern void ex_lua_pushobject ( struct lua_State *_l, ex_ref_t *_self );

// NOTE: unlike ref, generic will create an user-data and apply proper metatable on it
extern ref_proxy_t *ex_lua_newobject ( struct lua_State *_l, const char *_lua_typename );
extern ref_proxy_t *ex_lua_newluabehavior ( struct lua_State *_l, const char *_lua_typename );

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
