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

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#if 0
//
typedef struct test_cls_t {
    const ex_class_t _; // base class
    uint32  id;
    float   data1;
    float   data2;
} test_cls_t;

// why not use strid_t ???
// because if use strid_t for the property name, we can't define it in
// compile-time, but the array below is store in compile time.
static const ex_prop_t __PROPS_test_cls_t[] = {
    { "id",     EX_PROP_ATTR_READ_ONLY, offsetof(struct test_cls_t, id),    ex_prop_set_raw_int32, ex_prop_get_raw_int32 }
  , { "data1",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data1), ex_prop_set_raw_float, ex_prop_get_raw_float }
  , { "data2",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data2), ex_prop_set_raw_float, ex_prop_get_raw_float }
};

static strid_t __RTTI_test_cls_t;

//
void __ex_register_test_cls_t () {
    __RTTI_test_cls_t = ex_rtti_register_class ( "test_cls_t", "ex_class_t" );
    ex_rtti_register_properties ( __RTTI_test_cls_t, 
                                  __PROPS_test_cls_t, 
                                  EX_ARRAY_COUNT(__PROPS_test_cls_t)-1 ); \
} 
#else

EX_DEF_CLASS_BEGIN(test_cls_t)
    uint32  id;
    float   data1;
    float   data2;
EX_DEF_CLASS_END(test_cls_t)

EX_DEF_PROPS_BEGIN(test_cls_t)
    EX_PROP( test_cls_t, id,    "id",    EX_PROP_ATTR_READ_ONLY, ex_prop_set_raw_int32, ex_prop_get_raw_int32 )
    EX_PROP( test_cls_t, data1, "data1", EX_PROP_ATTR_NONE,      ex_prop_set_raw_float, ex_prop_get_raw_float )
    EX_PROP( test_cls_t, data2, "data2", EX_PROP_ATTR_NONE,      ex_prop_set_raw_float, ex_prop_get_raw_float )
EX_DEF_PROPS_END(test_cls_t)


#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
	EX_REGISTER_CLASS(test_cls_t);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_rtti ()
{
    normal();
}

