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

///////////////////////////////////////////////////////////////////////////////
// class define
// NOTE: the Resource/Asset can benefit in using refcount
// NOTE: the AI usually would like to immediatly invalid an ref
// please learn Py_INCREF, Py_DECREF, Py_XDECREF for more details.
///////////////////////////////////////////////////////////////////////////////

EX_DECL_CLASS_BEGIN(ex_ref_t)
    ex_uid_t uid;
    void *ptr;
    int *refcount;
EX_DECL_CLASS_END(ex_ref_t)

///////////////////////////////////////////////////////////////////////////////
// function define
///////////////////////////////////////////////////////////////////////////////

extern ex_ref_t *ex_newref ( ex_uid_t _uid );

extern void ex_incref ( ex_ref_t *_ref );
extern int ex_decref ( ex_ref_t *_ref );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END REF_H_1296264775
// #################################################################################