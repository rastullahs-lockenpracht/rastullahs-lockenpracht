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

// Xerces geht vor allen Ogre includes...
#include "XmlResourceManager.h"

#include "DotSceneOctreeWorld.h"


#include <OgreTextureManager.h>
#include <OgreRoot.h>
#include <OgreException.h>

#include "ActorManager.h"
#include "PhysicsManager.h"
#include "DotSceneLoader.h"




namespace rl {

    DotSceneOctreeWorld::DotSceneOctreeWorld( )
        :   World(ST_GENERIC)
    {
		 mSceneFile = "";
    }

    DotSceneOctreeWorld::~DotSceneOctreeWorld()
    {
		if( mSceneFile.length() != 0 )
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

			// 0, 0, 0
            mCamera->setPosition( getStartPoint() );
            mCamera->setFOVy(Degree(60));
            mCamera->setFixedYawAxis(false);
        }

        // Ein Viewport, das komplette Fenster
        Viewport* newVp = Ogre::Root::getSingletonPtr()->
            getAutoCreatedWindow()->addViewport(mCamera, 1);

		// Schwarzer Hintergrund
        newVp->setBackgroundColour(ColourValue(0,0,0));
    }

    void DotSceneOctreeWorld::loadScene(const String& levelName)
    {
        if( mSceneFile.length() != 0 )
            clearScene();

		// Leerer String, keine Map laden
		if( levelName.length() == 0 )
			return;

		/// TODO - In den Sky-Sonnenpart verschieben
		mSceneMgr->setAmbientLight(ColourValue(0.55, 0.55, 0.55));

		DotSceneLoader* dot = new DotSceneLoader( levelName );
		delete dot;
		mSceneFile = levelName;

        initializeDefaultCamera();
    }

    void DotSceneOctreeWorld::clearScene()
    {
        ActorManager::getSingleton().destroyAllActors();
        mSceneMgr->clearScene();
        Ogre::Root::getSingleton().getAutoCreatedWindow()->removeAllViewports(); 
		XmlResourceManager::getSingleton().unload(mSceneFile);
		PhysicsManager::getSingleton().clearLevelGeometry();
        mSceneMgr = Root::getSingleton().getSceneManager(ST_GENERIC);
		mSceneFile = "";
    }
    
	void DotSceneOctreeWorld::setCastShadows(bool enabled)
	{

	}
}
