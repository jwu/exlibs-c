// ======================================================================================
// File         : object.c
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 14:10:24 PM | Friday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_object_t)
    EX_UID_INVALID, // uid
    EX_STRID_NULL, // name
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_object_t)
    EX_PROP( ex_object_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
    EX_PROP( ex_object_t, strid, name, "name",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_object_t)
    EX_MEMBER_SERIALIZE(uid,uid)
    EX_MEMBER_SERIALIZE(strid,name)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_object_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
    EX_MEMBER_TOSTRING( strid, "name", self->name )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_object_init ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    obj->uid = ex_generate_uid();
    obj->name = ex_strid("New Object");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_object_deinit ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    obj->name = EX_STRID_NULL;
    obj->uid = EX_UID_INVALID;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_uid_t ex_object_uid ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    return obj->uid;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_object_name ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    return obj->name;
}