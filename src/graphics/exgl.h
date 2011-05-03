// ======================================================================================
// File         : exgl.h
// Author       : Wu Jie 
// Last Change  : 04/29/2011 | 18:23:30 PM | Friday,April
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef EXGL_H_1304072615
#define EXGL_H_1304072615
// #################################################################################


#define EX_GL_CHECK_ERROR() ({ \
    GLenum __error = glGetError(); \
    if(__error) ex_error("OpenGL Error: 0x%04X in %s\n", __error, __FUNCTION__); \
    })

// use macro generic some interface for OpenGLES and OpenGL
#if ( EX_PLATFORM == EX_IOS )

    #define ex_glOrtho					    glOrthof
    #define	ex_glClearDepth				    glClearDepthf
    #define ex_glGenerateMipmap			    glGenerateMipmapOES
    #define ex_glGenFramebuffers			glGenFramebuffersOES
    #define ex_glBindFramebuffer			glBindFramebufferOES
    #define ex_glFramebufferTexture2D	    glFramebufferTexture2DOES
    #define ex_glDeleteFramebuffers		    glDeleteFramebuffersOES
    #define ex_glCheckFramebufferStatus	    glCheckFramebufferStatusOES
    #define ex_glTranslate				    glTranslatef
    
    #define EX_GL_FRAMEBUFFER			    GL_FRAMEBUFFER_OES
    #define EX_GL_FRAMEBUFFER_BINDING	    GL_FRAMEBUFFER_BINDING_OES
    #define EX_GL_COLOR_ATTACHMENT0		    GL_COLOR_ATTACHMENT0_OES
    #define EX_GL_FRAMEBUFFER_COMPLETE	    GL_FRAMEBUFFER_COMPLETE_OES

#else

    #define ex_glOrtho					    glOrtho
    #define	ex_glClearDepth				    glClearDepth
    #define ex_glGenerateMipmap			    glGenerateMipmap
    #define ex_glGenFramebuffers			glGenFramebuffers
    #define ex_glBindFramebuffer			glBindFramebuffer
    #define ex_glFramebufferTexture2D	    glFramebufferTexture2D
    #define ex_glDeleteFramebuffers		    glDeleteFramebuffers
    #define ex_glCheckFramebufferStatus	    glCheckFramebufferStatus
    #define ex_glTranslate				    glTranslated
    
    #define EX_GL_FRAMEBUFFER			    GL_FRAMEBUFFER
    #define EX_GL_FRAMEBUFFER_BINDING	    GL_FRAMEBUFFER_BINDING
    #define EX_GL_COLOR_ATTACHMENT0		    GL_COLOR_ATTACHMENT0
    #define EX_GL_FRAMEBUFFER_COMPLETE	    GL_FRAMEBUFFER_COMPLETE

#endif

// #################################################################################
#endif // END EXGL_H_1304072615
// #################################################################################


