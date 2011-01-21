// ======================================================================================
// File         : serialize_macro.h
// Author       : Wu Jie 
// Last Change  : 01/16/2011 | 22:47:15 PM | Sunday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef SERIALIZE_MACRO_H_1295189236
#define SERIALIZE_MACRO_H_1295189236
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_BEGIN(_class) \
    void __ex_serialize_##_class( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        _class *self = (_class *)_val; \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_class) ); \
            if ( ret != 0 ) return; \
        } \
        if ( _stream->push ) _stream->push(_stream);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_BEGIN_SUPER(_class,_super) \
    void __ex_serialize_##_class( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        void (*tmp_pop) ( ex_stream_t *_stream ); \
        tmp_pop = _stream->pop; \
        _stream->pop = NULL; /*this make the super serialize no pop*/ \
        __ex_serialize_##_super(_stream,_name,_val); \
        _stream->pop = tmp_pop;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE(_type,_member) \
        __ex_serialize_##_type(_stream,ex_strid(#_member),&(self->_member));

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_ARRAY(_el_type,_member) \
        __ex_serialize_array_2(_stream, \
                               ex_strid(#_member), \
                               self->_member, /*NOTE: array always appear as pointer*/ \
                               __ex_serialize_##_el_type);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_MAP(_key_type,_val_type,_member) \
        __ex_serialize_map_2(_stream, \
                             ex_strid(#_member), \
                             self->_member, /*NOTE: map always appear as pointer*/ \
                             __ex_serialize_##_key_type, \
                             __ex_serialize_##_val_type);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SERIALIZE_END \
        if ( _stream->pop ) _stream->pop(_stream); \
        (void *)self; /*to avoid unused compile warning*/ \
    }

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END SERIALIZE_MACRO_H_1295189236
// #################################################################################


