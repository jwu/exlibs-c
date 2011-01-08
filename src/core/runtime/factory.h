// ======================================================================================
// File         : factory.h
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 11:59:28 AM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef FACTORY_H_1294459170
#define FACTORY_H_1294459170
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "rtti.h"

///////////////////////////////////////////////////////////////////////////////
// declares pfn
///////////////////////////////////////////////////////////////////////////////

typedef void* (*ex_create_pfn_t) ();

///////////////////////////////////////////////////////////////////////////////
// declares functions
///////////////////////////////////////////////////////////////////////////////

extern void ex_factory_register ( strid_t _classID, ex_create_pfn_t _pfn );
extern void ex_factory_unregister ( strid_t _classID );
extern void* ex_factory_create ( strid_t _classID );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END FACTORY_H_1294459170
// #################################################################################


