// ======================================================================================
// File         : device.c
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 11:08:41 AM | Saturday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

// SDL
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _init_window ()
{
    // Set the refresh rate. the nvidia graphic-card turn on the vsync when use opengl.
    // we need to turn this off.
#if (EX_PLATFORM == EX_WIN32)
    const char* extensions = (const char *)glGetString( GL_EXTENSIONS );
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

    if( strstr( extensions, "WGL_EXT_swap_control" ) != 0 )
    {
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
        if ( wglSwapIntervalEXT )
        {
            // TODO: bool useVSync = core_t::config()->get<bool>( "VSync" );
            bool useVSync = true;
            if ( useVSync )
                wglSwapIntervalEXT(1);
            else
                wglSwapIntervalEXT(0);
        }
    }
#endif

    glClearColor(0.0f,0.0f,0.0f,1.0f);	// Black Background

    // disable 3D uses
    glDisable( GL_LIGHTING );

    // depth
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

    // stencil
    // glEnable(GL_STENCIL_TEST);
    // glClearStencil(0);

    // cull face
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT, GL_FILL); // glPolygonMode(GL_FRONT, GL_LINE); // wireframe

    // Enable 2D texture
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture( GL_TEXTURE_2D, 0 );

    // misc setup
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH); // glShadeModel(GL_FLAT); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_init_sdl_gl ()
{
    uint32 vflags, bbColor, multiSample;
    uint32 width, height;
    SDL_Surface* pScreen;

    // init sdl video subsystem
    ex_log ("Init SDL Video");
    if ( SDL_InitSubSystem ( SDL_INIT_VIDEO ) )
    {
        // TODO: ex_check_return ( eResult_InitFailed, "Could not initialize SDL video: %s", SDL_GetError() );
    }

    // setup sdl video flag
    vflags = SDL_HWSURFACE|SDL_OPENGLBLIT;
    // TODO: if ( core_t::config()->get<bool>("Fullscreen") )
    if (0)
    {
        vflags |= SDL_FULLSCREEN;
    }

    // get back buffer color format
    // In order to use SDL_OPENGLBLIT we have to set GL attributes first colors 

    // TODO { 
    // bbColor = core_t::config()->get<uint32>("Color");
    bbColor = 32;
    // } TODO end 

    if ( bbColor == 16 )
    {
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    }
    else if ( bbColor == 32 )
    {
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    }
    else
    {
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    }

    // set gl doublebuffering
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // multisample 
    // TODO { 
    // multiSample = core_t::config()->get<uint32>("MultiSample");
    multiSample = 0;
    // } TODO end 
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, multiSample ) ;

    // TODO { 
    // width = core_t::config()->get<uint32>("BackBufferWidth");
    // height = core_t::config()->get<uint32>("BackBufferHeight");
    width = 640;
    height = 480;
    // } TODO end 

    pScreen = SDL_SetVideoMode ( width, height,  bbColor, vflags );

    //
    if ( pScreen == NULL )
    {
        // TODO: ex_check ( eResult_InitFailed, "Unable to set SDL video: %s", SDL_GetError() );
        // TODO: return eResult_InitFailed;
    }

    // A key event! God knows all the time I've spent just to figure out this basic thing. We have to open the Screen Alpha Channel!
    SDL_FillRect(pScreen, NULL, SDL_MapRGBA(pScreen->format,0,0,0,0));

    //
    _init_window ();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_begin_frame ()
{
    int mask = 0;

    mask |= GL_COLOR_BUFFER_BIT; 
    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );

    mask |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(1.0f);

    glClear (mask);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_end_frame ()
{
    SDL_GL_SwapBuffers();
}
