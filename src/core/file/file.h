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

#if 0
typedef struct ex_file_t {
#if (EX_PLATFORM == EX_WIN32)
    HANDLE handle;
#else
    FILE *fp;
#endif
} ex_file_t;
#else
typedef void ex_file_t;
#endif

///////////////////////////////////////////////////////////////////////////////
// file sys
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_fsys_init ();
extern void ex_fsys_deinit ();
extern bool ex_fsys_initialized ();
extern void ex_fsys_free_list( void *_list );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern const char *ex_fsys_app_dir (); // current application's directory
extern const char *ex_fsys_user_dir (); // current user's home directory

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_fsys_set_write_dir ( const char *_new_dir );
extern const char *ex_fsys_write_dir ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_fsys_mount ( const char *_dir, const char *_mount_point, bool _append );
extern int ex_fsys_unmount ( const char *_dir );
extern const char *ex_fsys_mount_point ( const char *_dir );
// usage: for ( i = ex_fsys_mounts(); *i != NULL; ++i )
// don't forget to call ex_fsys_free_list(li);
extern char **ex_fsys_mounts ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_fsys_mkdir ( const char *_path );
extern int ex_fsys_rmdir ( const char *_path, bool _force );
extern int ex_fsys_rm ( const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_fsys_isdir ( const char *_path );
extern bool ex_fsys_isfile ( const char *_path );
extern bool ex_fsys_issymlink ( const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// remember call ex_fsys_free_list(); to release the files.
extern char **ex_fsys_files_in ( const char *_path );
extern bool ex_fsys_exists( const char *_path );
extern bool ex_fsys_file_exists( const char *_path );
extern bool ex_fsys_dir_exists( const char *_path );

///////////////////////////////////////////////////////////////////////////////
// file io
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
/* "r" = reading, file must exist */
/* "w" = writing, truncate existing, file may not exist */
/* "r+"= reading or writing, file must exist            */
/* "a" = writing, append file may not exist             */
/* "a+"= append + read, file may not exist              */
/* "w+" = read, write, truncate. file may not exist     */
// ------------------------------------------------------------------ 

// extern ex_file_t *ex_fopen ( const char *_filename, const char *_mode ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_file_t *ex_fopen_r ( const char *_filename ); 
extern ex_file_t *ex_fopen_w ( const char *_filename ); 
extern ex_file_t *ex_fopen_a ( const char *_filename ); 
extern int ex_fclose ( ex_file_t *_file ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_fseek ( ex_file_t *_file, size_t _pos ); 
extern size_t ex_fsize ( ex_file_t *_file ); 
extern size_t ex_ftell ( ex_file_t *_file ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int64 ex_fread ( ex_file_t *_file, void *_buf, uint64 _size );
extern int64 ex_fwrite ( ex_file_t *_file, const void *_buf, uint64 _size );
extern int ex_fflush ( ex_file_t *_file ); 
extern int ex_feof ( ex_file_t *_file ); 

// #################################################################################
#endif // END FILE_H_1295057774
// #################################################################################
