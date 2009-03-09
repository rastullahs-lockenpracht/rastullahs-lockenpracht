#pragma once
#include "ExampleFrameListener.h"

#include <OgreNewt.h>


class OgreNewtonFrameListener :
	public ExampleFrameListener
{
protected:
	OgreNewt::World* m_World;
	SceneManager* mSceneMgr;
	int count;
	float timer;

public:
	OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W);
	~OgreNewtonFrameListener(void);

	bool frameStarted(const FrameEvent &evt);

};
