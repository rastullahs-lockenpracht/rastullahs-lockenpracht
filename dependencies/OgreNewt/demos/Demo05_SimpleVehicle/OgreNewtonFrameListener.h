#pragma once
#include "ExampleFrameListener.h"

#include <OgreNewt.h>
#include "SimpleVehicle.h"


class OgreNewtonFrameListener :
	public ExampleFrameListener
{
protected:
	OgreNewt::World* m_World;
	SceneManager* mSceneMgr;
	int count;
	float timer;
	bool mR;

	SimpleVehicle* mCar;

public:
	OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W, SimpleVehicle* car);
	~OgreNewtonFrameListener(void);

	bool frameStarted(const FrameEvent &evt);
};

