// ======================================================================================
// File         : app.h
// Author       : Wu Jie 
// Last Change  : 07/18/2010 | 18:08:34 PM | Sunday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef APP_H_1279447716
#define APP_H_1279447716
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// class App 
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

class App
{
    ///////////////////////////////////////////////////////////////////////////////
    // public member functions
    ///////////////////////////////////////////////////////////////////////////////

public:

    App ();
    ~App ();

    bool init ();
    bool deinit ();
    void tick ();

    Ogre::RenderWindow *window () const { return m_window; }

    ///////////////////////////////////////////////////////////////////////////////
    // protected member functions
    ///////////////////////////////////////////////////////////////////////////////

protected:

    void initWindow ();
    void initInput ();

    void deinitInput ();

    ///////////////////////////////////////////////////////////////////////////////
    // protected member functions
    ///////////////////////////////////////////////////////////////////////////////

protected:

    bool m_inited;
    Ogre::Root *m_root;
    Ogre::RenderWindow *m_window;

	OIS::InputManager *m_inputMgr;
	OIS::Mouse *m_mouse;
	OIS::Keyboard *m_keyboard;

}; // end class App

// #################################################################################
#endif // END APP_H_1279447716
// #################################################################################

