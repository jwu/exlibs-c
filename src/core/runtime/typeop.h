// ======================================================================================
// File         : typeop.h
// Author       : Wu Jie 
// Last Change  : 01/21/2011 | 10:18:09 AM | Friday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TYPEOP_H_1295576290
#define TYPEOP_H_1295576290
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

static inline ex_create_pfn ex_create_func ( strid_t _typeID ) {
    ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) return rtti->create; 
    return NULL;
}
static inline ex_serialize_pfn ex_serialize_func ( strid_t _typeID ) {
    ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) return rtti->serialize; 
    return NULL;
}
static inline ex_tostring_pfn ex_tostring_func ( strid_t _typeID ) {
    ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) return rtti->tostring; 
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void *ex_create ( strid_t _typeID );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: this is good for generic progromming in lua-level, in c-level
// we still use EX_SERIALIZE that will call those static function for performance.
// ------------------------------------------------------------------ 

extern void ex_serialize ( strid_t _typeID, struct ex_stream_t *_stream, strid_t _name, void *_val );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: this is good for generic progromming in lua-level, in c-level
// we still use EX_TOSTRING that will call those static function for performance.
// ------------------------------------------------------------------ 

extern void ex_tostring ( strid_t _typeID, struct ex_string_t *_string, void *_val );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern size_t ex_sizeof ( strid_t _typeID );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TYPEOP_H_1295576290
// #################################################################################


