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
#include "GameLoop.h"
#include "RubyInterpreter.h"
#include "Exception.h"
#include "ConfigurationManager.h"
#include "Logger.h"
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
        mInterpreter(NULL),
        mActiveAdventureModule(""),
		mDefaultActiveModule("")
    {
        resetClock();
        initializeCoreSubsystem();        
        resetClock();
    }

    CoreSubsystem::~CoreSubsystem() 
    {  
        if(mWorld != 0)
            delete mWorld;
        
        delete DeletionPropagator::getSingletonPtr();
        delete ScriptObjectRepository::getSingletonPtr();
    }

    void CoreSubsystem::startCore()
    {
        getInterpreter()->execute("load 'startup-global.rb'");
		if (mDefaultActiveModule == "")
			getInterpreter()->execute("load 'startup-global-mainmenu.rb'");
		else
			startAdventureModule(mDefaultActiveModule);

        Root::getSingleton().startRendering();
    }

	void CoreSubsystem::log(const Ogre::LogMessageLevel level, const String& msg, const String& ident)
    {
		Logger::getSingleton().log(level, "Core", msg, ident);
    }

	void CoreSubsystem::log(const String& msg)
    {
		log(Ogre::LML_NORMAL, msg);
    }

    bool  CoreSubsystem::setupConfiguration()
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        // Ein kleiner C-Trick: Wir versuchen erstmal die Konfiguration
        // wiederherzustellen. Klappt das nicht (Rueckgabe: FALSE), dann
        // zeigen wir den Konf.dialog.
        if(Root::getSingleton().restoreConfig()
            || Root::getSingleton().showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering 
            // window by passing 'true'
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
        mInterpreter=new RubyInterpreter();

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
        

        return true;
    }

    void CoreSubsystem::initializeResources()
    {
        new XmlResourceManager();

		// Fuer Configs die keinem Typ zugeordnet sind, und die per kompletten Verezeichnis erfragt werden
        addCommonSearchPath(ConfigurationManager::getSingleton().
        	getModulesRootDirectory());

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

            // ehemals Texturen vorraus laden / 
			initializeModuleTextures(value);

			// Ein common-modul - wird sofort hinzugeladen
            if (key.compare("common") == 0)
            {
                initializeModule(value);
                mCommonModules.push_back(value);
            }
			// Ein normales-modul, zur späteren Auswahl
            else if (key.compare("module") == 0)
                mActivatableModules.push_back(value);
        }

		// Partikelsystem initialisieren
        ParticleSystemManager::getSingleton().addRendererFactory( 
            new BillboardParticleRendererFactory() );

        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();        
    }

    void CoreSubsystem::initializeModuleTextures(const std::string& module)
    {
        std::string moduleDir = ConfigurationManager::getSingleton().
        	getModulesRootDirectory() + "/modules/" + module;
        ConfigFile cf;
        cf.load(ConfigurationManager::getSingleton().getModuleconfigCfgPath(
        	module));
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        std::string key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

            if (key.compare("TextureArchive") == 0)
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/materials/" + value, "Zip");
            else if (key.compare("TextureDir") == 0)
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/materials/" + value, "FileSystem");
            else if (key.compare("Archive") == 0)
                ResourceGroupManager::getSingleton().addResourceLocation(
                	moduleDir + "/" + value, "Zip");
        }
        addCommonSearchPath(moduleDir+"/materials");
    }

    void CoreSubsystem::initializeModule(const std::string& module)
    {
        std::string moduleDir = ConfigurationManager::getSingleton().
        	getModulesRootDirectory() + "/modules/" + module;

        addCommonSearchPath(moduleDir + "/conf");
        addCommonSearchPath(moduleDir + "/dsa");
        addCommonSearchPath(moduleDir + "/maps");
        addCommonSearchPath(moduleDir + "/models");
        addCommonSearchPath(moduleDir + "/sound"); //@todo ueber Verzeichnisnamen nachdenken
		addCommonSearchPath(moduleDir + "/sound/holz");
		addCommonSearchPath(moduleDir + "/sound/metall");
		addCommonSearchPath(moduleDir + "/sound/natur");
		addCommonSearchPath(moduleDir + "/sound/ost");
		addCommonSearchPath(moduleDir + "/sound/sonst");
		addCommonSearchPath(moduleDir + "/sound/waffen");
		addCommonSearchPath(moduleDir + "/sound/wesen");
		addCommonSearchPath(moduleDir + "/sound/zauber");
		addCommonSearchPath(moduleDir + "/sound/mensch");
        addCommonSearchPath(moduleDir + "/gui");
        addCommonSearchPath(moduleDir + "/gui/fonts");
        addCommonSearchPath(moduleDir + "/gui/imagesets");
        addCommonSearchPath(moduleDir + "/gui/schemes");
        addCommonSearchPath(moduleDir + "/gui/windows");
        addCommonSearchPath(moduleDir + "/gui/windows/buttons");
        addCommonSearchPath(moduleDir + "/dialogs");     

        if (getInterpreter() != NULL)
        {
            getInterpreter()->addSearchPath(moduleDir + "/scripts");
            getInterpreter()->addSearchPath(moduleDir + "/scripts/maps");
        }
    }

    void CoreSubsystem::addCommonSearchPath(const std::string& path)
    {
        try 
        {
            ResourceGroupManager::getSingleton().addResourceLocation(path, 
            	"FileSystem");
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

        initializeModule(module);
        mActiveAdventureModule = module;

        getInterpreter()->execute("load 'startup-module.rb'");
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

    void CoreSubsystem::setInterpreter(Interpreter* interpreter)
    {
        mInterpreter = interpreter;
    }

    Interpreter* CoreSubsystem::getInterpreter()
    {
        return mInterpreter;
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
    	const String startupScript)
    {
        GameLoopManager::getSingleton().setPaused(true);

        mWorld->loadScene(filename);
        if (startupScript.length() > 0)
            getInterpreter()->execute(String("load '") + startupScript + 
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
}
