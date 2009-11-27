#pragma once


#include <ExampleApplication.h>
#include <OgreNewt.h>

// CEGUI has issues with the Ogre memory manager, this seems to fix it for now.
//#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUI.h>
//#include <OgreMemoryMacros.h>


#include <OgreCEGUIRenderer.h>
#include <OgreCEGUIResourceProvider.h>




class OgreNewtonApplication :
	public ExampleApplication
{
public:
	OgreNewtonApplication(void);
	~OgreNewtonApplication(void);

protected:
	void createFrameListener();
	void createScene();

	void destroyScene();

	// our custom function to simplify making simpe dynamics rigid body boxes.
	OgreNewt::Body* makeSimpleBox( const Ogre::Vector3& size, const Ogre::Vector3& pos, const Ogre::Quaternion& orient );

    void makePlayer();

private:
	OgreNewt::World* m_World;
    OgreNewt::PlayerController *mPlayer;
	
	Ogre::FrameListener* mNewtonListener;

	int mEntityCount;

	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
};

