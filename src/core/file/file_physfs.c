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

static char __main_bundle_path[MAX_PATH]; 

///////////////////////////////////////////////////////////////////////////////
// fsys
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_fsys_init ( const char *_media_path ) {
    char path[MAX_PATH];
    const char *app_path = NULL;

    __PHYSFS_CHECK( PHYSFS_init("."), "%s: failed to init fsys", "ERROR" ); // NOTE: we write like this to prevent compile error 
    PHYSFS_permitSymbolicLinks(1); // yes, we permit symbolic links

#if (EX_PLATFORM == EX_IOS)
    app_path = ex_fsys_main_bundle_path();
#else
    app_path = ex_fsys_app_dir();
#endif

    ex_log("user dir: %s", ex_fsys_user_dir() );
    ex_log("app dir: %s", app_path );

    // NOTE: this must be done before ex_log_init, so that log.txt can be open in the corrent path.

    // ======================================================== 
    if ( ex_is_dev_mode() ) { // dev mode
    // ======================================================== 

        // we are in dev_mode, set the dev_media_path, mount the dev_media_path and dev_path

        // set write dir as dev_media_path
        if ( ex_fsys_set_write_dir( ex_dev_media_path() ) != 0 )
            return -1;
        ex_log("set write dir: %s", ex_dev_media_path() );

        // mount dev_media_path
        if ( ex_fsys_mount( ex_dev_media_path(), NULL, true ) != 0 )
            return -1;
        ex_log("mount dir: %s", ex_dev_media_path() );

        // mount dev_path
        if ( ex_fsys_mount( ex_dev_path(), NULL, true ) != 0 )
            return -1;
        ex_log("mount dir: %s", ex_dev_path() );

    }

    // ======================================================== 
    else { // normal mode
    // ======================================================== 

        if ( _media_path ) {

            // if the media_path exists, use it. if not, try to search it in the app/ directory
            if ( ex_os_exists (_media_path) == false ) {
                strncpy ( path, app_path, MAX_PATH );
                strcat ( path, _media_path );
            }
            else {
                strncpy ( path, _media_path, MAX_PATH );
            }

            //
            if ( ex_os_exists (path) ) {

                // set write dir
                if ( ex_fsys_set_write_dir(path) != 0 )
                    return -1;
                ex_log("set write dir: %s", path );

                // mount the write dir 
                // NOTE: set write dir doesn't means you mount it.
                if ( ex_fsys_mount( path, NULL, true ) != 0 )
                    return -1;
                ex_log("mount dir: %s", path );
            }
            else {
                ex_error ( "can't find the media path %s", _media_path );
                return -1;
            }
        }
        else if ( app_path ) {
            // we add app/ as the default write directory
            strncpy ( path, app_path, MAX_PATH );

            // NOTE: set write dir doesn't means you mount it.
            // we mount the app/ directory below
            if ( ex_fsys_set_write_dir(path) != 0 )
                return -1;
            ex_log("set default write dir: %s", path );
        }
        else {
            ex_error ( "can't set a write directory" );
            return -1;
        }

        // if ~/.exsdk/ exists we add it as the primary builtin search directory
        if ( ex_fsys_user_dir() ) {
            strncpy ( path, ex_fsys_user_dir(), MAX_PATH );
            strcat ( path, ".exsdk/" );
            if ( ex_os_exists(path) && ex_os_isdir(path) ) {
                if ( ex_fsys_mount( path, NULL, true ) != 0 )
                    return -1;
                ex_log("mount dir: %s", path );
            }
        }

        // if app/ exists, we add it as the second builtin search directory.
        if ( app_path ) {
            strncpy ( path, app_path, MAX_PATH );
            if ( ex_fsys_mount( path, NULL, true ) != 0 )
                return -1;
            ex_log("mount dir: %s", path );
        }
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

void ex_fsys_set_main_bundle_path ( const char *_path ) {
    strncpy( __main_bundle_path, _path, MAX_PATH );
}
const char *ex_fsys_main_bundle_path () { return __main_bundle_path; }

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
