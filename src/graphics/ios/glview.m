// ======================================================================================
// File         : glview.m
// Author       : Wu Jie 
// Last Change  : 04/29/2011 | 20:14:16 PM | Friday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// imports
///////////////////////////////////////////////////////////////////////////////

#import "exsdk.h"
#import "glview.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

@implementation GLView

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

+ (Class) layerClass {
	return [CAEAGLLayer class];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (id) initWithFrame: (CGRect) frame {

    if ( (self = [super initWithFrame:frame]) ) {

        //
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*) super.layer; 
        eaglLayer.opaque = YES; 

        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1]; 

        if ( !context || ![EAGLContext setCurrentContext:context] ) { 
            [self release];
            return nil;
        }

        // OpenGL Initialization
        GLuint frameBuffer, renderBuffer;
        glGenFramebuffersOES ( 1, &frameBuffer );
        glGenRenderbuffersOES ( 1, &renderBuffer );

        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, frameBuffer );
        glBindRenderbufferOES ( GL_RENDERBUFFER_OES, renderBuffer );

        [context renderbufferStorage: GL_RENDERBUFFER_OES 
                        fromDrawable: eaglLayer];

        glFramebufferRenderbufferOES( GL_FRAMEBUFFER_OES, 
                                      GL_COLOR_ATTACHMENT0_OES,
                                      GL_RENDERBUFFER_OES, 
                                      renderBuffer );

        glViewport ( 0, 0, CGRectGetWidth(frame), CGRectGetHeight(frame) );

        [self drawView];
    }

    return self;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) drawView {

    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) didRotate: (NSNotification *) notification
{
    // UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    // TODO { 
    // m_renderingEngine->OnRotate((DeviceOrientation) orientation);
    // } TODO end 
    [self drawView];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

- (void) dealloc {

    if ( [EAGLContext currentContext] == context )
        [EAGLContext setCurrentContext:nil];

    [context release];  
    [super dealloc];
}

@end
