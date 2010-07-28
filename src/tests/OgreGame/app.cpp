// ======================================================================================
// File         : app.cpp
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 18:09:46 PM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "ogre_game.h"
#include "app.h"

///////////////////////////////////////////////////////////////////////////////
// internal value
///////////////////////////////////////////////////////////////////////////////

static const char* pluginFile = ""; // "plugins.cfg"; 
static const char* configFile = ""; // "ogre.cfg"; 
static const char* logFile = "ogre.log"; 

static const uint wnd_width = 1280;
static const uint wnd_height = 720;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

App::App ()
    : m_inited (false)
    , m_root (NULL)
    , m_window (NULL)
	, m_inputMgr (NULL)
	, m_mouse (NULL)
	, m_keyboard (NULL)
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

App::~App ()
{
    deinit ();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool App::init ()
{
    // if we already inited, don't init it second times.
    if ( m_inited == true )
        return true;

    // create ogre root 
    m_root = new Ogre::Root ( pluginFile, configFile, logFile );

    // load plugins
    typedef std::vector<Ogre::String> Strings;
    Strings plugin_names;
    plugin_names.push_back("RenderSystem_GL");
    // plugin_names.push_back("RenderSystem_Direct3D9");
    plugin_names.push_back("Plugin_ParticleFX");
    plugin_names.push_back("Plugin_CgProgramManager");
    //plugin_names.push_back("Plugin_PCZSceneManager");
    //plugin_names.push_back("Plugin_OctreeZone");
    plugin_names.push_back("Plugin_OctreeSceneManager");
    //plugin_names.push_back("Plugin_BSPSceneManager");
    {
        Strings::iterator iter = plugin_names.begin();
        Strings::iterator iterEnd = plugin_names.end();
        for( Strings::iterator iter = plugin_names.begin(); iter != plugin_names.end(); ++iter )
        {
            Ogre::String& plugin_name = *iter;
            bool is_debug = OGRE_DEBUG_MODE;
            if ( is_debug )
            {
                plugin_name.append("_d");
            }
            m_root->loadPlugin(plugin_name);
        }
    }

    // choose the renderer
    const Ogre::RenderSystemList& rs_list = m_root->getAvailableRenderers();
    if( rs_list.size() == 0 ) {
        ex_warning ( "no rendersystem was found!" );
        return false;
    }
    Ogre::RenderSystem* rs = rs_list[0];
    m_root->setRenderSystem(rs);

    // init root ( only when a rendersystem has been selected)
    m_root->initialise ( false, "", "" );

    // create a window
    initWindow ();

    // create inputs
    initInput ();

    //
	Ogre::Timer* timer = m_root->getTimer();
	timer->reset();
	m_window->setActive(true);
	m_window->setAutoUpdated(false);

    // 
    m_inited = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool App::deinit ()
{
    if ( m_inited == false )
        return true;

    // destroy inputs
    deinitInput ();

    //
    if ( m_window ) {
        m_window->removeAllViewports ();
        m_window = NULL;
    }

    //
    if ( m_root ) {
        delete m_root;
        m_root = NULL;
    }

    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void App::deinitInput ()
{
    if ( m_inputMgr ) {
        if ( m_keyboard ) {
            m_inputMgr->destroyInputObject(m_keyboard);
            m_keyboard = NULL;
        }
        if ( m_mouse ) {
            m_inputMgr->destroyInputObject(m_mouse);
            m_mouse = NULL;
        }
		OIS::InputManager::destroyInputSystem(m_inputMgr);
		m_inputMgr = NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void App::tick ()
{
    if ( m_inited == false )
        return;

    // handle input
    m_keyboard->capture();
    m_mouse->capture();

    // draw to window
    bool vsync = true;
    m_window->update (false);
    m_window->swapBuffers (vsync); 

    // update internal counters and listeners
    m_root->renderOneFrame();

    //
    Ogre::WindowEventUtilities::messagePump();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void App::initWindow ()
{
    const char* wnd_title = "Ogre Game";
    uint width = wnd_width;
    uint height = wnd_height;
    bool fullScreen = false;
    Ogre::NameValuePairList params;
    params["FSAA"] = "0";
    params["vsync"] = "true";
    params["currentGLContext"] = "true";
    m_window = m_root->createRenderWindow( wnd_title, width, height, fullScreen, &params );
    m_window->setVisible (true);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void App::initInput ()
{
    // The input/output system needs an handle to a window as input, in a string format.
    // we need to get the "windows-API" handle
    size_t wnd_handle = 0;
    m_window->getCustomAttribute("WINDOW", &wnd_handle);

    // convert it into a string
    std::string str_wnd_handle ="";
    {
        std::ostringstream windowHndStr;
        windowHndStr << wnd_handle;
        str_wnd_handle = windowHndStr.str();
    }
    OIS::ParamList spec_params;
    spec_params.insert( std::make_pair( std::string("WINDOW"), str_wnd_handle ) );

    // we create the input/output system itself.
    m_inputMgr = OIS::InputManager::createInputSystem( spec_params );

    // creation of the keyboard-representing object
    m_keyboard = static_cast<OIS::Keyboard*>(m_inputMgr->createInputObject(OIS::OISKeyboard, false));

    // creation of the mouse-representing object
    m_mouse = static_cast<OIS::Mouse*>(m_inputMgr->createInputObject( OIS::OISMouse, false));

    // then must tell the mouse how much it is allowed to move.
    const OIS::MouseState& infoSouris = m_mouse->getMouseState();
    infoSouris.width = m_window->getWidth();
    infoSouris.height = m_window->getHeight();
}
