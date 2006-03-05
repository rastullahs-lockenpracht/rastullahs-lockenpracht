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

#include "XmlResourceManager.h"

#include "CoreSubsystem.h"

#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreMeshManager.h>
#include <OgreBillboardParticleRenderer.h>

#include "ContentModule.h"
#include "DotSceneOctreeWorld.h"
#include "PhysicsManager.h"
#include "ActorManager.h"
#include "DeletionPropagator.h"
#include "ScriptObjectRepository.h"
#include "AnimationManager.h"
#include "GameEventManager.h"
#include "SoundManager.h"
#include "GameLoop.h"
#include "RubyInterpreter.h"
#include "Exception.h"
#include "ConfigurationManager.h"
#include "Logger.h"
#include "CoreEvents.h"
#include "SoundUpdateTask.h"
#include <ctime>


template<> rl::CoreSubsystem* Singleton<rl::CoreSubsystem>::ms_Singleton = 0;

namespace rl {

    CoreSubsystem& CoreSubsystem::getSingleton()
    {
        return Singleton<CoreSubsystem>::getSingleton();
    }

    CoreSubsystem* CoreSubsystem::getSingletonPtr()
    {
        return Singleton<CoreSubsystem>::getSingletonPtr();
    }

    CoreSubsystem::CoreSubsystem()
        : 	mWorld(NULL),
        mRubyInterpreter(NULL),
        mModules(),
        mActiveAdventureModule(NULL),
        mDefaultActiveModule(""),
        mClockStartTime(),
        mDeveloperMode(false),
        mCoreEventCaster(),
        mSoundListenerActor(NULL)
    {
        resetClock();
        initializeCoreSubsystem();        
        resetClock();
    }

    CoreSubsystem::~CoreSubsystem() 
    {  
		mCoreEventCaster.removeEventListeners();

        if(mWorld != 0)
            delete mWorld;
        
        delete DeletionPropagator::getSingletonPtr();
        delete ScriptObjectRepository::getSingletonPtr();

//		ActorManager::getSingleton().destroyActor(mSoundListenerActor);
    }

    void CoreSubsystem::startCore()
    {
		mRubyInterpreter->executeFile("globals.rb");
		mRubyInterpreter->executeFile("startup-global.rb");

		if (mDefaultActiveModule == "")
		{
			mRubyInterpreter->executeFile("startup-global-mainmenu.rb");
		}
		else
		{
			ContentModule* mod = getModule(mDefaultActiveModule);
			if (mod == NULL)
				Throw(rl::RuntimeException, "Module "+mDefaultActiveModule+" not found");
			startAdventureModule(mod);
		}

        Root::getSingleton().startRendering();
    }

    bool CoreSubsystem::setupConfiguration()
    {      
        if(Root::getSingleton().restoreConfig()
            || Root::getSingleton().showConfigDialog())
        {
            Root::getSingleton().initialise(true);
            return true;
        }
        else
        {
            return false;
        }
    }

	void CoreSubsystem::setDeveloperMode(bool developerMode)
	{
		mDeveloperMode = developerMode;
	}

	bool CoreSubsystem::getDeveloperMode() const
	{
		return mDeveloperMode;
	}

    bool CoreSubsystem::initializeCoreSubsystem()
    {
        new Root(
        	ConfigurationManager::getSingleton().getPluginCfgPath(), 
        	ConfigurationManager::getSingleton().getRastullahCfgPath(), 
        	ConfigurationManager::getSingleton().getOgreLogPath()
        );

        Root::getSingleton().setFrameSmoothingPeriod(0.5f);

        // Muss vor dem Laden der Ressourcen geschehen,
        // weil es sonst sofort angewandt wird.
        MeshManager::getSingleton().setBoundsPaddingFactor(0.0);

        
        bool carryOn = setupConfiguration();

        if (!carryOn) 
            return false;

		new DeletionPropagator();
		mRubyInterpreter = new RubyInterpreter();
		mRubyInterpreter->initializeInterpreter();
		
		initializeResources();

        // Set default mipmap level (NB some APIs ignore this)
        //TODO: In Config-Datei verlagern
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR); 
        MaterialManager::getSingleton().setDefaultAnisotropy(1);
        
        
        mWorld = new DotSceneOctreeWorld();
		new PhysicsManager();
        
        new GameLoopManager(100); //TODO: In Config-Datei verlagern
        GameLoopManager::getSingleton().addSynchronizedTask(
            PhysicsManager::getSingletonPtr(), FRAME_STARTED);
        new AnimationManager();
        GameLoopManager::getSingleton().addSynchronizedTask(
            AnimationManager::getSingletonPtr(), FRAME_STARTED);
        new ActorManager();
        new GameEventManager();
        GameLoopManager::getSingleton().addSynchronizedTask(
            GameEventManager::getSingletonPtr(), FRAME_STARTED);
        GameLoopManager::getSingleton().addAsynchronousTask(
            SoundUpdateTask::getSingletonPtr()); 

