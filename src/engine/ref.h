// ======================================================================================
// File         : ref.h
// Author       : Wu Jie 
// Last Change  : 01/29/2011 | 09:32:47 AM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef REF_H_1296264775
#define REF_H_1296264775
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_ref_t {
    int refcount;
    void *ptr;
} ex_ref_t; 

///////////////////////////////////////////////////////////////////////////////
// function define
///////////////////////////////////////////////////////////////////////////////

extern ex_ref_t *ex_newref ( void *_obj );
extern void ex_delref ( ex_ref_t *_ref );

extern int ex_incref ( ex_ref_t *_ref );
extern int ex_decref ( ex_ref_t *_ref );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: the debug version will try to cast the ptr by type and report error.
// ------------------------------------------------------------------ 

// this is for performance quick test, in release version, the ex_cast already optimize to static cast. 
#if 1
    #define EX_REF_CAST(_type,_ref) ((_ref && _ref->ptr) ? ex_cast(_type,(_ref)->ptr) : NULL)
#else
    #define EX_REF_CAST(_type,_ref) ((_type *)((_ref)->ptr))
#endif

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: cast may return NULL
// ------------------------------------------------------------------ 

#define EX_REF_AS(_type,_ref) ex_as(_type,(_ref)->ptr)

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END REF_H_1296264775
// #################################################################################
