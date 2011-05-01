// ======================================================================================
// File         : os.c
// Author       : Wu Jie 
// Last Change  : 04/04/2011 | 18:47:47 PM | Monday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#if (EX_PLATFORM == EX_WIN32)
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// defines
extern int __PHYSFS_platformExists( const char * );
extern int __PHYSFS_platformIsSymLink( const char * );
extern int __PHYSFS_platformIsDirectory( const char * );
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_os_mkdir ( const char *_path ) {
#if (EX_PLATFORM == EX_WIN32)
    return -1;
#else
    int rc;
    rc = mkdir(_path, S_IRWXU);
    if ( rc == -1 )
        return -1;
    return 0;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_isdir ( const char *_path ) {
#if (EX_PLATFORM == EX_WIN32)
    return __PHYSFS_platformIsDirectory(_path);
#else
    struct stat statbuf;

    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return S_ISDIR(statbuf.st_mode) ? true : false;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_issymlink ( const char *_path ) {
#if (EX_PLATFORM == EX_WIN32)
    return __PHYSFS_platformIsSymLink(_path);
#else
    struct stat statbuf;

    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return S_ISLNK(statbuf.st_mode) ? true : false;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_isfile ( const char *_path ) {
#if (EX_PLATFORM == EX_WIN32)
    if ( __PHYSFS_platformExists(_path) == false ) {
        return false;
    }
    return (__PHYSFS_platformIsSymLink(_path) || __PHYSFS_platformIsDirectory(_path)) ? false : true;
#else
    struct stat statbuf;

    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return (S_ISDIR(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) ? false : true;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_exists ( const char *_path ) {
#if (EX_PLATFORM == EX_WIN32)
    return __PHYSFS_platformExists(_path);
#else
    struct stat statbuf;

    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return true;
#endif
}

