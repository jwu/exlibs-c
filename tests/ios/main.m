// ======================================================================================
// File         : main.m
// Author       : Wu Jie 
// Last Change  : 05/03/2011 | 13:07:24 PM | Tuesday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// imports
///////////////////////////////////////////////////////////////////////////////

#import "exsdk.h"
#import <UIKit/UIKit.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern char *g_world_path;
// ------------------------------------------------------------------ 

static void __parse_world ( int *_argc_p, char ***_argv_p ) {
    int argc = *_argc_p;
    char **argv = *_argv_p;
    int i, e;

    for ( i = 1; i < argc; ++i ) {
        if ( strcmp ( "--world", argv[i] ) == 0 ||
             strncmp ( "--world=", argv[i], 8 ) == 0 ) 
        {
            char *equal = argv[i] + 7;

            if ( *equal == '=' ) {
                int slen = strlen(equal+1);
                g_world_path = malloc ( slen + 1 );
                strcpy ( g_world_path, equal+1 );
            }
            else if ( i + 1 < argc ) {
                g_world_path = malloc ( strlen(argv[i+1]) + 1 );
                strcpy ( g_world_path, argv[i+1] ); 

                argv[i] = NULL;
                i += 1;
            }
            argv[i] = NULL;
        }
    }

    // reset the argv and argc
    e = 0;
    for (i = 1; i < argc; i++) {
        if (e) {
            if (argv[i]) {
                argv[e++] = argv[i];
                argv[i] = NULL;
            }
        }
        else if (!argv[i])
            e = i;
    }
    if (e)
        *_argc_p = e;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main(int argc, char *argv[]) {

    // get the main bundle path
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *path = [[[NSBundle mainBundle] bundlePath]stringByAppendingPathComponent:@""];
    path = [path stringByAppendingString:@"/"];
    const char *c_path = [path UTF8String];

    // set the main bundle path
    ex_fsys_set_main_bundle_path(c_path);

    // init the core
    if ( ex_core_init( &argc, &argv ) != -1 ) {
        __parse_world ( &argc, &argv );

        // init the application 
        int retVal = UIApplicationMain(argc, argv, nil, @"SimpleAppDelegate");
        [pool release];
        return retVal;
    }

    [pool release];
    return -1;
}
