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
#include "Actor.h"
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
        clearScene();
    }

    void DotSceneOctreeWorld::initializeDefaultCamera(void)
    {
        if (mCamera == 0)
        {
            // Kamera erstellen..
            Actor* actor = ActorManager::getSingleton().createCameraActor("DefaultCamera");
            // und initialisieren.
            mCamera = mSceneMgr->getCamera("DefaultCamera");

            mCamera->setPosition( getStartPoint() );
            mCamera->setFOVy(Degree(60));
            mCamera->setFixedYawAxis(false);
            mCamera->setAutoAspectRatio(true);

            actor->placeIntoScene();
            setActiveActor( actor );            
        }

        // Ein Viewport, das komplette Fenster
        Viewport* newVp = Ogre::Root::getSingletonPtr()->
            getAutoCreatedWindow()->addViewport(mCamera, 1);

        // Schwarzer Hintergrund
        newVp->setBackgroundColour(ColourValue(0,0,0));
    }

    void DotSceneOctreeWorld::loadScene(const String& levelName, const String& module)
    {   
        // Alte Szene löschen
        clearScene();

        mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
        mSceneMgr->setShadowTextureSize(1024);
        mSceneMgr->setShadowColour(ColourValue(0.7, 0.7, 0.7));
        mSceneMgr->setShadowFarDistance(8.0f);
        mSceneMgr->setShadowDirLightTextureOffset(0.8f);

        // Leerer String, keine Map laden
        if( levelName.length()  )
        {
            /// TODO - In den Sky-Sonnenpart verschieben
            mSceneMgr->setAmbientLight(ColourValue(0.55, 0.55, 0.55));
            mSceneFile = levelName;
            
            DotSceneLoader* dot = NULL;
            try
            {
                dot = new DotSceneLoader( mSceneFile, module );
                dot->initializeScene( mSceneMgr );
                delete dot;
            }
            catch( ... )
            {
                LOG_CRITICAL(Logger::CORE, 
                    "Laden der Szenenbeschreibung aus '" + mSceneFile + "' ist fehlgeschlagen." );
                delete dot;
            }
        }
        else
            mSceneFile = "";

        initializeDefaultCamera();
        fireAfterSceneLoaded();
    }

    void DotSceneOctreeWorld::clearScene()
    {
        fireBeforeClearScene();

        // This is necessary to destroy cameras too.
        Ogre::Root::getSingleton().getAutoCreatedWindow()->removeAllViewports();
        ActorManager::getSingleton().destroyAllActors();
        mSceneMgr->clearScene();
        PhysicsManager::getSingleton().clearLevelGeometry();

        mSceneFile = "";
        mCamera = NULL;
    }

    void DotSceneOctreeWorld::setCastShadows(bool enabled)
    {
        mSceneMgr->setShadowTechnique(enabled ? SHADOWTYPE_TEXTURE_MODULATIVE : SHADOWTYPE_NONE);
    }
}
