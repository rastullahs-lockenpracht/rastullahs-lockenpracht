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
        mActiveAdventureModule(""),
		mDefaultActiveModule("")
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
    }

    void CoreSubsystem::startCore()
    {
		mRubyInterpreter->execute("load 'globals.rb'");
        mRubyInterpreter->execute("load 'startup-global.rb'");
		if (mDefaultActiveModule == "")
			mRubyInterpreter->execute("load 'startup-global-mainmenu.rb'");
		else
			startAdventureModule(mDefaultActiveModule);

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

		initializeResources();

        // Set default mipmap level (NB some APIs ignore this)
        //TODO: In Config-Datei verlagern
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR); 
        MaterialManager::getSingleton().setDefaultAnisotropy(1);
        
        new DeletionPropagator();
        mWorld = new DotSceneOctreeWorld();
		new PhysicsManager();
        mRubyInterpreter=new RubyInterpreter();

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
        addSearchPath(ConfigurationManager::getSingleton().
            getModulesRootDirectory(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

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

			// Ein common-modul - wird sofort hinzugeladen
            if (key.compare("common") == 0)
            {
                initializeModuleTextures(value, true);
                initializeModule(value, true);
                mCommonModules.push_back(value);
            }
			// Ein normales-modul, zur späteren Auswahl
            else if (key.compare("module") == 0)
            {
                mActivatableModules.push_back(value);
            }
        }

		// Partikelsystem initialisieren
        ParticleSystemManager::getSingleton().addRendererFactory( 
            new BillboardParticleRendererFactory() );

        ResourceGroupManager::getSingleton()
            .initialiseResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        precreateTextures();
    }

    void CoreSubsystem::precreateMeshes(const std::string& module)
    {
        StringVectorPtr meshes = ResourceGroupManager::getSingleton()
            .findResourceNames(module, "*.mesh");

        for (size_t i = 0; i < meshes->size(); ++i)
        {
            ResourcePtr res = MeshManager::getSingleton().getByName((*meshes)[i]);
            if (res.isNull())
            {
                MeshPtr mesh = MeshManager::getSingleton().create((*meshes)[i],
                    module);
            }
        }
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

    void CoreSubsystem::initializeModuleTextures(const std::string& module, bool isCommon)
    {
        std::string moduleDir = ConfigurationManager::getSingleton().
        	getModulesRootDirectory() + "/modules/" + module;
        ConfigFile cf;
        cf.load(ConfigurationManager::getSingleton().getModuleconfigCfgPath(
        	module));
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        string resourceGroup = isCommon ?
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : module;

        std::string key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

            if (key.compare("TextureArchive") == 0)
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/materials/" + value, "Zip", resourceGroup);
            }
            else if (key.compare("TextureDir") == 0)
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/materials/" + value, "FileSystem", resourceGroup);
            }
            else if (key.compare("Archive") == 0)
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/" + value, "Zip", resourceGroup);
            }
        }
        addSearchPath(moduleDir+"/materials", resourceGroup);
    }

    void CoreSubsystem::initializeModule(const std::string& module, bool isCommon)
    {
        std::string moduleDir = ConfigurationManager::getSingleton().
        	getModulesRootDirectory() + "/modules/" + module;

        string resourceGroup = isCommon ?
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : module;

        addSearchPath(moduleDir + "/conf", resourceGroup);
        addSearchPath(moduleDir + "/dsa", resourceGroup);
        addSearchPath(moduleDir + "/maps", resourceGroup);
        addSearchPath(moduleDir + "/models", resourceGroup);
        addSearchPath(moduleDir + "/sound", resourceGroup); //@todo ueber Verzeichnisnamen nachdenken
		addSearchPath(moduleDir + "/sound/holz", resourceGroup);
		addSearchPath(moduleDir + "/sound/metall", resourceGroup);
		addSearchPath(moduleDir + "/sound/natur", resourceGroup);
		addSearchPath(moduleDir + "/sound/ost", resourceGroup);
		addSearchPath(moduleDir + "/sound/sonst", resourceGroup);
		addSearchPath(moduleDir + "/sound/waffen", resourceGroup);
		addSearchPath(moduleDir + "/sound/wesen", resourceGroup);
		addSearchPath(moduleDir + "/sound/zauber", resourceGroup);
		addSearchPath(moduleDir + "/sound/mensch", resourceGroup);
        addSearchPath(moduleDir + "/gui", resourceGroup);
        addSearchPath(moduleDir + "/gui/fonts", resourceGroup);
        addSearchPath(moduleDir + "/gui/imagesets", resourceGroup);
        addSearchPath(moduleDir + "/gui/schemes", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows/buttons", resourceGroup);
        addSearchPath(moduleDir + "/dialogs", resourceGroup);     

		SoundManager::getSingleton().addSounds( resourceGroup );

        if (mRubyInterpreter != NULL)
        {
            mRubyInterpreter->addSearchPath(moduleDir + "/scripts");
            mRubyInterpreter->addSearchPath(moduleDir + "/scripts/maps");
        }
    }

    void CoreSubsystem::addSearchPath(const std::string& path,
        const std::string& resourceGroup)
    {
        try 
        {
            ResourceGroupManager::getSingleton().addResourceLocation(path, 
                "FileSystem", resourceGroup);
        } 
        catch(...) 
        {} // and forget
    }

    void CoreSubsystem::unloadModule(const std::string& module)
    {
        //TODO: unloadModule
    }

    void CoreSubsystem::startAdventureModule(const std::string& module)
    {
        StringVector::iterator mod;
        for (mod = mActivatableModules.begin(); 
            mod != mActivatableModules.end(); mod++)
            if ((*mod).compare(module) == 0)
                break;

        if (mod == mActivatableModules.end())
            Throw(InvalidArgumentException, "Unknown Module '"+module+"'");

        if (mActiveAdventureModule.length() > 0)
            unloadModule(mActiveAdventureModule);

        initializeModuleTextures(module, false);
        initializeModule(module, false);
		mCoreEventCaster.dispatchEvent(new DataLoadedEvent(0.0));
        ResourceGroupManager::getSingleton().initialiseResourceGroup(module);

        precreateMeshes(module);

        mActiveAdventureModule = module;
		mCoreEventCaster.dispatchEvent(new DataLoadedEvent(100.0));

        mRubyInterpreter->execute("load 'startup-module.rb'");
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

    const String& CoreSubsystem::getActiveAdventureModule() const
    {
        return mActiveAdventureModule;
    }

    const StringVector& CoreSubsystem::getCommonModules() const
    {
        return mCommonModules;
    }

    const StringVector& CoreSubsystem::getActivatableModules() const
    {
        return mActivatableModules;
    }

    void CoreSubsystem::loadMap(const String type, const String filename, 
    	const String module, const String startupScript)
    {
        GameLoopManager::getSingleton().setPaused(true);

        mWorld->loadScene(filename, module);
        if (startupScript.length() > 0)
            mRubyInterpreter->execute(String("load '") + startupScript + 
            	String("'"));

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
}
