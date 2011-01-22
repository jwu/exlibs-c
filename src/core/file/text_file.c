// ======================================================================================
// File         : text_file.c
// Author       : Wu Jie 
// Last Change  : 01/23/2011 | 01:11:49 AM | Sunday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define BUF_SIZE 2048

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_text_file_t *ex_create_text_file ( const char *_filename, bool _readonly ) {
    ex_text_file_t *txtFile;
    ex_file_t *file;

    if ( _readonly )
        file = ex_open_file ( _filename, "r+" );
    else
        file = ex_open_file ( _filename, "w+" );

    if ( file == NULL ) 
        return NULL;

    txtFile = (ex_text_file_t *)ex_malloc ( sizeof(ex_text_file_t) );
    txtFile->index = 0;
    txtFile->file = file;
    return txtFile;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_text_file ( ex_text_file_t *_txtFile ) {
    if ( _txtFile->index != 0 )
        ex_file_write ( _txtFile->file, _txtFile->buffer, 1, _txtFile->index );

    _txtFile->index = 0;
    ex_close_file(_txtFile->file);
    ex_free (_txtFile);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_text_file_write_line ( ex_text_file_t *_txtFile, const char *_fmt, ... ) {
    int result = -1;
    char buf[BUF_SIZE];

    // keep get va string until success 
    EX_GET_VA_STRING_WITH_RESULT( buf, 2048, _fmt, &result );

    // if we can't use BUF_SIZE store it. 
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE*2;
        while ( result == -1 ) {
            dyn_buf = (char *)ex_realloc( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count, _fmt, &result );
            buffer_count *= 2;
        }

        //
        if ( _txtFile->index != 0 ) {
            ex_file_write ( _txtFile->file, _txtFile->buffer, 1, _txtFile->index );
            _txtFile->index = 0;
        }
        ex_file_write ( _txtFile->file, dyn_buf, 1, result );
        ex_free (dyn_buf);
    }
    else {
        int rest_size = BUF_SIZE - _txtFile->index;

        // if the in text count more than the reset space of our buffer
        // fill the text-buffer, then write it to the sys-file, and store the rest to the buffer. 
        if ( result >= rest_size ) {
            memcpy ( _txtFile->buffer + _txtFile->index, buf, rest_size ); 
            ex_file_write ( _txtFile->file, _txtFile->buffer, 1, BUF_SIZE );
            memcpy ( _txtFile->buffer, buf + rest_size, result - rest_size ); 
            _txtFile->index = result - rest_size;
        }
        // just fill the buffer.
        else {
            memcpy ( _txtFile->buffer + _txtFile->index, buf, result );
            _txtFile->index += result;
        }
    }
    return result;
} 
