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

#include "DotSceneOctreeWorld.h"

#include <OgreTextureManager.h>
#include <OgreRoot.h>
#include <OgreException.h>
#include "GameLoop.h"

#include "ActorManager.h"
#include "PhysicsManager.h"

namespace rl {

    DotSceneOctreeWorld::DotSceneOctreeWorld( )
        :   World(ST_GENERIC),
            mSceneEntity(0)
    {
        // Set up shadowing
        mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
        mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
        mSceneMgr->setShadowFarDistance(10000);
        mSceneMgr->setShadowDirLightTextureOffset(0.9);
		if (StringUtil::startsWith(Root::getSingletonPtr()->
		    getRenderSystem()->getName(), "direct"))
        {
            // In D3D, use a 1024x1024 shadow texture
            mSceneMgr->setShadowTextureSettings(1024, 2);
        }
        else
        {
            // Use 512x512 texture in GL since we can't go higher than the window res
            mSceneMgr->setShadowTextureSettings(512, 2);
        }
    }

    DotSceneOctreeWorld::~DotSceneOctreeWorld()
    {
        clearScene();
    }

    void DotSceneOctreeWorld::initializeDefaultCamera(void)
    {
        if (mCamera == 0)
        {
            // Kamera erstellen..
            ActorManager::getSingleton().createCameraActor("DefaultCamera");
            // und initialisieren.
            mCamera = mSceneMgr->getCamera("DefaultCamera");
            ViewPoint defaultVP = mSceneMgr->getSuggestedViewpoint(true);

            mCamera->setPosition( defaultVP.position );

            // Quake uses X/Y horizon, Z up
            mCamera->setFOVy(Degree(60));

            mCamera->setFixedYawAxis(false);

        }
        // Create one viewport, entire window
        Viewport* newVp = Ogre::Root::getSingletonPtr()->
            getAutoCreatedWindow()->addViewport(mCamera, 1);
        newVp->setBackgroundColour(ColourValue(0,0,0));
    }

    void DotSceneOctreeWorld::doLoadScene(const String& levelName)
    {
        if( mbSceneLoaded )
            clearScene();

        // Add some default lighting to the scene
        mSceneMgr->setAmbientLight(ColourValue(0.55, 0.55, 0.55));

		/// Create the visual entity and scene node
		/// und das Level korrekt abgebaut wird.
		mSceneEntity = mSceneMgr->createEntity("level", levelName);
		SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("level");
		node->attachObject(mSceneEntity);

		//Level wirft per Default keinen Schatten
		setCastShadows(false);
		
        initializeDefaultCamera();
        mbSceneLoaded = true;
    }

    void DotSceneOctreeWorld::clearScene()
    {
		try {
		    mSceneMgr->destroySceneNode("level");
		}
		catch (Ogre::Exception&)
		{
		    // ignorieren. Gab es den Node halt nicht.
		}
		
		try {
		    mSceneMgr->removeEntity("level");
		}
		catch (Ogre::Exception&)
		{
		    // ignorieren. Gab es die Entity halt nicht.
		}
		mSceneEntity = 0;
		
        Ogre::Root::getSingleton().getAutoCreatedWindow()->removeAllViewports();

        ActorManager::getSingleton().destroyAllActors();

        mSceneMgr = Root::getSingleton().getSceneManager(ST_GENERIC);
        PhysicsManager::getSingleton().createLevelGeometry(0);
        mbSceneLoaded = false;
    }
    
    Entity* DotSceneOctreeWorld::getSceneEntity()
    {
        return mSceneEntity;
    }

	void DotSceneOctreeWorld::setCastShadows(bool enabled)
	{
		mSceneEntity->setCastShadows(enabled);
	}
}
