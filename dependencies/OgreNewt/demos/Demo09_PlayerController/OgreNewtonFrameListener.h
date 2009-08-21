#pragma once
#include "ExampleFrameListener.h"
#include <OgreNewt.h>
#include <OIS.h>
#include <CEGUI.h>


class OgreNewtonFrameListener :
	public ExampleFrameListener,
    public OIS::KeyListener,
    public OIS::MouseListener
{
protected:
	OgreNewt::World* m_World;
	SceneManager* mSceneMgr;
	int count;
    OgreNewt::PlayerController* mPlayer;
public:
	OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W, OgreNewt::PlayerController* player);
	~OgreNewtonFrameListener(void);

	bool frameStarted(const FrameEvent &evt);

	bool mouseMoved (const OIS::MouseEvent &e)
	{
		CEGUI::System::getSingleton().injectMouseMove( e.state.X.rel, e.state.Y.rel );
		return true;
	}

	bool keyPressed( const OIS::KeyEvent &arg )
	{
		CEGUI::System::getSingleton().injectKeyDown( arg.key );
		CEGUI::System::getSingleton().injectChar( arg.text );
		return true;
	}

	bool keyReleased( const OIS::KeyEvent &arg )
	{
		CEGUI::System::getSingleton().injectKeyUp( arg.key );
		return true;
	}

	bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
		return true;
	}

	bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
		return true;		
	}

protected:
	CEGUI::MouseButton convertOISMouseButtonToCegui(int id)
	{
		switch (id)
		{
			case 0: return CEGUI::LeftButton;
			case 1: return CEGUI::RightButton;
			case 2:	return CEGUI::MiddleButton;
			case 3: return CEGUI::X1Button;
			default: return CEGUI::LeftButton;
		}

	}
};

