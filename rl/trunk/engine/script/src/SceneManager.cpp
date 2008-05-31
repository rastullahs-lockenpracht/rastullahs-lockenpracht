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

#include "Creature.h"
#include "CreatureControllerManager.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "UiSubsystem.h"

rl::SceneManager* Ogre::Singleton<rl::SceneManager>::ms_Singleton = NULL;

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

        Creature* activeChar = UiSubsystem::getSingleton().getActiveCharacter();

        if (activeChar) {
            CreatureControllerManager::getSingleton().detachController(
                activeChar);
            activeChar->setState(GOS_LOADED);
        }

        if (saveCurrent) 
        {
            mSceneStates[mCurrentScene] = mCurrentScene->getAllProperties();
        }

        std::map<CeGuiString, Scene*>::iterator itScene = mScenes.find(sceneName);
        if (itScene != mScenes.end())
        {            
            if (saveCurrent) 
            {
                itScene->second->load(false);
                mCurrentScene = itScene->second;
                std::map<Scene*, PropertyRecordPtr>::iterator itState 
                    = mSceneStates.find(mCurrentScene);
                if (itState != mSceneStates.end())
                {
                    mCurrentScene->setProperties(itState->second);
                }
            }
            else 
            {
                itScene->second->load(true);
                mCurrentScene = itScene->second;
            }
        }
        else
        {
            LOG_ERROR("SceneManager", 
                "Scene '" + sceneName + "' not found. Have you forgotten to define it in a .rlscene file?");
        }

        if (activeChar) 
        {
            UiSubsystem::getSingleton().setActiveCharacter(activeChar);
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
