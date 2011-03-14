// ======================================================================================
// File         : runtime_macro.h
// Author       : Wu Jie 
// Last Change  : 01/22/2011 | 13:46:37 PM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef RUNTIME_MACRO_H_1295675198
#define RUNTIME_MACRO_H_1295675198
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// NOTE: to define a rtti class you need:
/**
  suppose you want to define a foo class
  EX_DECL_CLASS_BEGIN(foo)
      int val1;
  EX_DECL_CLASS_END(foo)

  EX_DEF_CLASS_BEGIN(foo)
    EX_MEMBER(foo,val1,10)
  EX_DEF_CLASS_END

  EX_DEF_PROPS_BEGIN(foo)
      EX_PROP( foo, int32, val1, "value_01",  EX_PROP_ATTR_NONE )
  EX_DEF_PROPS_END

  EX_SERIALIZE_BEGIN(foo)
      EX_MEMBER_SERIALIZE(int,val1);
  EX_SERIALIZE_END

  EX_DEF_TOSTRING_BEGIN(foo)
      EX_MEMBER_TOSTRING(int,"val",self->val);
  EX_DEF_TOSTRING_END

  finally register in classes_registry
  EX_REGISTER_CLASS(foo);
*/

///////////////////////////////////////////////////////////////////////////////
// rtti help macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_RTTI(_typename) (__RTTI_##_typename##__)
#define EX_TYPEID(_typename) (__TYPEID_##_typename##__)

// ex_instanceof is strong compare, it must be the class, can't be its child or parent
// _obj_ptr->type == _type
#define ex_instanceof(_type,_obj_ptr) __ex_instanceof((ex_class_t *)_obj_ptr,EX_RTTI(_type))

// ex_childof gets the _obj_ptr's parent, parent's parent ... for compare ( *not* including itself )
// _obj_ptr->parent->...->parent->type == _type
#define ex_childof(_type,_obj_ptr) __ex_childof((ex_class_t *)_obj_ptr,EX_RTTI(_type))

// ex_superof gets the _type's parent, parent's parent ... for compare ( *not* including _type itself )
// _type->parent->...->parent->type == _type
#define ex_superof(_type,_obj_ptr) __ex_superof((ex_class_t *)_obj_ptr,EX_RTTI(_type))

// ex_isa equals to ex_instanceof || ex_childof
#define ex_isa(_type,_obj_ptr) __ex_isa((ex_class_t *)_obj_ptr,EX_RTTI(_type))

// ex_cast use ex_isa compare the type, if it is, then return the casted type, else report an error and return NULL.
// NOTE: ex_cast will use static cast in release version.
#ifdef EX_DEBUG
    #define ex_cast(_type,_obj_ptr) ((_type *)__ex_cast((ex_class_t *)(_obj_ptr),EX_RTTI(_type),false))
#else
    #define ex_cast(_type,_obj_ptr) ((_type *)(_obj_ptr))
#endif

// ex_as use ex_isa compare the type, if it is, then return the casted type.
#define ex_as(_type,_obj_ptr) ((_type *)__ex_cast((ex_class_t *)(_obj_ptr),EX_RTTI(_type),true))

