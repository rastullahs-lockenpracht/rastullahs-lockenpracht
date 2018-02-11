/* This source file is part of Rastullahs Lockenpracht.
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

#include "XmlResourceManager.h"

#include "GenericWorld.h"


#include "CoreSubsystem.h"
#include "ConfigurationManager.h"
#include "ActorManager.h"
#include "Actor.h"
#include "PhysicsManager.h"
#include "ZoneManager.h"

using namespace Ogre;

namespace rl {

    GenericWorld::GenericWorld( )
        :   World(ST_GENERIC)
    {
        mSceneFile = "";
    }

    GenericWorld::~GenericWorld()
    {
        clearScene();
    }

    void GenericWorld::initializeDefaultCamera(void)
    {
        if (mCamera == 0)
        {
            // Kamera erstellen..
            Actor* actor = ActorManager::getSingleton().createCameraActor("DefaultCamera");
            // und initialisieren.
            mCamera = mSceneMgr->getCamera("DefaultCamera");

            mCamera->setPosition( getStartPoint() );
            mCamera->setFOVy(Degree(60));
            mCamera->setFixedYawAxis(true);
            mCamera->setAutoAspectRatio(true);

            actor->placeIntoScene();

			// Ein Viewport, das komplette Fenster
			Viewport* newVp = CoreSubsystem::getSingleton().getRenderWindow()->addViewport(mCamera, 1);

			// Schwarzer Hintergrund
			newVp->setBackgroundColour(ColourValue(0,0,0));

		}
    }

    void GenericWorld::clearScene()
    {
        fireBeforeClearScene();

        // This is necessary to destroy cameras too.
        CoreSubsystem::getSingleton().getRenderWindow()->removeAllViewports();
        ZoneManager::getSingleton().destroyAllZones();
        ActorManager::getSingleton().destroyAllActors();
        mSceneMgr->clearScene();
        PhysicsManager::getSingleton().clearLevelGeometry();

        mSceneFile = "";
        mCamera = NULL;
    }

    void GenericWorld::setCastShadows(bool enabled)
    {
		bool castShadows = false;

		if( enabled )
		{
			Ogre::String tmp =
				ConfigurationManager::getSingleton().getStringSetting(
					"Video", "Cast Shadows" );

			if (tmp == "yes")
				castShadows = true;
			else if (tmp == "no")
				castShadows = false;
		}

		/// @todo Settings for multiple Shadow-Types?
		if( castShadows )
		{
			int textureSize =
				ConfigurationManager::getSingleton().getIntSetting(
					"Video", "Shadow Texture Size" );
			textureSize = std::max( 64, std::min( 4096, textureSize ) );

			mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
			mSceneMgr->setShadowTextureSize( textureSize );
			mSceneMgr->setShadowColour(ColourValue(0.7, 0.7, 0.7));
			mSceneMgr->setShadowFarDistance(8.0f);
			mSceneMgr->setShadowDirLightTextureOffset(0.8f);
            mSceneMgr->setShadowCameraSetup((Ogre::ShadowCameraSetupPtr) new Ogre::LiSPSMShadowCameraSetup());
		}
		else
		{
			mSceneMgr->setShadowTechnique( SHADOWTYPE_NONE);
		}
    }
}
