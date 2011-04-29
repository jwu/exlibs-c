// ======================================================================================
// File         : file_physfs.c
// Author       : Wu Jie 
// Last Change  : 02/05/2011 | 19:25:03 PM | Saturday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

// physfs
#include <physfs.h>

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#define __PHYSFS_CHECK(_expr,_msg,...) \
    if ( (_expr) == 0 ) { \
        /*__log_error( __FILE__, __FUNCTION__, __LINE__, _msg"\n|- physFS error message: %s", ##__VA_ARGS__, PHYSFS_getLastError() );*/ \
        ex_error( _msg"\n|- physFS error message: %s", ##__VA_ARGS__, PHYSFS_getLastError() ); \
        return -1; \
    }

///////////////////////////////////////////////////////////////////////////////
// fsys
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_init () {
    char path[1024];

    __PHYSFS_CHECK( PHYSFS_init(NULL), "%s: failed to init fsys", "ERROR" ); // NOTE: we write like this to prevent compile error 
    PHYSFS_permitSymbolicLinks(1); // yes, we permit symbolic links

    ex_log("user dir: %s", ex_fsys_user_dir() );
    ex_log("app dir: %s", ex_fsys_app_dir() );

    // we add app/ as the default write directory
    strncpy ( path, ex_fsys_app_dir(), 1024 );
    if ( ex_fsys_set_write_dir(path) == 0 )
        ex_log("set default write dir: %s", path );

    // if ~/.exsdk/ exists we add it as the primary builtin search directory
    strncpy ( path, ex_fsys_user_dir(), 1024 );
    strcat ( path, ".exsdk/" );
    if ( ex_os_exists(path) && ex_os_isdir(path) ) {
        if ( ex_fsys_mount( path, "/", true ) == 0 )
            ex_log("mount dir: %s", path );
    }

    // if app/builtin/ exists, we add it as the second builtin search directory.
    strncpy ( path, ex_fsys_app_dir(), 1024 );
    strcat ( path, "builtin/" );
    if ( ex_os_exists(path) && ex_os_isdir(path) ) {
        if ( ex_fsys_mount( path, "/", true ) == 0 )
            ex_log("mount dir: %s", path );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_fsys_deinit () {
    PHYSFS_deinit();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_initialized () {
    return PHYSFS_isInit();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_fsys_free_list( void *_list ) {
    PHYSFS_freeList(_list);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

const char *ex_fsys_app_dir () { return PHYSFS_getBaseDir(); }
const char *ex_fsys_user_dir () { return PHYSFS_getUserDir(); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_set_write_dir ( const char *_new_dir ) { 
    __PHYSFS_CHECK( PHYSFS_setWriteDir(_new_dir),
                    "failed to set write directory to %s",
                    _new_dir ); 
    return 0;
}
const char *ex_fsys_write_dir () { return PHYSFS_getWriteDir(); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_mount ( const char *_dir, const char *_mount_point, bool _append ) {
    __PHYSFS_CHECK( PHYSFS_mount ( _dir, _mount_point, _append ), 
                    "failed to mount dir %s to %s", 
                    _dir,
                    _mount_point );
    return 0;
}
int ex_fsys_unmount ( const char *_dir ) {
#if 0
    __PHYSFS_CHECK( PHYSFS_unmount(_dir), 
                    "failed to unmount dir %s", 
                    _dir );
    return 0;
#else
    __PHYSFS_CHECK( PHYSFS_removeFromSearchPath(_dir), 
                    "failed to unmount dir %s", 
                    _dir );
    return 0;
#endif
}
const char *ex_fsys_mount_point ( const char *_dir ) {
    return PHYSFS_getMountPoint ( _dir );
}
char **ex_fsys_mounts () { return PHYSFS_getSearchPath (); }
const char *ex_fsys_realpath ( const char *_path ) { return PHYSFS_getRealDir(_path); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_mkdir ( const char *_dir ) {
    __PHYSFS_CHECK( PHYSFS_mkdir(_dir), 
                    "failed to create directory %s", 
                    _dir );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_rmdir ( const char *_dir, bool _force ) {
    if ( ex_fsys_isdir (_dir) ) {
        // TODO: if force, remove all the files in the directory, then delete it.
        // if not, check if the directory is empty and delete it.

        __PHYSFS_CHECK( PHYSFS_delete(_dir), 
                        "failed to delete directory %s", 
                        _dir );
        return 0;
    }

    ex_error ( "%s is not a directory", _dir );
    return -1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_rm ( const char *_filename ) {
    if ( ex_fsys_isfile (_filename) ) {
        __PHYSFS_CHECK( PHYSFS_delete(_filename), 
                        "failed to delete file %s", 
                        _filename );
        return 0;
    }

    ex_error ( "%s is not a file", _filename );
    return -1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_isdir ( const char *_path ) {
#if 0
    PHYSFS_Stat stat;
    if ( PHYSFS_stat( _path, &stat ) == 0 ) {
        ex_error ( "error: %s", PHYSFS_getLastError() );
        return false;
    }
    return (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
#else
    return PHYSFS_isDirectory(_path);
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_isfile ( const char *_path ) {
#if 0
    PHYSFS_Stat stat;
    if ( PHYSFS_stat( _path, &stat ) == 0 ) {
        ex_error ( "error: %s", PHYSFS_getLastError() );
        return false;
    }
    return (stat.filetype == PHYSFS_FILETYPE_REGULAR);
#else
    return !PHYSFS_isDirectory(_path);
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_issymlink ( const char *_path ) {
#if 0
    PHYSFS_Stat stat;
    if ( PHYSFS_stat( _path, &stat ) == 0 ) {
        ex_error ( "error: %s", PHYSFS_getLastError() );
        return false;
    }
    return (stat.filetype == PHYSFS_FILETYPE_SYMLINK);
#else
    return PHYSFS_isSymbolicLink(_path);
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

char **ex_fsys_files_in ( const char *_path ) {
    return PHYSFS_enumerateFiles(_path); 
}

bool ex_fsys_exists( const char *_path ) {
    return PHYSFS_exists(_path);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_file_exists( const char *_path ) {
    return ex_fsys_exists(_path) && ex_fsys_isfile(_path);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_fsys_dir_exists( const char *_path ) {
    return ex_fsys_exists(_path) && ex_fsys_isdir(_path);
}

///////////////////////////////////////////////////////////////////////////////
// file io
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_file_t *ex_fopen_r ( const char *_filename ) {
    return PHYSFS_openRead(_filename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_file_t *ex_fopen_w ( const char *_filename ) {
    return PHYSFS_openWrite(_filename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_file_t *ex_fopen_a ( const char *_filename ) {
    return PHYSFS_openAppend(_filename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fclose ( ex_file_t *_file ) {
    return PHYSFS_close(_file);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fseek ( ex_file_t *_file, size_t _pos ) {
    return PHYSFS_seek(_file,_pos);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fsize ( ex_file_t *_file ) {
    return (size_t)PHYSFS_fileLength(_file);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_ftell ( ex_file_t *_file ) {
    return (size_t)PHYSFS_tell(_file);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fread ( ex_file_t *_file, void *_buf, uint64 _size ) {
#if 0
    return (size_t)PHYSFS_readBytes( _file, _buf, _size );
#else
    return (size_t)PHYSFS_read( _file, _buf, 1, (uint)_size );
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_fwrite ( ex_file_t *_file, const void *_buf, uint64 _size ) {
#if 0
    return (size_t)PHYSFS_writeBytes( _file, _buf, _size );
#else
    return (size_t)PHYSFS_write( _file, _buf, 1, (uint)_size );
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fflush ( ex_file_t *_file ) {
    return PHYSFS_flush(_file);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_feof ( ex_file_t *_file ) {
    return PHYSFS_eof(_file);
}
