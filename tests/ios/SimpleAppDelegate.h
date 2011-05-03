// ======================================================================================
// File         : SimpleAppDelegate.h
// Author       : Wu Jie 
// Last Change  : 04/29/2011 | 23:54:06 PM | Friday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// import
///////////////////////////////////////////////////////////////////////////////

#import <UIKit/UIKit.h>
#import "graphics/ios/glview.h"

///////////////////////////////////////////////////////////////////////////////
// decl
///////////////////////////////////////////////////////////////////////////////

@interface SimpleAppDelegate : NSObject <UIApplicationDelegate> {

    UIWindow *window;
    GLView *view;
}

- (void) startGame;
- (void) startTick:(UIApplication*)application; 
- (void) tick;

@end
