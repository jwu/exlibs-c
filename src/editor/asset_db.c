// ======================================================================================
// File         : asset_db.c
// Author       : Wu Jie 
// Last Change  : 07/13/2011 | 19:54:37 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;
static ex_hashmap_t __uid_to_path;
static ex_hashmap_t __path_to_uid;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __do_import ( const char *_path ) {
    int path_len;
    void *result;
    ex_uid_t uid;

    //
    path_len = strlen(_path);
    result = ex_hashmap_get( &__path_to_uid, &_path, NULL );
    if ( result == NULL ) {
        uid = ex_generate_uid();
        ex_hashmap_insert( &__path_to_uid, &_path, &uid, NULL );
        ex_hashmap_insert( &__uid_to_path, &uid, &_path, NULL );
    }
    else {
        uid = *((ex_uid_t *)result);
    }

    //
    if ( strncmp (_path+path_len-4, ".png", 4 ) == 0 ) {
        // TODO: import texture
    }
    else if ( strncmp (_path+path_len-4, ".ttf", 4 ) == 0 ) {
        // TODO: import true type font
    }
} 


///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_asset_db_init () {

    ex_stream_t *stream_read;

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "ex_asset_db already inited" );
        return 1;
    }

    // init uid_to_path table
    ex_hashmap_init ( &__uid_to_path,
                      EX_TYPEID(uid), sizeof(ex_uid_t), 
                      EX_TYPEID(strid), sizeof(strid_t),
                      1024,
                      ex_hashkey_uid, ex_keycmp_uid,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                    );

    // init path_to_uid table
    ex_hashmap_init ( &__path_to_uid,
                      EX_TYPEID(strid), sizeof(strid_t),
                      EX_TYPEID(uid), sizeof(ex_uid_t), 
                      1024,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                    );

    // if not found .exsdk, create one 
    if ( ex_fsys_dir_exists ( ".exsdk" ) == false ) {
        ex_fsys_mkdir ( ".exsdk" );
        ex_log ("create directory .exsdk/");
    }

    // load asset_db path_to_uid and uid_to_path
    if ( ex_fsys_file_exists( ".exsdk/assets_map.json" ) == false ) {
        stream_read = ex_create_json_read_stream( ".exsdk/assets_map.json" );
        __ex_serialize_map (stream_read, ex_strid("uid_to_path"), &__uid_to_path );
        __ex_serialize_map (stream_read, ex_strid("path_to_uid"), &__path_to_uid );
        ex_destroy_json_stream((ex_stream_json_t *)stream_read);
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_asset_db_deinit () {
    if ( __initialized ) {

        //
        ex_log ( "ex_asset_db deinitied" );
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_asset_db_initialized () { return __initialized; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_asset_db_uid_to_asset_path ( ex_uid_t _uid ) {
    return EX_STRID_NULL; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_uid_t ex_asset_db_asset_path_to_uid ( const char *_path ) {
    return EX_UID_INVALID; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_asset_db_import ( const char *_path, bool _recursively ) {

    //
    if ( ex_fsys_exists (_path) == false ) {
        ex_error ("Failed to import assets at: %s. Can't find the path", _path );
        return;
    }

    // scan the directory, and import assets in it.
    if ( ex_fsys_isdir(_path) ) {
        char **file_list, **i;
        char full_path[MAX_PATH];
        int dir_len, fname_len;

        //
        dir_len = strlen(_path);
        if ( dir_len+2 > MAX_PATH ) {
            ex_error ( "directory path is too long! %s", _path );
            return;
        }

        // append '/' at the end of the path if not exists.
        strncpy ( full_path, _path, dir_len  );
        if ( full_path[dir_len-1] != '/' ) {
            full_path[dir_len] = '/';
            full_path[dir_len+1] = '\0';
            dir_len += 1;
        }
        else {
            full_path[dir_len] = '\0';
        }

        // process files
        file_list = ex_fsys_files_in(_path);
        for ( i = file_list; *i != NULL; ++i ) {
            fname_len = strlen(*i);
            if ( dir_len + fname_len + 1 > MAX_PATH ) {
                ex_error ( "file path is too long! %s%s", _path, *i );
                continue;
            }

            // skip the hidden directory or file
            if ( **i == '.' ) {
                continue;
            }

            // get the full path
            full_path[dir_len] = '\0'; // the easist way to reset the full_path to base_path
            strncat ( full_path, *i, fname_len  );
            full_path[dir_len+fname_len] = '\0';

            //
            ex_asset_db_import ( full_path, _recursively );
        }

        return;
    }

    // do real asset import
    __do_import (_path);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_asset_db_import_async ( const char *_path, bool _recursively ) {
    // TODO:
}
