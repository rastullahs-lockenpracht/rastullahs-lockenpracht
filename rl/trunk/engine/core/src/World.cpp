/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "World.h"

#include "Exception.h"
#include "CoreMessages.h"
#include "MessagePump.h"



using namespace Ogre;

namespace rl {

	World::World(SceneType sceneType)
		:   mSceneMgr(0),
            mCamera(0),
            mSceneFile(),
	        mActiveActor(0),
            mUniqueNameSeed(0)
	{
        mSceneMgr = Root::getSingleton()
            .createSceneManager(sceneType, "world_sm");
	}

	SceneManager* World::getSceneManager(void) const
	{
		return mSceneMgr;
	}

	void World::setSceneManager(SceneManager* SceneMgr)
	{
		mSceneMgr = SceneMgr;
	}

    //Enables / disables a 'sky plane' i.e.
    void World::setSkyPlane(bool enable, const Plane &plane,
        const String &materialName, Real scale, Real tiling,
        bool drawFirst, Real bow)
    {
        mSceneMgr->setSkyPlane(enable, plane, materialName, scale,
            tiling, drawFirst, bow);
    }

    void World::setAmbientLight(Real r, Real g, Real b)
    {
        mSceneMgr->setAmbientLight(ColourValue(r,g,b));
    }

    //Enables / disables a 'sky box' i.e.
    void World::setSkyBox(bool enable, const String &materialName, Real distance, bool drawFirst )
    {
        mSceneMgr->setSkyBox(enable, materialName,
			distance, drawFirst, Quaternion::ZERO);
    }

    //Enables / disables a 'sky dome' i.e.
    void World::setSkyDome(bool enable, const String &materialName,
        Real curvature, Real tiling, Real distance, bool drawFirst)
    {
        mSceneMgr->setSkyDome(enable, materialName, curvature,
            tiling, distance, drawFirst, Quaternion::ZERO);
    }

    //Sets the fogging mode applied to the scene.
    void World::setFog(FogMode mode, const ColourValue &colour,
        Real expDensity, Real linearStart, Real linearEnd)
    {
        mSceneMgr->setFog(Ogre::FogMode(mode), colour,
            expDensity, linearStart, linearEnd );
    }

    //Returns the fog mode for the scene.
    rl::World::FogMode World::getFogMode(void) const
    {
        return rl::World::FogMode(mSceneMgr->getFogMode( ));
    }

    //Returns the fog colour for the scene.
    const ColourValue& World::getFogColour(void) const
    {
        return mSceneMgr->getFogColour();
    }

    //Returns the fog start distance for the scene.
    Real World::getFogStart(void) const
    {
        return mSceneMgr->getFogStart();
    }

    //Returns the fog end distance for the scene.
    Real World::getFogEnd(void) const
    {
        return mSceneMgr->getFogEnd();
    }

	Vector3 World::getStartPoint() const
	{
        return Vector3::ZERO;
	}

	void World::setCastShadows(bool enabled)
	{
		Throw(OperationNotSupportedException,
			"SceneManager does not support shadows");
	}

    void World::setShowBoundingBoxes( bool dis )
    {
        mSceneMgr->showBoundingBoxes( dis );
    }

    Ogre::String World::getUniqueName()
    {
        return "__RL_WORLD_UNIQUE_NAME__" + StringConverter::toString(++mUniqueNameSeed);
    }

    void World::fireAfterSceneLoaded()
    {
        MessagePump::getSingleton().sendMessage<MessageType_SceneLoaded>();
    }

    void World::fireBeforeClearScene()
    {
        MessagePump::getSingleton().sendMessage<MessageType_SceneClearing>();
    }
}
