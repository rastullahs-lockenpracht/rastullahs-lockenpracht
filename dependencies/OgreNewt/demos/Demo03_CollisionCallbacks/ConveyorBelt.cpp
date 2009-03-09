#include "ConveyorBelt.h"


ConveyorBelt::ConveyorBelt(void)
{
}

ConveyorBelt::~ConveyorBelt(void)
{
	if (mBody)
		killme();
}

void ConveyorBelt::init( Ogre::String& name, Ogre::SceneManager* mgr, OgreNewt::World* world, Ogre::Vector3& size, Ogre::Vector3& dir, Ogre::Real speed,
						Ogre::Vector3& pos, Ogre::Quaternion& orient, const OgreNewt::MaterialID* conveyorMat, int conveyorType )
{
	// build a conveyor belt object.  first create the basic visual object.
	mSceneMgr = mgr;
	mDir = dir;
	mSpeed = speed;

	mNode = mgr->getRootSceneNode()->createChildSceneNode();

	Ogre::Entity* ent = mSceneMgr->createEntity(name, "box.mesh");
	ent->setMaterialName(name);
	mNode->attachObject(ent);
	mNode->setScale( size );

	// create the collision object for the conveyor belt.
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::Box( world, size );
	mBody = new OgreNewt::Body( world, col, conveyorType );
	delete col;

	mBody->setMassMatrix( 0.0, Ogre::Vector3(0,0,0) );
	mBody->attachNode( mNode );
	mBody->setMaterialGroupID( conveyorMat );

	mBody->setUserData( this );

	mBody->setPositionOrientation( pos, orient );
	mNode->setPosition( pos );
	mNode->setOrientation( orient );
}


Ogre::Vector3 ConveyorBelt::getGlobalDir()
{
	Ogre::Vector3 ret = mNode->_getDerivedOrientation() * mDir;
	return ret;
}

void ConveyorBelt::killme()
{
	// first destroy the rigid body.
	delete mBody;
	mBody = NULL;

	//now destroy the scene node and entity.
	while (mNode->numAttachedObjects() > 0)
	{
		Ogre::Entity* ent = (Ogre::Entity*)mNode->getAttachedObject(0);
		mSceneMgr->destroyEntity( ent );
	}

	mSceneMgr->getRootSceneNode()->removeAndDestroyChild( mNode->getName() );

	mNode = NULL;
}