		return true;
    }

    void CoreSubsystem::initializeResources()
    {
        new XmlResourceManager();

		// Fuer Configs die keinem Typ zugeordnet sind, und die per kompletten Verezeichnis erfragt werden
		ResourceGroupManager::getSingleton().addResourceLocation(
			ConfigurationManager::getSingleton().
				getModulesRootDirectory(), 
            "FileSystem", 
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        // Laden mittels eines Configfiles
        ConfigFile cf;
        cf.load(ConfigurationManager::getSingleton().getModulesCfgPath());

        // Durchgehen der einzelnen Settings
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();
        String key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

			if (key.compare("module") == 0)
            {
				mRubyInterpreter->executeFile(ContentModule::getInitFile(value));
				
				ContentModule* module = getModule(value);

				if (module == NULL)
				{
					Throw(
						rl::RuntimeException,
						ContentModule::getInitFile(value) + " did not register module '"+value+"'");
				}
				else
				{
					if (module->isCommon())
					{
						module->initializeTextures();
						module->initialize();
					}
				}
            }
        }

		// Partikelsystem initialisieren
        ParticleSystemManager::getSingleton().addRendererFactory( 
            new BillboardParticleRendererFactory() );

        ResourceGroupManager::getSingleton()
            .initialiseResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        precreateTextures();
    }

    void CoreSubsystem::precreateTextures()
    {
        // Falls Texturen noch nicht über Materialien referenziert erzeugt wurden,
        // hole das jetzt nach.
        char* suffixes[] = {"png", "jpg", "jpeg", "tga", "dds"};
        int suffix_count = 5;
        vector<String> texes;
        for (int i = 0; i < suffix_count; ++i)
        {
            StringVectorPtr curTexes = ResourceGroupManager::getSingleton()
                .findResourceNames(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                String("*.") + suffixes[i]);
            texes.insert(texes.end(), curTexes->begin(), curTexes->end());
        }
        for (size_t i = 0; i < texes.size(); ++i)
        {
            ResourcePtr res = TextureManager::getSingleton().getByName(texes[i]);
            if (res.isNull())
            {
                TexturePtr tex = TextureManager::getSingleton().create(texes[i],
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
                tex->setNumMipmaps(TextureManager::getSingleton().getDefaultNumMipmaps());
                tex->enable32Bit(true);
            }
        }
    }

	ContentModule* CoreSubsystem::getModule(const String& moduleId) const
	{
		ModuleMap::const_iterator moduleIt = mModules.find(moduleId);

		if (moduleIt != mModules.end())
		{
			return (*moduleIt).second;
		}
		else
		{
			return NULL;
		}
	}

	void CoreSubsystem::registerModule(ContentModule* module)
	{
		mModules[module->getId()] = module;
	}

    void CoreSubsystem::startAdventureModule(ContentModule* module)
    {
		if (mActiveAdventureModule != NULL)
		{
	        mActiveAdventureModule->unload();
		}

        module->initializeTextures();
        module->initialize();

		mCoreEventCaster.dispatchEvent(new DataLoadedEvent(0.0));
		
		ResourceGroupManager::getSingleton().initialiseResourceGroup(module->getId());
        module->precreateMeshes();
        mActiveAdventureModule = module;

		mCoreEventCaster.dispatchEvent(new DataLoadedEvent(100.0));

        module->start();
    }

	void CoreSubsystem::setDefaultActiveModule(const Ogre::String& module)
	{
		mDefaultActiveModule = module;
	}

	const Ogre::String& CoreSubsystem::getDefaultActiveModule() const
	{
		return mDefaultActiveModule;
	}

    World* CoreSubsystem::getWorld()
    { 
        return mWorld;
    }


    RubyInterpreter* CoreSubsystem::getRubyInterpreter()
    {
        return mRubyInterpreter;
    }

    void CoreSubsystem::makeScreenshot( const String& sName )
    {
        Ogre::Root::getSingleton().getAutoCreatedWindow()->
            writeContentsToTimestampedFile(sName, ".jpg");
    }

    ContentModule* CoreSubsystem::getActiveAdventureModule() const
    {
        return mActiveAdventureModule;
    }

	const ModuleMap& CoreSubsystem::getAllModules() const
	{
		return mModules;
	}

    void CoreSubsystem::loadMap(const String type, const String filename, 
    	const String module, const String startupScript)
    {
        GameLoopManager::getSingleton().setPaused(true);

        mWorld->loadScene(filename, module);
        if (startupScript.length() > 0)
            mRubyInterpreter->executeFile(startupScript);

        GameLoopManager::getSingleton().setPaused(false);
    }

    void CoreSubsystem::resetClock()
    {
        mClockStartTime = getCurrentTime();
    }

    RL_LONGLONG CoreSubsystem::getClock()
    {
        return getCurrentTime() - mClockStartTime;
    }

    RL_LONGLONG CoreSubsystem::getCurrentTime()
    {
        timeval timebuffer;
        gettimeofday(&timebuffer, NULL);

        return static_cast<RL_LONGLONG>(timebuffer.tv_sec) * 1000L + 
        	static_cast<RL_LONGLONG>(timebuffer.tv_usec / 1000);
    }

	void CoreSubsystem::addCoreEventListener(rl::CoreEventListener *listener)
	{
		mCoreEventCaster.addEventListener(listener);
	}

	Actor* CoreSubsystem::getSoundListener()
	{
		if (mSoundListenerActor == NULL)
			mSoundListenerActor = ActorManager::getSingleton().createListenerActor("SoundListenerObject");
		return mSoundListenerActor;
	}
}
