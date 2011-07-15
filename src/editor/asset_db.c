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

#include <IL/il.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static bool __initialized = false;
static ex_hashmap_t __uid_to_path;
static ex_hashmap_t __path_to_uid;
static ex_hashmap_t __path_to_refptr;

///////////////////////////////////////////////////////////////////////////////
// static defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_ref_t *__import_texture ( const char *_path, int _il_img_fmt ) {

    void *result;
    ex_uid_t uid;

    ILuint imageID, fileSize;
    void *buffer;
    ex_file_t *file;
    ex_ref_t *tex2d_ref = NULL;
    int width, height, bytes;
    int tex2d_fmt, il_fmt;
    strid_t pathID;
    char filename[MAX_PATH];

    // ======================================================== 
    // load image by DevIL 
    // ======================================================== 

    // bind the image
    ilGenImages ( 1, &imageID );
    ilBindImage ( imageID );

    // load the image from raw data
    file = ex_fopen_r ( _path );
    fileSize = ex_fsize (file);
    buffer = ex_malloc ( fileSize );
    ex_fread ( file, buffer, fileSize );
    ilLoadL ( _il_img_fmt, buffer, (ILuint)fileSize );
    ex_free (buffer);
    ex_fclose(file);

    // extract image data
    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);

    // copy the raw data
    bytes = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
    il_fmt = IL_RGBA;
    if ( bytes == 4 ) {
        il_fmt = IL_RGBA;
        tex2d_fmt = EX_TEXTURE_FORMAT_ARGB32;
    }
    else if ( bytes == 3 ) {
        il_fmt = IL_RGB;
        tex2d_fmt = EX_TEXTURE_FORMAT_RGB24;
    }
    else if ( bytes == 1 ) {
        il_fmt = IL_ALPHA;
        tex2d_fmt = EX_TEXTURE_FORMAT_ALPHA8;
    }
    else if ( bytes == 2 ) {
        il_fmt = IL_LUMINANCE_ALPHA;
        tex2d_fmt = EX_TEXTURE_FORMAT_RGB565;
    }
    else {
        ex_error ("bytes number is %d, which not support", bytes );
        il_fmt = -1;
        tex2d_fmt = -1;
        goto FAILED;
    }

    // ======================================================== 
    // Set pixel to texture2d resource 
    // ======================================================== 

    // get uid frome path
    pathID = ex_strid(_path);
    result = ex_hashmap_get( &__path_to_uid, &pathID, NULL );
    if ( result == NULL ) {
        uid = ex_generate_uid();
        ex_hashmap_insert( &__path_to_uid, &pathID, &uid, NULL );
        ex_hashmap_insert( &__uid_to_path, &uid, &pathID, NULL );
    }
    else {
        uid = *((ex_uid_t *)result);
    }

    //
    tex2d_ref = ex_create_texture2d_by_uid( width, height, tex2d_fmt, uid );
    ex_path_get_filename( filename, _path );
    EX_REF_CAST(ex_object_t,tex2d_ref)->name = ex_strid(filename);
    ilCopyPixels( 0, 
                  0, 
                  0, 
                  width, 
                  height, 
                  1, 
                  il_fmt, 
                  IL_UNSIGNED_BYTE, 
                  EX_REF_CAST(ex_texture2d_t,tex2d_ref)->data );

    // ======================================================== 
    // finally, apply the changes 
    // ======================================================== 

    ex_texture2d_apply (tex2d_ref);
    ex_texture2d_set_readable (tex2d_ref, false);

FAILED:
    // unbind il image
    ilBindImage (0);
    ilDeleteImages ( 1, &imageID );
    return tex2d_ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __do_import ( const char *_path ) {
    int path_len;
    ex_ref_t *ref = NULL;

    //
    path_len = strlen(_path);

    // ======================================================== 
    // load texture 
    // ======================================================== 

    if ( strncmp (_path+path_len-4, ".png", 4 ) == 0 ) {
        ref = __import_texture ( _path, IL_PNG );
    }
    else if ( strncmp (_path+path_len-4, ".bmp", 4 ) == 0 ) {
        ref = __import_texture ( _path, IL_BMP );
    }
    else if ( strncmp (_path+path_len-4, ".jpg", 4 ) == 0 ) {
        ref = __import_texture ( _path, IL_JPG );
    }
    else if ( strncmp (_path+path_len-4, ".dds", 4 ) == 0 ) {
        ref = __import_texture ( _path, IL_DDS );
    }
    else if ( strncmp (_path+path_len-4, ".raw", 4 ) == 0 ) {
        ref = __import_texture ( _path, IL_RAW );
    }

    // ======================================================== 
    // load font  
    // ======================================================== 

    else if ( strncmp (_path+path_len-4, ".ttf", 4 ) == 0 ) {
        // TODO: import true type font
    }

    //
    if ( ref && ref->ptr != NULL ) {
        strid_t pathID = ex_strid(_path);
        ex_incref(ref);
        ex_hashmap_insert( &__path_to_refptr, &pathID, &ref, NULL );
        ex_log ( "Improt asset %s", _path );
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

    //
    ex_hashmap_init ( &__path_to_refptr,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(ex_ref_t *), 
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
    if ( ex_fsys_file_exists( ".exsdk/assets_map.json" ) ) {
        stream_read = ex_create_json_read_stream( ".exsdk/assets_map.json" );
        __ex_serialize_map (stream_read, ex_strid("uid_to_path"), &__uid_to_path );
        __ex_serialize_map (stream_read, ex_strid("path_to_uid"), &__path_to_uid );
        ex_destroy_json_stream((ex_stream_json_t *)stream_read);
    }

    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_asset_db_deinit () {
    if ( __initialized ) {

        //
        ex_hashmap_each ( &__path_to_refptr, ex_ref_t *, ref ) {
            ex_decref(ref);
        } ex_hashmap_each_end
        ex_hashmap_deinit (&__path_to_refptr);

        ex_hashmap_deinit (&__path_to_uid);
        ex_hashmap_deinit (&__uid_to_path);

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

ex_ref_t *ex_asset_db_load ( const char *_path ) {

    strid_t pathID;
    void *result;

    pathID = ex_strid(_path);
    result = ex_hashmap_get( &__path_to_refptr, &pathID, NULL );
    if ( result != NULL ) {
        return result; 
    }

    return ex_newref(NULL); // it will be delete in ex_object_gc
}
