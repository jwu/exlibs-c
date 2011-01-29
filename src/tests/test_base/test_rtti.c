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
static const ex_prop_t __PROPS_test_cls_t__[] = {
    { "id",     EX_PROP_ATTR_READ_ONLY, offsetof(struct test_cls_t, id),    ex_prop_set_raw_int32, ex_prop_get_raw_int32 }
  , { "data1",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data1), ex_prop_set_raw_float, ex_prop_get_raw_float }
  , { "data2",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data2), ex_prop_set_raw_float, ex_prop_get_raw_float }
};

static strid_t __RTTI_test_cls_t__;

//
void __ex_register_test_cls_t () {
    __RTTI_test_cls_t__ = ex_rtti_register_class ( ex_strid("test_cls_t"), "ex_class_t" );
    ex_rtti_register_properties ( __RTTI_test_cls_t__, 
                                  __PROPS_test_cls_t__, 
                                  EX_ARRAY_COUNT(__PROPS_test_cls_t__)-1 ); \
} 
#else

EX_DECL_CLASS_BEGIN(test_cls_t)
    uint32  id;
    float   data1;
    float   data2;
EX_DECL_CLASS_END(test_cls_t)

EX_DEF_CLASS_BEGIN(test_cls_t)
    EX_MEMBER(test_cls_t, id, -1)
    EX_MEMBER(test_cls_t, data1, 1.0f)
    EX_MEMBER(test_cls_t, data2, 1.0f)
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(test_cls_t)
    EX_PROP( test_cls_t, int32, id,    "id",    EX_PROP_ATTR_READ_ONLY )
    EX_PROP( test_cls_t, float, data1, "data1", EX_PROP_ATTR_NONE )
    EX_PROP( test_cls_t, float, data2, "data2", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(test_cls_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(test_cls_t)
EX_DEF_TOSTRING_END

#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
	EX_REGISTER_CLASS(test_cls_t);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_rtti () {
    normal();
}

