/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <OgreSceneManager.h>

#include "World.h"
#include "PhysicsManager.h"
#include "Exception.h"

namespace rl {

	World::World(Ogre::SceneType sceneType)
	    :   mSceneMgr(Root::getSingleton().getSceneManager( sceneType )),
            mCamera(0),
            mbSceneLoaded(false),
	        mActiveActor(0)

	{
	    new PhysicsManager(this);
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
	///@todo eventuell in BSPWorld ueberschreiben
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

	void World::setActiveActor(Actor* actor)
	{
		mActiveActor = actor;
	}

	Actor* World::getActiveActor() const
	{
		return mActiveActor;
	}

	Vector3 World::getStartPoint() const
	{
		//return mSceneMgr->getSuggestedViewpoint().position;
		return Vector3(0, 20, 40);
	}
	
    float* World::getVerticesPtr()
    {
        Throw(OperationNotSupportedException,
            "SceneManager does not support TriMesh-Retrieval");
    }

	int* World::getIndexPtr()
	{
        Throw(OperationNotSupportedException,
            "SceneManager does not support TriMesh-Retrieval");
	}

	int World::getVerticeCount()
	{
        Throw(OperationNotSupportedException,
            "SceneManager does not support TriMesh-Retrieval");
	}

	int World::getIndexCount()
	{
        Throw(OperationNotSupportedException,
            "SceneManager does not support TriMesh-Retrieval");
	}
}
