//
//  main.m
//  ios_simple
//
//  Created by Jie Wu on 4/29/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "exsdk.h"
#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {

    ex_core_set_default_path ( "./" );
    if ( ex_core_init( &argc, &argv ) != -1 ) {

        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        int retVal = UIApplicationMain(argc, argv, nil, @"SimpleAppDelegate");
        [pool release];
        return retVal;

    }
    return -1;
}
