// ======================================================================================
// File         : test_rtti.c
// Author       : Wu Jie 
// Last Change  : 11/29/2010 | 13:30:03 PM | Monday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../runtime/runtime_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

//
typedef struct test_cls_t {
    const ex_object_t _; // base class
    uint32  id;
    float   data1;
    float   data2;
} test_cls_t;

// why not use strid_t ???
// because if use strid_t for the property name, we can't define it in
// compile-time, but the array below is store in compile time.
static const ex_prop_t _props[] = {
    { "id",     EX_PROP_ATTR_READ_ONLY, offsetof(struct test_cls_t, id),    ex_prop_set_raw_int32, ex_prop_get_raw_int32 }
  , { "data1",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data1), ex_prop_set_raw_float, ex_prop_get_raw_float }
  , { "data2",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data2), ex_prop_set_raw_float, ex_prop_get_raw_float }
}

static strid_t __CLASSID_test_cls_t
#define EX_CLASSID(_name) __CLASSID_##_name

//
static void register_test_cls_t () {
    ex_rtti_t* rtti = ex_rtti_register_class ( "test_cls_t", "ex_object_t" );
    if ( rtti ) {
        EX_CLASSID(test_cls_t) = rtti->_classid;
        rtti->props = _props;
    }
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
    // TODO:
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_rtti ()
{
    normal();
}

