#pragma once
#include <ExampleApplication.h>

#include <OgreNewt.h>
#include "SimpleVehicle.h"


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
	
	Ogre::FrameListener* mNewtonListener;

	SimpleVehicle* mCar;


};

