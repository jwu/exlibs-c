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
#include "engine/engine_inc.h"
#include "app/app.h"

#import "SimpleAppDelegate.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

@implementation SimpleAppDelegate

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // init window
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    window = [[UIWindow alloc] initWithFrame: screenBounds]; 
    view = [[GLView alloc] initWithFrame: screenBounds]; 

    [window addSubview: view];
    [window makeKeyAndVisible];

    // hide the status bar
    [application setStatusBarHidden: YES];

    // init game
    ex_app_init();
    // ex_lua_load_modules( ex_lua_main_state(), "scripts" );

    return YES;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)applicationWillTerminate:(UIApplication *)application
{
    ex_lua_deinit();
    ex_app_deinit();
    ex_core_deinit();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void)dealloc
{
    [view release];
    [window release];
    [super dealloc];
}

@end
