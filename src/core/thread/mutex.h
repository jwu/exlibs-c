// ======================================================================================
// File         : mutex.h
// Author       : Wu Jie 
// Last Change  : 07/10/2010 | 01:09:09 AM | Saturday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MUTEX_H_1278695351
#define MUTEX_H_1278695351
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct mutex_t mutex_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

mutex_t* mutex_create ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void mutex_destroy ( mutex_t* );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int mutex_lock ( mutex_t* );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int mutex_unlock ( mutex_t* );

// #################################################################################
#endif // END MUTEX_H_1278695351
// #################################################################################


