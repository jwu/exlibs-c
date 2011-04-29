// ======================================================================================
// File         : file_mac.c
// Author       : Wu Jie 
// Last Change  : 01/15/2011 | 11:21:14 AM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_file_t *ex_fopen ( const char *_filename, const char *_mode ) {
    ex_assert_return ( _filename && *_filename, NULL, "file name can't be null" );
    ex_assert_return ( _mode && *_mode, NULL, "file mode can't be null" );

    FILE *fp = NULL;
	fp = fopen(_filename, _mode);
    if ( fp == NULL ) {
        ex_error ( "failed to open file %s", _filename );
        return NULL;
    }

    // now create ex_file_t
    ex_file_t *file = (ex_file_t *)ex_malloc ( sizeof(ex_file_t) );
    file->fp = fp;
    return file;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fclose ( ex_file_t *_file ) {
    ex_assert_return( _file != NULL, -1, "the file can't be null" );
    if ( fclose (_file->fp) != 0 ) {
        ex_error ( "failed to close file." );
        ex_free (_file);
        return -1;
    }

    ex_free (_file);
    return 0;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fseek ( ex_file_t *_file, size_t _offset, int _whence ) {
    if ( fseek(_file->fp, _offset, _whence) == 0 ) {
        return ftell (_file->fp);
    } else {
        ex_error ( "failed to seek file" );
        return -1;
    }
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fread ( ex_file_t *_file, void *_buf, size_t _size, size_t _count ) {
    size_t nread;

    nread = fread( _buf, _size, _count, _file->fp );
    if ( nread == 0 && ferror(_file->fp) ) {
        ex_error ( "failed to read file" );
        return -1;
    }
    return nread;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fwrite ( ex_file_t *_file, const void *_buf, size_t _size, size_t _count ) {
    size_t nwrote;
    nwrote = fwrite( _buf, _size, _count, _file->fp );
    if ( nwrote == 0 && ferror(_file->fp) ) {
        ex_error ( "failed to write file" );
    }
    return nwrote;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fsize ( ex_file_t *_file ) {
    size_t cur,size; 

    cur = ftell(_file->fp);
    fseek ( _file->fp, 0, SEEK_END );
    size = ftell(_file->fp);
    fseek ( _file->fp, cur, SEEK_SET );
    return size;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_ftell ( ex_file_t *_file ) {
    return ftell (_file->fp);
} 


