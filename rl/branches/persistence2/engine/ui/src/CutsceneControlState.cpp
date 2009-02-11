/*
* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "CutsceneControlState.h"
#include "Exception.h"
#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "GameObject.h"
#include "PhysicalThing.h"

using namespace Ogre;

namespace rl {

	CutsceneControlState::CutsceneControlState(CommandMapper* cmdMapper,
        Actor* camera) : ControlState(cmdMapper, camera, NULL, CST_CUTSCENE),
        mTarget(NULL)
	{
	}

	CutsceneControlState::~CutsceneControlState()
	{
	}

    void CutsceneControlState::pause()
    {
		mCameraActor->getPhysicalThing()->unfreeze();
    }

    void CutsceneControlState::resume()
    {
		mCameraActor->getPhysicalThing()->freeze();
		Camera* ogreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
		ogreCam->setFixedYawAxis(true);
		mCameraActor->_getSceneNode()->setFixedYawAxis(true);
    }

	void CutsceneControlState::run(Real elapsedTime)
	{
	    CameraPosition pos = getBestCameraPosition();
	    switch (pos.type)
	    {
	    case CPT_FIXED:
	        setCameraPosition(pos.position);
	        setCameraOrientation(pos.orientation1);
	        break;
	    case CPT_ROTATING:
	        setCameraPosition(pos.position);
	        lookAt(mTarget->getPosition());
	        break;
	    }
	}

	void CutsceneControlState::setCameraTarget(GameObject* target)
	{
	    mTarget = target;
	}

    void CutsceneControlState::setCameraPosition(const Vector3& pos)
    {
        mCameraActor->setPosition(pos);
    }

    void CutsceneControlState::setCameraOrientation(const Quaternion& orient)
    {
        mCameraActor->setOrientation(orient);
    }

    void CutsceneControlState::lookAt(const Vector3& point)
    {
        dynamic_cast<Camera*>(mCameraActor->getControlledObject()->getMovableObject())
            ->lookAt(point);
    }

    CameraPosition CutsceneControlState::getBestCameraPosition() const
    {
        if (mPositions.empty())
        {
            CameraPosition nullPos = {CPT_FIXED, Vector3::ZERO, Quaternion::IDENTITY, Quaternion::IDENTITY};
            return nullPos;
        }
        return *mPositions.begin();
    }

    void CutsceneControlState::addCamera(const Vector3& position, const Quaternion& orientation)
    {
        CameraPosition cpos = {CPT_FIXED, position, orientation, orientation};
        mPositions.push_back(cpos);
    }

    void CutsceneControlState::addCamera(const Vector3& position, const Quaternion& orientation1,
            const Quaternion& orientation2)
    {
        CameraPosition cpos = {CPT_ROTATING, position, orientation1, orientation2};
        mPositions.push_back(cpos);
    }
}