///////////////////////////////////////////////////////////////////////////////
// class help macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_REGISTER_CLASS(_ctype) EX_REGISTER_CLASS_2(_ctype,_ctype)
#define EX_REGISTER_CLASS_2(_ctype,_typename) __ex_register_class_##_ctype(ex_strid(#_typename))

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_BEGIN(_typename) \
    typedef struct _typename { \
        const struct ex_class_t _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_END(_typename) \
    } _typename; \
    extern strid_t __TYPEID_##_typename##__; /*for EX_TYPEID*/ \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    extern void *__ex_create_##_typename(); \
    extern void __ex_register_properties_##_typename (); \
    extern void __ex_serialize_##_typename( struct ex_stream_t *, strid_t, void * ); \
    extern void __ex_tostring_##_typename( struct ex_string_t *, void * ); \
    static inline void __ex_register_class_##_typename ( strid_t _typeID ) { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = _typeID; \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          EX_RTTI(ex_class_t), \
                                                          sizeof(_typename), \
                                                          __ex_create_##_typename, \
                                                          __ex_serialize_##_typename, \
                                                          __ex_tostring_##_typename \
                                                          ); \
        __ex_register_properties_##_typename(); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_SUPER_BEGIN(_typename,_super) \
    typedef struct _typename { \
        const struct _super _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_SUPER_END(_typename,_super) \
    } _typename; \
    extern strid_t __TYPEID_##_typename##__; /*for EX_TYPEID*/ \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    extern void *__ex_create_##_typename(); \
    extern void __ex_register_properties_##_typename (); \
    extern void __ex_serialize_##_typename( struct ex_stream_t *, strid_t, void * ); \
    extern void __ex_tostring_##_typename( struct ex_string_t *, void * ); \
    static inline void __ex_register_class_##_typename ( strid_t _typeID ) { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = _typeID; \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          EX_RTTI(_super), \
                                                          sizeof(_typename), \
                                                          __ex_create_##_typename, \
                                                          __ex_serialize_##_typename, \
                                                          __ex_tostring_##_typename \
                                                          ); \
        __ex_register_properties_##_typename(); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_BEGIN(_typename) \
    strid_t __TYPEID_##_typename##__ = EX_STRID_NULL; \
    ex_rtti_t *__RTTI_##_typename##__ = NULL; \
    void *__ex_create_##_typename() { \
        void *__obj__ = ex_malloc(sizeof(_typename)); \
        ((ex_class_t *)__obj__)->rtti = EX_RTTI(_typename); \

#define EX_DEF_CLASS_END \
        return __obj__; \
    }

#define EX_MEMBER(_class_type,_member_type,_expr) \
    ((_class_type *)__obj__)->_member_type = _expr;

///////////////////////////////////////////////////////////////////////////////
// property help macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: flags
// ------------------------------------------------------------------ 

#define EX_PROP_ATTR_NONE           0x00000000
#define EX_PROP_ATTR_READ_ONLY      0x00000001
#define EX_PROP_ATTR_HIDE           0x00000002
#define EX_PROP_ATTR_NO_SERIALIZE   0x00000004
#define EX_PROP_ATTR_EDITOR_ONLY    0x00000008

// EXAMPLE { 
// ex_prop_t[] props = {
//     { "value1", EX_PROP_ATTR_NONE,      offsetof(struct foobar, value1), ex_prop_set_raw_int8,  ex_prop_get_raw_int8 }
//   , { "value2", EX_PROP_ATTR_READ_ONLY, offsetof(struct foobar, value2), ex_prop_set_raw_float, ex_prop_get_raw_float }
// }
//
// just show u how to set empty value for each item.
// { "unknown", 0, -1, NULL, NULL }
// } EXAMPLE end 

// // why not use strid_t ???
// // because if use strid_t for the property name, we can't define it in
// // compile-time, but the array below is store in compile time.
// static const ex_prop_t __PROPS_test_cls_t__[] = {
//     { "id",     EX_PROP_ATTR_READ_ONLY, offsetof(struct test_cls_t, id),    ex_prop_set_raw_int32, ex_prop_get_raw_int32 }
//   , { "data1",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data1), ex_prop_set_raw_float, ex_prop_get_raw_float }
//   , { "data2",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data2), ex_prop_set_raw_float, ex_prop_get_raw_float }
// };

// ------------------------------------------------------------------ 
// Desc: EX_DEF_PROPS_BEGIN(_typename)
// ------------------------------------------------------------------ 

