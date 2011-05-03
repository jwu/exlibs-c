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
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    }

    return self;
}

// DELME { 
// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// - (void) didRotate: (NSNotification *) notification
// {
//     // UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
//     // TODO { 
//     // m_renderingEngine->OnRotate((DeviceOrientation) orientation);
//     // } TODO end 
//     [self drawView];
// }
// } DELME end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

-(void) swapBuffers {

#ifdef __IPHONE_4_0
	
    // TODO { 
    // if (multiSampling_)
    // {
    //     /* Resolve from msaaFramebuffer to resolveFramebuffer */
    //     //glDisable(GL_SCISSOR_TEST);     
    //     glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, [renderer_ msaaFrameBuffer]);
    //     glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, [renderer_ defaultFrameBuffer]);
    //     glResolveMultisampleFramebufferAPPLE();
    // }
    // 
    // if( discardFramebufferSupported_)
    // {	
    //     if (multiSampling_)
    //     {
    //         if (depthFormat_)
    //         {
    //             GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
    //             glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
    //         }
    //         else
    //         {
    //             GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES};
    //             glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
    //         }
    //         
    //         glBindRenderbufferOES(GL_RENDERBUFFER_OES, [renderer_ colorRenderBuffer]);
    // 
    //     }	
    //     
    //     // not MSAA
    //     else if (depthFormat_ ) {
    //         GLenum attachments[] = { GL_DEPTH_ATTACHMENT_OES};
    //         glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES, 1, attachments);
    //     }
    // }
    // } TODO end 
	
#endif // __IPHONE_4_0
	
	if( ![context presentRenderbuffer:GL_RENDERBUFFER_OES] )
		ex_error ( "Failed to swap renderbuffer" );

#ifdef EX_DEBUG
	EX_GL_CHECK_ERROR();
#endif
	
    // TODO { 
    // // We can safely re-bind the framebuffer here, since this will be the
    // // 1st instruction of the new main loop
    // if( multiSampling_ )
    //     glBindFramebufferOES(GL_FRAMEBUFFER_OES, [renderer_ msaaFrameBuffer]);
    // } TODO end 
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
