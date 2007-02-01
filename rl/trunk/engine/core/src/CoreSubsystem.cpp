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

#include "XmlResourceManager.h"

#include "CoreSubsystem.h"

#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreMeshManager.h>
#include <OgreBillboardParticleRenderer.h>

#include "ActorManager.h"
#include "AnimationManager.h"
#include "ContentModule.h"
#include "ConfigurationManager.h"
#include "CoreEvents.h"
#include "DotSceneOctreeWorld.h"
#include "Exception.h"
#include "GameEventManager.h"
#include "GameLoop.h"
#include "Logger.h"
#include "PhysicsManager.h"
#include "RubyInterpreter.h"
#include "ScriptWrapper.h"
#include "SoundManager.h"
#include "DebugVisualsManager.h"
#include "JobScheduler.h"

#include <ctime>

using namespace Ogre;

template<> rl::CoreSubsystem* Singleton<rl::CoreSubsystem>::ms_Singleton = 0;

namespace rl {

    CoreSubsystem::CoreSubsystem()
        : Singleton<CoreSubsystem>(),
        mWorld(NULL),
        mRubyInterpreter(NULL),
        mModules(),
        mActiveAdventureModule(NULL),
        mDefaultActiveModule(""),
        mClockStartTime(),
        mDeveloperMode(false),
		mInitialized(false),
        mCoreEventCaster(),
        mDefaultTechniques(),
        mOgreRoot(NULL),
        mScriptWrapper(NULL),
        mXmlResourceManager(NULL),
        mPhysicsManager(NULL),
        mGameLoop(NULL),
        mAnimationManager(NULL),
        mActorManager(NULL),
        mGameEventManager(NULL),
        mConfigurationManager(NULL),
		mSoundManager(NULL),
        mDebugVisualsManager(NULL),
        mJobScheduler(NULL)
    {
        initializeCoreSubsystem();
    }

    CoreSubsystem::~CoreSubsystem()
    {
		mCoreEventCaster.removeEventListeners();

        delete mWorld;
        delete mGameLoop;
        delete mJobScheduler;
        delete mActorManager;
        delete mGameEventManager;
        delete mAnimationManager;
        delete mDebugVisualsManager;
        delete mPhysicsManager;
        delete mXmlResourceManager;
        delete mScriptWrapper;
        delete mConfigurationManager;
		delete mSoundManager;
        delete mOgreRoot;
        delete mRubyInterpreter;
	}

    void CoreSubsystem::startCore()
    {
        loadPlugins();

		mInitialized = true;

		if (mDefaultActiveModule == "")
		{
			mDefaultActiveModule = "intro"; ///@fixme replace by last active
		}


		ContentModule* mod = getModule(mDefaultActiveModule);
		if (mod == NULL)
		{
			Throw(rl::RuntimeException, "Module "+mDefaultActiveModule+" not found");
		}
		else
		{
			startAdventureModule(mod);
		}

		mGameLoop->loop();

        mWorld->clearScene();
        unloadPlugins();

        //mRubyInterpreter->finalizeInterpreter();
    }

