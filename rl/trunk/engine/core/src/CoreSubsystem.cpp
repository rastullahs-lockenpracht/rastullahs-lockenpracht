#include "XmlResourceManager.h"

#include "CoreSubsystem.h"

#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include <OgreLog.h>
#include <OgreConfigFile.h>

#if OGRE_PLATFORM == PLATFORM_APPLE 
#    include <SDL.H>
#elif OGRE_PLATFORM == PLATFORM_LINUX
#    include <SDL/SDL.h>
#endif

#include "BSPWorld.h"
#include "NatureWorld.h"
#include "TerrainWorld.h"
#include "PhysicsManager.h"
#include "ActorManager.h"
#include "GameLoop.h"
#include "RubyInterpreter.h"


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
	{
		mWorld = 0;
		mActiveModule = "";
        initializeCoreSubsystem();
	}

    CoreSubsystem::~CoreSubsystem() 
    {  
		if(mWorld != 0)
			delete mWorld;

		mWorld = 0;
        Root::getSingleton().shutdown();
    }

	void CoreSubsystem::startCore()
    {		
		Root::getSingleton().startRendering();
    }

    void CoreSubsystem::log(const String& msg)
    {
        if (LogManager::getSingletonPtr() == 0)
        {
            new LogManager();
        }
        LogManager::getSingleton().getLog( "logs/rlCore.log" )->logMessage(msg);
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
            // Here we choose to let the system create a default rendering window by passing 'true'
			Root::getSingleton().initialise(true);
            return true;
        }
        else
        {
            return false;
        }
    }

	bool CoreSubsystem::initializeCoreSubsystem()
    {
		#if OGRE_PLATFORM == PLATFORM_WIN32
			new Root( "plugins-win.cfg", "rastullah.cfg", "logs/ogre.log" );
		#elif OGRE_PLATFORM == PLATFORM_LINUX
			new Root( "plugins-linux.cfg", "rastullah.cfg", "logs/ogre.log" );
		#else
			new Root( "plugins-mac.cfg", "rastullah.cfg", "logs/ogre.log" );
		#endif

        initializeResources();

        bool carryOn = setupConfiguration();
        
		if (!carryOn) 
			return false;

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipMaps(5);
        Log* log = LogManager::getSingleton().createLog( "logs/rlCore.log" );
        log->setLogDetail( LL_BOREME );
		
		new GameLoop();
        PhysicsManager* pm = new PhysicsManager();
        GameLoop::getSingleton().addSynchronizedTask( pm );
        mWorld = new BSPWorld( );
		mInterpreter=new RubyInterpreter();
	//	mInterpreter->initializeInterpreter();
		//mWorld = new NatureWorld();
		//mWorld = new TerrainWorld();
		new ActorManager( );

		mWorld->loadScene("testraum.bsp");
		mWorld->setSkyBox(true, "rl/dsa07");
		//mWorld->loadScene("rastullah-testworld.cfg");
		//mWorld->loadScene("terrain.cfg");

        return true;
    }

	void CoreSubsystem::initializeResources()
    {
		new XmlResourceManager();
        // Load resource paths from config file
        ConfigFile cf;
        cf.load("modules/modules.cfg");

        // Go through all settings in the file
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        String key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

			if (key.compare("common") == 0)
			{
				initializeModule(value);
				mCommonModules.push_back(value);
			}
			else if (key.compare("module") == 0)
			{
				//initializeModule(value);
				mActivatableModules.push_back(value);
			}
        }
    }

	void CoreSubsystem::initializeModule(std::string module)
	{
		std::string moduleDir = "modules/"+module;
		ConfigFile cf;
		cf.load(moduleDir+"/conf/moduleconfig.cfg");
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

		bool hasGui = false, hasDialogs = false;

		std::string key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

			if (key.compare("TextureArchive") == 0)
				ResourceManager::addCommonArchiveEx(moduleDir+"/materials/"+value, "Zip");
			else if (key.compare("Archive") == 0)
				ResourceManager::addCommonArchiveEx(moduleDir+"/"+value, "Zip");
			else if (key.compare("ContainsGUI") == 0)
				hasGui = true;
			else if (key.compare("ContainsDialogs") == 0)
				hasDialogs = true;
		}

		ResourceManager::addCommonSearchPath(moduleDir+"/dsa");
		ResourceManager::addCommonSearchPath(moduleDir+"/materials");
		ResourceManager::addCommonSearchPath(moduleDir+"/maps");
		ResourceManager::addCommonSearchPath(moduleDir+"/models");			
		ResourceManager::addCommonSearchPath(moduleDir+"/sound");
		if (hasGui)
			ResourceManager::addCommonSearchPath(moduleDir+"/gui/imagesets");
		if (hasDialogs)
			ResourceManager::addCommonSearchPath(moduleDir+"/dialogs");
	}

	void CoreSubsystem::unloadModule(std::string module)
	{
		//TODO: unloadModule
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

	StringVector CoreSubsystem::getActiveModules() const
	{
		StringVector modules(mCommonModules);
		modules.push_back(mActiveModule);
		return modules;
	}

	const StringVector& CoreSubsystem::getCommonModules() const
	{
		return mCommonModules;
	}

	const StringVector& CoreSubsystem::getActivatableModules() const
	{
		return mActivatableModules;
	}

	void CoreSubsystem::setActiveModule(const String module)
	{
		if (mActiveModule.length() > 0)
			unloadModule(mActiveModule);
		initializeModule(module);
		mActiveModule = module;
	}


}
