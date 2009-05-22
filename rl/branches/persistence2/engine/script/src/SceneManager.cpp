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

#include "SceneManager.h"

#include "CoreSubsystem.h"
#include "Creature.h"
#include "CreatureControllerManager.h"
#include "GameObjectManager.h"
#include "PartyManager.h"
#include "RulesMessages.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "SaveAbleManager.h"

template<> rl::SceneManager* Ogre::Singleton<rl::SceneManager>::ms_Singleton = NULL;

namespace rl 
{

    SceneManager::SceneManager()
        : Ogre::Singleton<SceneManager>(),
          Ogre::ScriptLoader(),
          mCurrentScene(NULL)
    {
        mScriptPatterns.push_back("*.rlscene");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    }

    SceneManager::~SceneManager()
    {
        Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
        for (std::map<CeGuiString, Scene*>::iterator it = mScenes.begin();
            it != mScenes.end(); ++it)
        {
            delete it->second;
        }
    }

    void SceneManager::addScene(Scene* scene) 
    {
        mScenes[scene->getName()] = scene;
    }

    void SceneManager::loadScene(const CeGuiString& sceneName, bool saveCurrent)
    {
        if (mCurrentScene && sceneName == mCurrentScene->getName())
        {
            return;
        }

        std::map<CeGuiString, Scene*>::iterator itScene = mScenes.find(sceneName);
        if (itScene != mScenes.end())
        {  
            Creature* activeChar = PartyManager::getSingleton().getActiveCharacter();
            CeGuiString activeCharId = "";

            if (activeChar) {
                CreatureControllerManager::getSingleton().detachController(
                    activeChar);
                activeChar->setState(GOS_LOADED);
                activeCharId = activeChar->getId();
                //PartyManager::getSingleton().removeCharacter(activeChar); //erzeugt nur ein game over --> party manager und character window müssen komplett überarbeitet werden
                PartyManager::getSingleton().setActiveCharacter(NULL);
            }
    
            if (saveCurrent && mCurrentScene) 
            {
                //hier daten abspeichern
                SaveAbleManager::getSingleton().saveState();
                //mSceneStates[mCurrentScene] = mCurrentScene->getAllProperties();
            }
                  
            std::map<Scene*, PropertyRecordPtr>::iterator itState =
                mSceneStates.end();
            if (saveCurrent) 
            {
                //gespeicherte daten für die scene abrufen
                std::map<Scene*, PropertyRecordPtr>::iterator itState 
                    = mSceneStates.find(mCurrentScene);
            }

            itScene->second->load(itState != mSceneStates.end()); // don't load game objects if there is a saved state
            mCurrentScene = itScene->second;
            
            if (itState != mSceneStates.end())
            {
                mCurrentScene->setProperties(itState->second);
            }

            if (activeCharId != "") 
            {
                if(static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(activeCharId)))
                    PartyManager::getSingleton().setActiveCharacter(static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(activeCharId)));
            }
        }
        else
        {
            LOG_ERROR("SceneManager", 
                "Scene '" + sceneName + "' not found. Have you forgotten to define it in a .rlscene file?");
        }
    }

    const Ogre::StringVector& SceneManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    void SceneManager::parseScript(Ogre::DataStreamPtr& data, const Ogre::String& resourceGroup)
    {
        SceneLoader* loader = new SceneLoader();
        addScene(loader->loadScene(data));
        delete loader;
    }

    Ogre::Real SceneManager::getLoadingOrder() const
    {
        return 1000;
    }

}
