// ======================================================================================
// File         : prop.h
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 17:34:31 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef PROP_H_1290764072
#define PROP_H_1290764072
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// EXAMPLE: 
// ex_prop_t[] props = {
//     { "value1", EX_PROP_ATTR_NONE, NULL, ex_prop_set_raw_int8, NULL, ex_prop_get_raw_int8 }
//   , { "value2", EX_PROP_ATTR_READ_ONLY, NULL, ex_prop_set_raw_float, NULL, ex_prop_get_raw_float }
// }

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: ex_prop_t
// ------------------------------------------------------------------ 

typedef struct ex_prop_t {
    const char* _name;  // NOTE: we not use ex_strid_t, cause property defined before ex_strid_t initialize.
    uint32 _attrs;      // attribute flags
    void (*set_property) ( ex_object_t* _pObj, ex_strid_t* _propID, const void* _value );
    void (*set_raw) ( void* _raw_addr, const void* _value );
    void (*get_property) ( ex_object_t* _pObj, ex_strid_t* _propID, void* _value );
    void (*get_raw) ( void* _raw_addr, void* _value );
} ex_prop_t;

// ------------------------------------------------------------------ 
// Desc: flags
// ------------------------------------------------------------------ 

#define EX_PROP_ATTR_NONE           0x00000000
#define EX_PROP_ATTR_READ_ONLY      0x00000001
#define EX_PROP_ATTR_HIDE           0x00000002
#define EX_PROP_ATTR_NO_SERIALIZE   0x00000004
#define EX_PROP_ATTR_EDITOR_ONLY    0x00000008

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int8 ( void* _raw_addr, const void* _value ) { *(int8*)_raw_addr = *(const int8*)_value; }
inline void ex_prop_get_raw_int8 ( void* _raw_addr, void* _value ) {  *(int8*)_value = *(int8*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int16 ( void* _raw_addr, const void* _value ) { *(int16*)_raw_addr = *(const int16*)_value; }
inline void ex_prop_get_raw_int16 ( void* _raw_addr, void* _value ) {  *(int16*)_value = *(int16*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int32 ( void* _raw_addr, const void* _value ) { *(int32*)_raw_addr = *(const int32*)_value; }
inline void ex_prop_get_raw_int32 ( void* _raw_addr, void* _value ) {  *(int32*)_value = *(int32*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int64 ( void* _raw_addr, const void* _value ) { *(int64*)_raw_addr = *(const int64*)_value; }
inline void ex_prop_get_raw_int64 ( void* _raw_addr, void* _value ) {  *(int64*)_value = *(int64*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint8 ( void* _raw_addr, const void* _value ) { *(uint8*)_raw_addr = *(const uint8*)_value; }
inline void ex_prop_get_raw_uint8 ( void* _raw_addr, void* _value ) {  *(uint8*)_value = *(uint8*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint16 ( void* _raw_addr, const void* _value ) { *(uint16*)_raw_addr = *(const uint16*)_value; }
inline void ex_prop_get_raw_uint16 ( void* _raw_addr, void* _value ) {  *(uint16*)_value = *(uint16*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint32 ( void* _raw_addr, const void* _value ) { *(uint32*)_raw_addr = *(const uint32*)_value; }
inline void ex_prop_get_raw_uint32 ( void* _raw_addr, void* _value ) {  *(uint32*)_value = *(uint32*)_raw_addr; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint64 ( void* _raw_addr, const void* _value ) { *(uint64*)_raw_addr = *(const uint64*)_value; }
inline void ex_prop_get_raw_uint64 ( void* _raw_addr, void* _value ) {  *(uint64*)_value = *(uint64*)_raw_addr; }

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END PROP_H_1290764072
// #################################################################################


