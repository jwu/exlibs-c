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

ex_text_file_t *ex_text_fopen ( const char *_filename, bool _readonly ) {
    ex_text_file_t *txtFile;
    ex_file_t *file;

    if ( _readonly )
        file = ex_fopen_r ( _filename );
    else
        file = ex_fopen_w ( _filename );

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

void ex_text_fclose ( ex_text_file_t *_txtFile ) {
    if ( _txtFile->index != 0 )
        ex_fwrite ( _txtFile->file, _txtFile->buffer, _txtFile->index );

    _txtFile->index = 0;
    ex_fclose(_txtFile->file);
    ex_free (_txtFile);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_text_fwrite ( ex_text_file_t *_txtFile, const char *_text, int _len ) {
    int wroteBytes = 0;

    if ( _len > BUF_SIZE ) {
        if ( _txtFile->index != 0 ) {
            wroteBytes += ex_fwrite ( _txtFile->file, _txtFile->buffer, _txtFile->index );
            _txtFile->index = 0;
        }
        wroteBytes += ex_fwrite ( _txtFile->file, _text, _len );
    }
    else {
        int rest_size = BUF_SIZE - _txtFile->index;

        // if the in text count more than the reset space of our buffer
        // fill the text-buffer, then write it to the sys-file, and store the rest to the buffer. 
        if ( _len >= rest_size ) {
            memcpy ( _txtFile->buffer + _txtFile->index, _text, rest_size ); 
            wroteBytes += ex_fwrite ( _txtFile->file, _txtFile->buffer, BUF_SIZE );
            memcpy ( _txtFile->buffer, _text + rest_size, _len - rest_size ); 
            _txtFile->index = _len - rest_size;
        }
        // just fill the buffer.
        else {
            memcpy ( _txtFile->buffer + _txtFile->index, _text, _len );
            _txtFile->index += _len;
        }
    }
    return wroteBytes;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_text_fwrite_fmt ( ex_text_file_t *_txtFile, const char *_fmt, ... ) {
    int result = -1;
    char buf[BUF_SIZE];

    // keep get va string until success 
    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE, _fmt, &result );

    // if we can't use BUF_SIZE store it. 
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE*2;

        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count, _fmt, &result );
            buffer_count *= 2;
        } while ( result == -1 );
        ex_text_fwrite( _txtFile, dyn_buf, result );
        ex_free_nomng (dyn_buf);
    }
    else {
        ex_text_fwrite( _txtFile, buf, result );
    }

    return result;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_text_fwrite_line ( ex_text_file_t *_txtFile, const char *_fmt, ... ) {
    int result = -1;
    char buf[BUF_SIZE];
    char *buffer;

    // keep get va string until success 
    // NOTE: the buffer_count-1 will leave 1 character for '\n'
    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _fmt, &result );
    buffer = buf;

    // if we can't use BUF_SIZE store it. 
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE*2;

        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            // NOTE: the buffer_count-1 will leave 1 character for '\n'
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _fmt, &result );
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }

    buffer[result-1] = '\n';
    ex_text_fwrite( _txtFile, buffer, result );

    // if we use dynamic buffer, release it
    if ( buffer != buf )
        ex_free_nomng (buffer);

    return result;
} 
