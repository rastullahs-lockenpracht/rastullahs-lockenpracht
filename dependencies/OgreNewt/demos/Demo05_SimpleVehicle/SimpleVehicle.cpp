#include "SimpleVehicle.h"

int SimpleVehicle::mEntityCount = 5;

// SimpleVehicle constructor.  this creates and sets up the entire vehicle!
SimpleVehicle::SimpleVehicle(Ogre::SceneManager* mgr, OgreNewt::World* world, Ogre::Vector3& position, Ogre::Quaternion& orient) : OgreNewt::Vehicle()
{
	// save the scene manager
	mSceneMgr = mgr;
	mWorld = world;


	// first thing we need to do is create the rigid body for the main chassis.
	Ogre::Vector3 size(5,1.2,2);
	OgreNewt::Body* bod = makeSimpleBox(size, position, orient);

	//now that we have defined the chassis, we can call the "init()" function.  this is a helper function that
	// simply sets up some internal wiring of the vehicle class that makes eveything work :)  it also calls the virtual
	// function "setup" to finish building the vehicle.
	// you pass this function the body to be used as the main chassis, and the up direction of the world (for suspension purposes).
	init( bod, Ogre::Vector3(0,1,0) );

	// the above function calls our "setup" function, which takes care of the rest of the vehicle setup.
	
}


// virtual function - setup().  this actually builds the tires, etc.
void SimpleVehicle::setup()
{
	// okay, we have the main chassis all setup.  let's do a few things to it:
	m_chassis->setStandardForceCallback();
	// we don't want the vehicle to freeze, because we'll be unable to control it.
	m_chassis->setAutoFreeze(0);


	// okay, let's add tires!
	// all offsets here are in local space of the vehicle.
	Ogre::Vector3 offset(1.8,-1.6,0.87);

	int tireNum = 0;
	for (int x=-1;x<=1;x+=2)
	{
		for (int z=-1;z<=1;z+=2)
		{

			// okay, let's create the tire itself.  we'll use the OgreNewt::Vehicle::Tire class for this.  most of the
			// parameters are self-explanatory... try changing some of them to see what happens.
			Ogre::Quaternion	tireorient = Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3::UNIT_Y);
			Ogre::Vector3		tirepos = offset * Ogre::Vector3(x,0.5f,z);
			Ogre::Vector3		pin(0,0,x);
			Ogre::Real			mass = 15.0;
			Ogre::Real			width = 0.3;
			Ogre::Real			radius = 0.5;
			Ogre::Real			susShock = 30.0;
			Ogre::Real			susSpring = 200.0;
			Ogre::Real			susLength = 1.2;
			bool				steering;


			// first, load the visual mesh that represents the tire.
			Ogre::Entity* ent = mSceneMgr->createEntity("Tire"+Ogre::StringConverter::toString(mEntityCount++), "wheel.mesh");
			// make a scene node for the tire.
			Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject( ent );
			node->setScale( Ogre::Vector3(radius, radius, width) );

			if (x > 0)
				steering = true;
			else
				steering = false;

			// create the actual tire!
			SimpleTire* tire = new SimpleTire(this, tireorient, tirepos, pin, mass, width, radius,
				susShock, susSpring, susLength, 0, steering);

			// attach the tire to the node.
			tire->attachToNode( node );
			
		}
	}

}
			

SimpleVehicle::SimpleTire::~SimpleTire()
{
	// destroy entity, and scene node.
	Ogre::Entity* ent = (Ogre::Entity*)m_node->getAttachedObject(static_cast<unsigned short>(0));
	m_node->detachAllObjects();
	m_node->getCreator()->destroyEntity( ent );
	m_node->getParentSceneNode()->removeAndDestroyChild( m_node->getName() );
}

SimpleVehicle::~SimpleVehicle(void)
{
	std::vector< SimpleTire* > toDelete;

	// delete tire objects.
	for (SimpleTire* tire = (SimpleTire*)getFirstTire(); tire; tire=(SimpleTire*)getNextTire(tire))
	{
		toDelete.push_back( tire );
	}

	while (!toDelete.empty())
	{
		SimpleTire* tire = toDelete.back();
		delete tire;
		toDelete.pop_back();
	}

	// finally, destroy entity and node from chassis.
	Ogre::Entity* ent = (Ogre::Entity*)((Ogre::SceneNode*)m_chassis->getOgreNode())->getAttachedObject(static_cast<unsigned short>(0));
	((Ogre::SceneNode*)m_chassis->getOgreNode())->detachAllObjects();
	((Ogre::SceneNode*)m_chassis->getOgreNode())->getCreator()->destroyEntity( ent );
	((Ogre::SceneNode*)m_chassis->getOgreNode())->getParentSceneNode()->removeAndDestroyChild( m_chassis->getOgreNode()->getName() );

	destroy();
}



// This is the important callback, which is the meat of controlling the vehicle.
void SimpleVehicle::userCallback()
{

	// foop through wheels, adding torque and steering, and updating their positions.
	for (SimpleTire* tire = (SimpleTire*)getFirstTire(); tire; tire=(SimpleTire*)getNextTire(tire))
	{
		// set the torque and steering!  non-steering tires get the torque.
		
		// is this a steering tire?
		if (tire->mSteeringTire)
			tire->setSteeringAngle( mSteering );
		else
			tire->setTorque( mTorque );

		// finally, this command updates the location of the visual mesh.
		tire->updateNode();
	}

}







OgreNewt::Body* SimpleVehicle::makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient )
{
	Ogre::Entity* box1;
	Ogre::SceneNode* box1node;

	box1 = mSceneMgr->createEntity( "Entity"+Ogre::StringConverter::toString(mEntityCount++), "box.mesh" );
	box1node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	box1node->attachObject( box1 );
	box1node->setScale( size );

	OgreNewt::ConvexCollision* col = new OgreNewt::CollisionPrimitives::Box( mWorld, size );
	OgreNewt::Body* bod = new OgreNewt::Body( mWorld, col );


	// base mass on the size of the object.
	Ogre::Real mass = size.x * size.y * size.z * 100.0;
	// calculate the inertia based on box formula and mass
	Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

	delete col;

				
	bod->attachNode( box1node );
	bod->setMassMatrix( mass, mass*inertia );
    bod->setCenterOfMass(offset);
	bod->setStandardForceCallback();

	box1->setMaterialName( "Simple/BumpyMetal" );
	box1->setNormaliseNormals(true);


	bod->setPositionOrientation( pos, orient );

	return bod;
}

