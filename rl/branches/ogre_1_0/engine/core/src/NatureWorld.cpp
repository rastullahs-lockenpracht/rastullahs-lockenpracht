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

#include "NatureWorld.h"
#include "CameraActor.h"
#include "ActorManager.h"

namespace rl {

	NatureWorld::NatureWorld() : World(ST_EXTERIOR_FAR)
	{
	}

	NatureWorld::~NatureWorld()
	{
	}

	void NatureWorld::clearScene()
	{
	}

	void NatureWorld::doLoadScene(const String& levelName)
	{
		initializeDefaultCamera();

		Entity *waterEntity;
        Plane waterPlane;

        // Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        // create a water plane/scene node
        waterPlane.normal = Vector3::UNIT_Y; 
        waterPlane.d = -30; 
        MeshManager::getSingleton().createPlane(
            "WaterPlane",
            waterPlane,
            2800, 2800,
            20, 20,
            true, 1, 
            10, 10,
            Vector3::UNIT_Z
        );

        waterEntity = mSceneMgr->createEntity("water", "WaterPlane"); 
        waterEntity->setMaterialName("Examples/TextureEffect4"); 

        SceneNode *waterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterNode"); 
        waterNode->attachObject(waterEntity); 
        waterNode->translate(1000, 0, 1000);



        // Create a light
        Light* l = mSceneMgr->createLight("MainLight");
        // Accept default settings: point light, white diffuse, just set position
        // NB I could attach the light to a SceneNode if I wanted it to move automatically with
        //  other objects, but I don't
        l->setPosition(20,80,50);

        mSceneMgr->setWorldGeometry( levelName );

        setFog(FOG_EXP2, ColourValue(0.77, 0.86, 1.0), 0.0015, 0,0 );
		
		//mSceneMgr->setFog(FOG_LINEAR, ColourValue(0.77, 0.86, 1.0), 0, 150, 500);
       //mRoot -> showDebugOverlay( true );

      // 
	}

	void NatureWorld::initializeDefaultCamera()
	{
		// Also change position, and set Quake-type orientation
        // Get random player start point
		mCamera = ActorManager::getSingleton().createCameraActor("DefaultCamera");
		ViewPoint defaultVP = mSceneMgr->getSuggestedViewpoint(true);

		mCamera->setNearClipDistance(10);
        mCamera->setFarClipDistance(10000);
		mCamera->setPosition(-0.0, 400.0, 200.0);
		mCamera->lookAt(-128.0, 128.0, 24.0);
  
		// Quake uses X/Y horizon, Z up
		mCamera->setFOVy(60);
        // Don't yaw along variable axis, causes leaning
        mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);

		// Create one viewport, entire window
        Viewport* newVp = Ogre::Root::getSingleton().getAutoCreatedWindow()->addViewport(mCamera->getOgreCamera(),1);
        newVp->setBackgroundColour(ColourValue(0,0,0));
	}
}

