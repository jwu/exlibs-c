// ======================================================================================
// File         : stage.c
// Author       : Wu Jie 
// Last Change  : 05/28/2011 | 12:01:54 PM | Saturday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor_inc.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_stage_t)
    EX_STAGE_DEFAULT_MEMBER
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_stage_t)
    // EX_PROP( ex_stage_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_stage_t)
    // EX_SERIALIZE_ARRAY( _stream, ex_element_t, "elements", self->elements );
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_stage_t)
    // EX_MEMBER_TOSTRING( uid, "uid", self->uid )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_stage_t *ex_create_stage ( int _width, int _height ) {
    ex_stage_t *stage;
    // DISABLE { 
    // cairo_t *cr;
    // cairo_surface_t *surf;
    // uint8 *buffer;
    // } DISABLE end 

    stage = ex_malloc( sizeof(ex_stage_t) );
    stage->cr = NULL;
    stage->buffer = NULL;

    // DISABLE { 
    //
    // buffer = ex_malloc ( 4 * _width * _height * sizeof (uint8) );
    // ex_memzero( buffer, 4 * _width * _height * sizeof (uint8) );
    // surf = cairo_image_surface_create_for_data ( buffer,
    //                                              CAIRO_FORMAT_ARGB32,
    //                                              _width,
    //                                              _height,
    //                                              4 * _width );
    // if ( cairo_surface_status (surf) != CAIRO_STATUS_SUCCESS ) {
    //     ex_free (buffer);
    //     buffer = NULL;
    //     ex_error ("can't create cairo surface.");
    //     return NULL;
    // }

    // // create cairo context
    // cr = cairo_create (surf);
    // cairo_surface_destroy (surf);
    // if ( cairo_status (cr) != CAIRO_STATUS_SUCCESS ) {
    //     ex_free (buffer);
    //     buffer = NULL;
    //     ex_error ("can't create cairo context");
    //     return NULL;
    // }

    // //
    // stage->buffer = buffer;
    // stage->cr = cr;
    // } DISABLE end 

    return stage;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_stage ( ex_stage_t *_stage ) {
    // destroy cairo
    if ( _stage->cr ) {
        cairo_destroy (_stage->cr);
        _stage->cr = NULL;
    }

    //
    if ( _stage->buffer ) {
        ex_free (_stage->buffer);
        _stage->buffer = NULL;
    }

    //
    ex_free (_stage);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_stage_resize ( ex_stage_t *_stage, int _width, int _height ) {
    // DISABLE { 
    // cairo_t *cr;
    // cairo_surface_t *surf;
    // uint8 *buffer;

    // //
    // if ( _stage->buffer ) {
    //     ex_free (_stage->buffer);
    //     _stage->buffer = NULL;
    // }
    // if ( _stage->cr ) {
    //     cairo_destroy (_stage->cr);
    //     _stage->cr = NULL;
    // }

    // //
    // buffer = ex_malloc ( 4 * _width * _height * sizeof (uint8) );
    // ex_memzero( buffer, 4 * _width * _height * sizeof (uint8) );
    // surf = cairo_image_surface_create_for_data ( buffer,
    //                                              CAIRO_FORMAT_ARGB32,
    //                                              _width,
    //                                              _height,
    //                                              4 * _width );
    // if ( cairo_surface_status (surf) != CAIRO_STATUS_SUCCESS ) {
    //     ex_free (buffer);
    //     buffer = NULL;
    //     ex_error ("can't create cairo surface.");
    //     return;
    // }

    // // create cairo context
    // cr = cairo_create (surf);
    // cairo_surface_destroy (surf);
    // if ( cairo_status (cr) != CAIRO_STATUS_SUCCESS ) {
    //     ex_free (buffer);
    //     buffer = NULL;
    //     ex_error ("can't create cairo context");
    //     return;
    // }

    // //
    // _stage->buffer = buffer;
    // _stage->cr = cr;
    // } DISABLE end 
}
