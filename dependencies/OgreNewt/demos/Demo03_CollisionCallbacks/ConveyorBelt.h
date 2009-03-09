#pragma once

#include <OgreNewt.h>

// conveyorBelt - simple class for creating conveyor belt objects!
class ConveyorBelt
{
public:
	ConveyorBelt(void);
	~ConveyorBelt(void);

	void init( Ogre::String& name, Ogre::SceneManager* mgr, OgreNewt::World* world, Ogre::Vector3& size, Ogre::Vector3& dir, Ogre::Real speed,
		Ogre::Vector3& pos, Ogre::Quaternion& orient, const OgreNewt::MaterialID* conveyorMat, int conveyorType );

	Ogre::Vector3 getGlobalDir();
	Ogre::Real getSpeed() { return mSpeed; }

private:
	void killme();

	OgreNewt::Body* mBody;
	
	Ogre::Vector3 mDir;
	Ogre::Real mSpeed;

	Ogre::SceneNode* mNode;

	Ogre::SceneManager* mSceneMgr;

};

