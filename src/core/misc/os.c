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
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_os_mkdir ( const char *_path ) {
    int rc;

#if (EX_PLATFORM == EX_WIN32)
#else
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
    struct stat statbuf;

#if (EX_PLATFORM == EX_WIN32)
#else
    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return S_ISDIR(statbuf.st_mode) ? true : false;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_issymlink ( const char *_path ) {
    struct stat statbuf;

#if (EX_PLATFORM == EX_WIN32)
#else
    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return S_ISLNK(statbuf.st_mode) ? true : false;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_isfile ( const char *_path ) {
    struct stat statbuf;

#if (EX_PLATFORM == EX_WIN32)
#else
    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return (S_ISDIR(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) ? false : true;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_os_exists ( const char *_path ) {
    struct stat statbuf;

#if (EX_PLATFORM == EX_WIN32)
#else
    if ( lstat(_path, &statbuf) == -1 )
        return false;
    return true;
#endif
}

