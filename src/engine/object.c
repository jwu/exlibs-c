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
    0, // refcount

    NULL, // init
    NULL, // deinit
    NULL, // reset
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_object_t)
    EX_PROP( ex_object_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
    EX_PROP( ex_object_t, int, refcount, "refcount",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_object_t)
    EX_MEMBER_SERIALIZE(uid,uid)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_object_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
    EX_MEMBER_TOSTRING( int, "refcount", self->refcount )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////
