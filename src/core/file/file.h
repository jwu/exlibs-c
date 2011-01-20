// ======================================================================================
// File         : file.h
// Author       : Wu Jie 
// Last Change  : 01/15/2011 | 10:16:13 AM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef FILE_H_1295057774
#define FILE_H_1295057774
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_SEEK_SET	0 // Seek from the beginning of data
#define EX_SEEK_CUR	1 // Seek relative to current read point
#define EX_SEEK_END	2 // Seek relative to the end of data

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_file_t {
#if (EX_PLATFORM == EX_WIN32)
    HANDLE handle;
#else
    FILE *fp;
#endif
} ex_file_t;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

extern ex_file_t *ex_open_file ( const char *_filename, const char *_mode ); 

// success: 0
// failed: -1
extern int ex_close_file ( ex_file_t *_file ); 

// success: 0
// failed: -1
extern int ex_file_seek ( ex_file_t *_file, size_t _offset, int _whence ); 

extern int ex_file_read ( ex_file_t *_file, void *_buf, size_t _size, size_t _count );
extern int ex_file_write ( ex_file_t *_file, const void *_buf, size_t _size, size_t _count ); 

extern size_t ex_file_size ( ex_file_t *_file ); 
extern size_t ex_file_tell ( ex_file_t *_file ); 

// #################################################################################
#endif // END FILE_H_1295057774
// #################################################################################
