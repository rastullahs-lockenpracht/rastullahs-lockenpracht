#pragma once
#include <ExampleApplication.h>

#include <OgreNewt.h>



class OgreNewtonApplication :
	public ExampleApplication
{
public:
	OgreNewtonApplication(void);
	~OgreNewtonApplication(void);

protected:
	void createFrameListener();
	void createScene();

private:
	OgreNewt::World* m_World;
	
//	Ogre::SceneNode* msnCam;

	Ogre::FrameListener* mNewtonListener;

};