#define EX_DEF_PROPS_BEGIN(_typename) \
    void __ex_register_properties_##_typename () { \
        ex_rtti_t *__rtti__ = EX_RTTI(_typename); \
        const char *__typename__ = #_typename; \
        static const ex_prop_t __props__[] = {

// ------------------------------------------------------------------ 
// Desc: EX_DEF_PROPS_END
// ------------------------------------------------------------------ 

#define EX_DEF_PROPS_END \
            { "", "", 0, -1, NULL, NULL } \
        }; /*end of __props__*/ \
        ex_assert_return( __rtti__, /**/, "failed to register class %s", __typename__ ); \
        ex_rtti_register_properties ( __rtti__, __props__, EX_ARRAY_COUNT(__props__)-1 ); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_PROP( _class_type, _member_type, _member, _prop_name, _attrs ) \
    { _prop_name, #_member_type, _attrs, offsetof(struct _class_type, _member), ex_prop_set_raw_##_member_type, ex_prop_get_raw_##_member_type },

#define EX_PROP_SET_GET( _class_type, _member_type, _member, _prop_name, _attrs, _set_func, _get_func ) \
    { _prop_name, #_member_type, _attrs, offsetof(struct _class_type, _member), _set_func, _get_func },

///////////////////////////////////////////////////////////////////////////////
// serialize help macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_BEGIN(_class) \
    void __ex_serialize_##_class( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        _class *self = (_class *)_val; \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, ex_rtti_info(_val)->typeID /*NOTE: this can avoid use base class' type when use SUPER*/ ); \
            if ( ret != 0 ) return; \
        } \
        if ( _stream->push ) _stream->push(_stream); \
        { /*NOTE: without this you can't declare variable*/

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_BEGIN_SUPER(_class,_super) \
    void __ex_serialize_##_class( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        void (*tmp_pop) ( ex_stream_t *_stream ); \
        _class *self = (_class *)_val; \
        tmp_pop = _stream->pop; \
        _stream->pop = NULL; /*this make the super serialize no pop*/ \
        __ex_serialize_##_super(_stream,_name,_val); \
        _stream->pop = tmp_pop; \
        { /*NOTE: without this you can't declare variable*/

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_END \
        } \
        if ( _stream->pop ) _stream->pop(_stream); \
        (void *)self; /*to avoid unused compile warning*/ \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE(_stream,_type,_name,_val_ptr) \
        __ex_serialize_##_type(_stream,ex_strid(_name),_val_ptr);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_STRING(_stream,_type,_name,_string_ptr) \
        __ex_serialize_string(_stream,ex_strid(_name),_string_ptr);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_ARRAY(_stream,_el_type,_name,_array_ptr) \
        __ex_serialize_array_2(_stream, ex_strid(_name), _array_ptr, __ex_serialize_##_el_type);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_MAP(_stream,_key_type,_val_type,_name,_map_ptr) \
        __ex_serialize_map_2(_stream, ex_strid(_name), _map_ptr, __ex_serialize_##_key_type, __ex_serialize_##_val_type );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_MEMBER_SERIALIZE(_type,_member) \
        __ex_serialize_##_type(_stream,ex_strid(#_member),&(self->_member));

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_MEMBER_SERIALIZE_STRING(_member) \
        __ex_serialize_string(_stream, \
                               ex_strid(#_member), \
                               self->_member /*NOTE: string always appear as pointer*/ \
                               );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_MEMBER_SERIALIZE_ARRAY(_el_type,_member) \
        __ex_serialize_array_2(_stream, \
                               ex_strid(#_member), \
                               self->_member, /*NOTE: array always appear as pointer*/ \
                               __ex_serialize_##_el_type);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_MEMBER_SERIALIZE_MAP(_key_type,_val_type,_member) \
        __ex_serialize_map_2(_stream, \
                             ex_strid(#_member), \
                             self->_member, /*NOTE: map always appear as pointer*/ \
                             __ex_serialize_##_key_type, \
                             __ex_serialize_##_val_type);

///////////////////////////////////////////////////////////////////////////////
// tostring help macros
///////////////////////////////////////////////////////////////////////////////

#define EX_DEF_TOSTRING_BEGIN(_type) \
    void __ex_tostring_##_type ( ex_string_t *_string, void *_val ) { \
        _type *self = (_type *)_val; \
        ex_string_cat( _string, #_type": {\n" );

#define EX_DEF_TOSTRING_SUPER_BEGIN(_type,_super) \
    void __ex_tostring_##_type ( ex_string_t *_string, void *_val ) { \
        _type *self = (_type *)_val; \
        ex_string_cat( _string, #_type": {\n" ); \
        __ex_tostring_##_super(_string,_val);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_TOSTRING_END \
        ex_string_cat( _string, "}\n" ); \
        (void *)self; \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_MEMBER_TOSTRING(_type,_name,_val) \
        ex_string_cat( _string, _name" " ); \
        __ex_tostring_##_type(_string,&(_val)); \
        ex_string_cat( _string, ",\n" );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END RUNTIME_MACRO_H_1295675198
// #################################################################################


