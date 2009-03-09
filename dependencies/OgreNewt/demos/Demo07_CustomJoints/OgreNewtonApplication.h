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

	// our custom function to simplify making simpe dynamics rigid body boxes.
	OgreNewt::Body* makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient );

private:
	OgreNewt::World* m_World;
	
	Ogre::FrameListener* mNewtonListener;

	int mEntityCount;

};