    bool CoreSubsystem::setupConfiguration()
    {
        if(Root::getSingleton().restoreConfig()
            || Root::getSingleton().showConfigDialog())
        {
            std::stringstream name;
            name << "Rastullahs Lockenpracht - ";
            name << ConfigurationManager::getSingleton().getEngineVersionString() << " ";
            name << ConfigurationManager::getSingleton().getEngineVersionName() << " [";
            name << ConfigurationManager::getSingleton().getEngineBuildNumber() << "]";

            Root::getSingleton().initialise(true, name.str() );
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

	bool CoreSubsystem::isInitialized() const
	{
		return mInitialized;
	}

    bool CoreSubsystem::initializeCoreSubsystem()
    {
        mConfigurationManager = ConfigurationManager::getSingletonPtr();
        mOgreRoot = new Root(
        	mConfigurationManager->getPluginCfgPath(),
        	mConfigurationManager->getRastullahCfgPath(),
        	mConfigurationManager->getOgreLogFile()
        );

        //Root::getSingleton().setFrameSmoothingPeriod(0.5f);

        // Muss vor dem Laden der Ressourcen geschehen,
        // weil es sonst sofort angewandt wird.
        MeshManager::getSingleton().setBoundsPaddingFactor(0.0);

		// EDIT:
		LOG_MESSAGE(Logger::CORE,"init");

        bool carryOn = setupConfiguration();

        if (!carryOn)
            return false;

		mGameLoop = new GameLoop();
        LOG_MESSAGE(Logger::CORE,"GameLoopmanager erzeugt");

		mScriptWrapper = new ScriptWrapper();
		LOG_MESSAGE(Logger::CORE,"Skriptwrapper erzeugt");
        // TODO: muss löschbar werden.
		mRubyInterpreter = new RubyInterpreter();
		LOG_MESSAGE(Logger::CORE,"RubyInterpreter erzeugt");
		mRubyInterpreter->initializeInterpreter();
		LOG_MESSAGE(Logger::CORE,"RubyInterpreter initialisiert");

        mActorManager = new ActorManager();
		LOG_MESSAGE(Logger::CORE,"Aktormanager erzeugt");

		mSoundManager = new SoundManager();
		LOG_MESSAGE(Logger::CORE,"SoundManager erzeugt");

		initializeResources();

        // Set default mipmap level (NB some APIs ignore this)
        //TODO: In Config-Datei verlagern
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR);
        MaterialManager::getSingleton().setDefaultAnisotropy(
            ConfigurationManager::getSingleton().getIntSetting("max_anisotropy",1));


        mWorld = new DotSceneOctreeWorld();
		mActorManager->setWorld(mWorld);

		mPhysicsManager = new PhysicsManager();
        GameLoop::getSingleton().addTask(mPhysicsManager, GameLoop::TG_PHYSICS);

        mAnimationManager = new AnimationManager();
        GameLoop::getSingleton().addTask(mAnimationManager, GameLoop::TG_GRAPHICS);

        mGameEventManager = new GameEventManager();
        GameLoop::getSingleton().addTask(mGameEventManager, GameLoop::TG_LOGIC);

        mDebugVisualsManager = new DebugVisualsManager();
        GameLoop::getSingleton().addTask(mDebugVisualsManager, GameLoop::TG_GRAPHICS);

        mJobScheduler = new JobScheduler();
        GameLoop::getSingleton().addTask(mJobScheduler, GameLoop::TG_GRAPHICS);

		return true;
    }

    void CoreSubsystem::initializeResources()
    {
        mXmlResourceManager = new XmlResourceManager();

		// Fuer Configs die keinem Typ zugeordnet sind,
        // und die per kompletten Verezeichnis erfragt werden
		ResourceGroupManager::getSingleton().addResourceLocation(
			ConfigurationManager::getSingleton().
				getModulesRootDirectory(),
            "FileSystem",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        ResourceGroupManager::getSingleton().addResourceLocation(
			ConfigurationManager::getSingleton().
                getConfigModulePath(),
            "FileSystem",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        mConfigurationManager->loadConfig();

        // Laden mittels eines Configfiles
        ConfigFile cf;
        cf.load(mConfigurationManager->getModulesCfgPath());

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
				//else
				//{
				//	if (module->isCommon())
				//	{
				//		module->initializeTextures();
				//		module->initialize();
				//	}
				//}
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

	void CoreSubsystem::updateDefaultScheme()
	{
		String tuScheme = mConfigurationManager->getTextureUnitScheme();

		for (ResourceManager::ResourceMapIterator itMat =
			MaterialManager::getSingleton().getResourceIterator();
			itMat.hasMoreElements();)
		{
			MaterialPtr mat = itMat.getNext();
			for (Material::TechniqueIterator itTech =
				mat->getTechniqueIterator();
				itTech.hasMoreElements();)
			{
				Technique* tech = itTech.getNext();
				if (tech->getSchemeName() == tuScheme)
				{
					tech->setSchemeName(MaterialManager::DEFAULT_SCHEME_NAME);
					mDefaultTechniques.push_back(tech);
				}
				else if (tech->getSchemeName() == MaterialManager::DEFAULT_SCHEME_NAME)
				{
					mDefaultTechniques.push_back(tech);
				}
			}
		}
	}

	void CoreSubsystem::setScheme(const Ogre::String& schemeName)
	{
		if (schemeName != MaterialManager::getSingleton().getActiveScheme())
		{
			/*for (std::vector<Technique*>::iterator iter = mDefaultTechniques.begin();
				iter != mDefaultTechniques.end(); ++iter)
			{
				Technique* cur = *iter;
				cur->setSchemeName(schemeName);
			}*/

			MaterialManager::getSingleton().setActiveScheme(schemeName);
		}
	}

    void CoreSubsystem::startAdventureModule(ContentModule* module)
    {
        if (mActiveAdventureModule != NULL)
		{
	        mActiveAdventureModule->unload();
		}

        //mCoreEventCaster.dispatchEvent(new DataLoadedEvent(0.0));

		updateDefaultScheme();
        loadModule(module);
        mActiveAdventureModule = module;

        //mCoreEventCaster.dispatchEvent(new DataLoadedEvent(100.0));

        module->start();
    }

    void CoreSubsystem::loadModule(ContentModule* module)
    {
        if (module->getMinimumEngineVersion()
            > ConfigurationManager::getSingleton().getEngineBuildNumber())
		{
			Throw(
				rl::RuntimeException,
				"Module "
				+ Ogre::String(module->getName().c_str())
				+ " needs engine >="
				+ StringConverter::toString(module->getMinimumEngineVersion())
				+ " but engine is "
				+ StringConverter::toString(ConfigurationManager::getSingleton().getEngineBuildNumber()));
		}

        StringVector deps = module->getDependencies();
        for (StringVector::const_iterator depsIt = deps.begin(); depsIt != deps.end(); depsIt++)
        {
            String depName = *depsIt;
            ModuleMap::iterator modIt = mModules.find(depName);
            if (modIt == mModules.end())
            {
			    Throw(
				    rl::RuntimeException,
				    "Depedency Module " + depName
                    + " needed by " + Ogre::String(module->getName().c_str())
                    + " not found.");
            }

            ContentModule* depMod = (*modIt).second;

            if (!depMod->isLoaded())
            {
                loadModule(depMod);
            }
        }

        LOG_MESSAGE(
            Logger::CORE,
            "Start initializing module " + module->getName());
        module->initializeTextures();
        module->initialize();

		ResourceGroupManager::getSingleton().initialiseResourceGroup(module->getId());
        module->precreateMeshes();

        if (module->isCommon())
        {
            module->start();
        }

        LOG_MESSAGE(
            Logger::CORE,
            "Module " + module->getName() + " initialized.");
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
        mWorld->loadScene(filename, module);
        if (startupScript.length() > 0) mRubyInterpreter->executeFile(startupScript);
    }

    void CoreSubsystem::loadPlugins()
    {
		mSoundManager->loadConf(rl::ConfigurationManager::getSingleton().getSoundCfgPath());
		LOG_MESSAGE(Logger::CORE, "Soundkonfiguration geladen");
    }

    void CoreSubsystem::unloadPlugins()
    {
        mSoundManager->saveConf(rl::ConfigurationManager::getSingleton().getSoundCfgPath());
		LOG_MESSAGE(Logger::CORE, "Soundkonfiguration gespeichert.");
        mSoundManager->unloadAllDrivers();
    }

	void CoreSubsystem::addCoreEventListener(rl::CoreEventListener *listener)
	{
		mCoreEventCaster.addEventListener(listener);
	}

	void CoreSubsystem::removeCoreEventListener(CoreEventListener* listener)
    {
		mCoreEventCaster.removeEventListener(listener);
    }
}
