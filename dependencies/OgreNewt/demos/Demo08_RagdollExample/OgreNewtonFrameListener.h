#pragma once
#include "ExampleFrameListener.h"
#include "Ragdoll.h"
#include <OgreNewt.h>


class OgreNewtonFrameListener :
	public ExampleFrameListener
{
protected:

	RagDoll*			mRagdoll;
	SceneNode*			mRagNode;
	Entity*				mRagEntity;
	OgreNewt::World*	m_World;
	SceneManager*		mSceneMgr;
	int					count;
	float				timer;
	bool				mK1;
	bool				mK2;

public:
	OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W);
	~OgreNewtonFrameListener(void);

	bool frameStarted(const FrameEvent &evt);

};

