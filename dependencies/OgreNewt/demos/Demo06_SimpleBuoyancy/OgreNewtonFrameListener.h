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
	float timer;

	bool dragging;
	Ogre::Vector3 dragPoint;
	Ogre::Real dragDist;
	OgreNewt::Body* dragBody;

	// for drawing 3D lines...
	Ogre::SceneNode* mDragLineNode;
	Ogre::ManualObject* mDragLine;

	void remove3DLine();

public:
	OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W);
	~OgreNewtonFrameListener(void);

	bool frameStarted(const FrameEvent &evt);

	// special callback for applying drag force to bodies.
	void dragCallback( OgreNewt::Body* me, float timestep, int threadindex );

    // standard callback that does also apply the buoyancy force
	void standardForceCallback( OgreNewt::Body* me, float timestep, int threadindex );

	/////////////////////////////////////////////////////////////////
	//	Buoyancy callback - necesary to implement buoyancy!
	bool buoyancyCallback( int colID, OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos, Ogre::Plane& plane );

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

