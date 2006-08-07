/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/

#include "TargetSelection.h"

#include <OgreNewt_World.h>

#include "Actor.h"
#include "CoreSubsystem.h"
#include "PhysicsManager.h"

using namespace OgreNewt;

namespace rl {

	TargetSelection::TargetSelection(Actor* character, Actor* camera)
		: mCharacter(character),
		  mCamera(camera)
	{
		OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

        const MaterialID* defaultID = world ->getDefaultMaterialID();
		mSelectionID = new MaterialID(world);

		mSelectionCollision = 
			new Body(
				world, 
				CollisionPtr(new CollisionPrimitives::Cylinder(world, 0.1, 5)));
		mSelectionCollision->setMaterialGroupID(mSelectionID);
		mOffset = Vector3(0, 1.5, -2.5);
		mOrientationBias.FromAngleAxis(Degree(90), Vector3::UNIT_Y);

		mDefaultSelectionPair = new MaterialPair(world, defaultID, mSelectionID); 
		mDefaultSelectionPair->setContactCallback(this);
		mCharSelectionPair = new MaterialPair(
			world, 
			PhysicsManager::getSingleton()._getCharMaterialID(), 
			mSelectionID);
		mCharSelectionPair->setContactCallback(this);
	}

	TargetSelection::~TargetSelection()
	{
		delete mSelectionCollision;
		delete mDefaultSelectionPair;
		delete mSelectionID;
	}

	void TargetSelection::update()
	{
		const Quaternion& camOrient = 
			mCamera->getWorldOrientation();

		const Quaternion& charOrient = 
			mCharacter->getWorldOrientation();

		mSelectionCollision->setPositionOrientation(
			charOrient * mOffset + mCharacter->getWorldPosition(), 
			mOrientationBias * camOrient);

		cleanSelection();
	}
	
	int TargetSelection::userProcess() 
	{
		Body* target = NULL;
		if (m_body0 == mSelectionCollision)
		{
			target = m_body1;
		}
		else if (m_body1 == mSelectionCollision)
		{
			target = m_body0;
		}

		if (target != NULL)
		{
			Actor* actor = static_cast<Actor*>(target->getUserData());
			if (actor != mCharacter && actor != mCamera)
				mSelectedObjects[actor] = CoreSubsystem::getSingleton().getClock();
		}

		//Ignore collision
		return 0; 
	}

	void TargetSelection::cleanSelection()
	{
		static RL_LONGLONG TIME_IN_SELECTION = 2000;
		RL_LONGLONG curTime = CoreSubsystem::getSingleton().getClock();

		for(std::map<Actor*, RL_LONGLONG>::iterator it = mSelectedObjects.begin();
			it != mSelectedObjects.end();)
		{
			if (curTime - (*it).second > TIME_IN_SELECTION)
			{
				it = mSelectedObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}


}
