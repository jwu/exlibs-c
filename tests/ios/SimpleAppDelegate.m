// ======================================================================================
// File         : SimpleAppDelegate.m
// Author       : Wu Jie 
// Last Change  : 04/29/2011 | 23:54:22 PM | Friday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// imports
///////////////////////////////////////////////////////////////////////////////

#import "exsdk.h"
#import "engine/engine_inc.h"
#import "app/app.h"

#import "SimpleAppDelegate.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

ex_ref_t *g_world = NULL;
char *g_world_path = NULL;
int __win_width = 640; 
int __win_height = 480; 

#define FPS 60.0 

@implementation SimpleAppDelegate

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (BOOL) application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // init window
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    window = [[UIWindow alloc] initWithFrame: screenBounds]; 
    view = [[GLView alloc] initWithFrame: screenBounds]; 

    [window addSubview: view];
    [window makeKeyAndVisible];

    // get window width and height
    CAEAGLLayer *eaglLayer = (CAEAGLLayer*)[view layer];
	CGSize newSize = [eaglLayer bounds].size;
	__win_width = newSize.width;
	__win_height = newSize.height;

    // hide the status bar
    [application setStatusBarHidden: YES];

    // init game
    ex_app_init();

    // load lua modules
    ex_lua_load_modules( ex_lua_main_state(), "builtin" );
    ex_lua_load_modules( ex_lua_main_state(), "scripts" );

    // init the game
    [self startGame];

    return YES;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) startGame {

    struct lua_State *l;
    ex_ref_t *mainCam;

    // EX_REGISTER_CLASS_2( ex_simple_t, ex.simple );

    // create the world
    g_world = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_incref(g_world);
    EX_REF_CAST(ex_object_t,g_world)->init(g_world);

    // create main camera
    ex_log ("create main camera...");
    ex_world_create_camera2d ( g_world, ex_strid("main_camera") );
    mainCam = ex_world_main_camera (g_world);
    ex_assert ( mainCam, "can't find main camera" );
    ex_camera_set_ortho( mainCam, true );
    ex_camera_set_aspect( mainCam, (float)__win_width/(float)__win_height );
    ex_camera_set_ortho_size( mainCam, (float)__win_height/2.0f );
    ex_log("viewport width = %i", __win_width );
    ex_log("viewport height = %i", __win_height );

    // load the world from script
    l = ex_lua_main_state();
    ex_assert_return ( g_world_path, /*void*/, "can't find a world! please specific it by --world=" );
    ex_lua_dofile( l, g_world_path, NULL );

    // run the world
    ex_world_run(g_world);

    [NSThread detachNewThreadSelector:@selector(startTick:) toTarget:self withObject:nil];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) startTick:(UIApplication*)application 
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; 

	const double SecondsPerFrame = 1.0 / ((FPS > 60.0)? 60.0: FPS);
	const double OneMillisecond = 1.0 / 1000.0;
	for (;;) {
		double frameStartTime = (double)CFAbsoluteTimeGetCurrent();
		[self performSelectorOnMainThread:@selector(tick) withObject:nil waitUntilDone:YES]; 
		
		double secondsToProcessEvents = SecondsPerFrame - (((double)CFAbsoluteTimeGetCurrent()) - frameStartTime);
		// if we run considerably slower than desired framerate anyhow
		// then we should sleep for a while leaving OS some room to process events
		if (secondsToProcessEvents < -OneMillisecond)
			secondsToProcessEvents = OneMillisecond;
		if (secondsToProcessEvents > 0)
			[NSThread sleepForTimeInterval:secondsToProcessEvents];
	}

	[pool release];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) tick {

    ex_world_update(g_world); // tick world
    ex_world_render(g_world,true); // render the world

    [view swapBuffers];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) applicationWillTerminate:(UIApplication *)application
{
    // deinit the world
    ex_world_stop(g_world);
    ex_world_clear(g_world);
    ex_decref(g_world);
    ex_destroy_object_immediately(g_world, true);

    // deinit the core
    ex_lua_deinit();
    ex_app_deinit();
    ex_core_deinit();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) dealloc
{
    [view release];
    [window release];
    [super dealloc];
}

@end
