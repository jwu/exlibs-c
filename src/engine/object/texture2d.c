// ======================================================================================
// File         : texture2d.c
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 00:14:45 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "texture2d.h"
#include "../misc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __texture_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture2d_init ( ex_ref_t *_self ) {
    __texture_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __texture_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture2d_deinit ( ex_ref_t *_self ) {
    ex_texture2d_t *self;

    self = EX_REF_CAST(ex_texture2d_t,_self);
    if ( self->data != NULL ) {
        ex_free( self->data );
        self->data = NULL;
    }

    __texture_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_texture2d_t)

    EX_TEXTURE2D_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Texture2D") )
    EX_MEMBER( ex_object_t, init, __texture2d_init )
    EX_MEMBER( ex_object_t, deinit, __texture2d_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_texture2d_t)
    EX_PROP( ex_texture2d_t, int, mipmapCount, "Mipmap Count", EX_PROP_ATTR_NONE )
    EX_PROP( ex_texture2d_t, int, format, "Format", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_texture2d_t,ex_texture_t)
    EX_MEMBER_SERIALIZE( int, mipmapCount )
    EX_MEMBER_SERIALIZE( int, format )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_texture2d_t,ex_texture_t)
    EX_MEMBER_TOSTRING( int, "mipmapCount", self->mipmapCount )
    EX_MEMBER_TOSTRING( int, "format", self->format )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// create functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_create_texture2d ( int _width, int _height, int _fmt ) {
    return ex_create_texture2d_by_uid ( _width, _height, _fmt, ex_generate_uid() );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_create_texture2d_by_uid ( int _width, int _height, int _fmt, ex_uid_t _uid ) {
    ex_ref_t *ref;
    int bytes;

    ref = ex_create_object( EX_RTTI(ex_texture2d_t), _uid );
    EX_REF_CAST(ex_object_t,ref)->init(ref);

    EX_REF_CAST(ex_texture_t,ref)->width = _width;
    EX_REF_CAST(ex_texture_t,ref)->height = _height;

    bytes = ex_texture_fmt_to_bytes(_fmt);
    EX_REF_CAST(ex_texture2d_t,ref)->format = _fmt;
    EX_REF_CAST(ex_texture2d_t,ref)->data = ex_malloc ( _width * _height * bytes );

    return ref;
}

///////////////////////////////////////////////////////////////////////////////
// member functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_texture2d_apply ( ex_ref_t *_self ) {
    ex_texture2d_t *self;
    GLenum gl_fmt, gl_type;

    self = EX_REF_CAST( ex_texture2d_t, _self );

    // ======================================================== 
    // generate a texture handle
    // ======================================================== 

    if ( ((ex_texture_t *)self)->textureID == -1 ) {
        glGenTextures ( 1, &(((ex_texture_t *)self)->textureID) );
    }
    glBindTexture ( GL_TEXTURE_2D, ((ex_texture_t *)self)->textureID );

    // ======================================================== 
    // get proper format for tex-image2d
    // ======================================================== 

    switch ( self->format ) {
    case EX_TEXTURE_FORMAT_ALPHA8:
        gl_fmt = GL_ALPHA;
        gl_type = GL_UNSIGNED_BYTE;
        break;

    case EX_TEXTURE_FORMAT_ARGB4444:
        gl_fmt = GL_RGBA;
        gl_type = GL_UNSIGNED_SHORT_4_4_4_4;
        break;

    case EX_TEXTURE_FORMAT_RGB24:
        gl_fmt = GL_RGB;
        gl_type = GL_UNSIGNED_BYTE;
        break;

    case EX_TEXTURE_FORMAT_ARGB32:
        gl_fmt = GL_RGBA;
        gl_type = GL_UNSIGNED_BYTE;
        break;

    case EX_TEXTURE_FORMAT_RGB565:
        gl_fmt = GL_RGB;
        gl_type = GL_UNSIGNED_SHORT_5_6_5;
        break;

    // case EX_TEXTURE_FORMAT_DXT1:
    // case EX_TEXTURE_FORMAT_DXT5:
    // case EX_TEXTURE_FORMAT_PVRTC_RGB2:
    // case EX_TEXTURE_FORMAT_PVRTC_RGBA2:
    // case EX_TEXTURE_FORMAT_PVRTC_RGB4:
    // case EX_TEXTURE_FORMAT_PVRTC_RGBA4:
    // case EX_TEXTURE_FORMAT_ETC_RGB4:

    default:
        gl_fmt = GL_RGBA;
        gl_type = GL_UNSIGNED_BYTE;
        break;
    }

    // ======================================================== 
    // generate mip map
    // ======================================================== 

    if ( self->mipmapCount > 0 ) {
        // TODO: generate mip map
    }

    // ======================================================== 
    // generate texture
    // ======================================================== 

    glTexImage2D( GL_TEXTURE_2D, 
                  0, 
                  gl_fmt, 
                  ((ex_texture_t *)self)->width, 
                  ((ex_texture_t *)self)->height, 
                  0, 
                  gl_fmt, 
                  gl_type, 
                  self->data );

    // ======================================================== 
    // set texture filter mode
    // ======================================================== 

    switch ( ((ex_texture_t *)self)->filterMode ) {
    case EX_FILTER_MODE_POINT:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        break;

    case EX_FILTER_MODE_BILINEAR:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        break;

    // TODO { 
    // case EX_FILTER_MODE_TRILINEAR:
    //     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    //     break;
    // } TODO end 

    default:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        break;
    }

    // ======================================================== 
    // set texture wrap mode
    // ======================================================== 

    switch ( ((ex_texture_t *)self)->wrapMode ) {
    case EX_TEXTURE_WRAP_MODE_REPEAT:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        break;

    case EX_TEXTURE_WRAP_MODE_CLAMP:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        break;

    default:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        break;
    }

    // finish apply
    glBindTexture( GL_TEXTURE_2D, 0 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_texture2d_set_readable ( ex_ref_t *_self, bool _isReadable ) {
    ex_texture2d_t *self;

    self = EX_REF_CAST( ex_texture2d_t, _self );
    if ( self->isReadable != _isReadable ) {
        self->isReadable = _isReadable;

        if ( self->isReadable ) {
            int bytes;

            ex_assert ( self->data == NULL, "data should be NULL, this may cause memory leak." );
            bytes = ex_texture_fmt_to_bytes(self->format);
            self->data = ex_malloc ( ((ex_texture_t *)self)->width * ((ex_texture_t *)self)->height * bytes );
        }
        else {
            ex_assert_return ( self->data, /*dummy*/, "data should exists." );

            ex_free(self->data);
            self->data = NULL;
        }
    }
}
