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
#include "ActorFactory.h"
#include "GameLoop.h"


template<> rl::CoreSubsystem* Singleton<rl::CoreSubsystem>::ms_Singleton = 0;

namespace rl {

	CoreSubsystem& CoreSubsystem::getSingleton(void)
	{
		return Singleton<CoreSubsystem>::getSingleton();
	}

	CoreSubsystem* CoreSubsystem::getSingletonPtr(void)
	{
		return Singleton<CoreSubsystem>::getSingletonPtr();
	}

	CoreSubsystem::CoreSubsystem()
	{
		mWorld = 0;
        initializeCoreSubsystem();
	}

    CoreSubsystem::~CoreSubsystem() 
    {  
		if(mWorld != 0)
			delete mWorld;

		mWorld = 0;
        Root::getSingleton().shutdown();
    }

	void CoreSubsystem::startCore(void)
    {		
		Root::getSingleton().startRendering();
    }

    void CoreSubsystem::log(const String& msg)
    {		
        LogManager::getSingleton().getLog( "rlCore.log" )->logMessage(msg);
    }
    
	bool  CoreSubsystem::setupConfiguration(void)
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        // Ein kleiner C-Trick: Wir versuchen erstmal die Konfiguration
        // wiederherzustellen. Klappt das nicht (Rueckgabe: FALSE), dann
        // zeigen wir den Konf.dialog.
        if( Root::getSingleton().restoreConfig()
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

	bool CoreSubsystem::initializeCoreSubsystem(void)
    {
		#if OGRE_PLATFORM == PLATFORM_WIN32
			new Root( "plugins-win.cfg", "rastullah.cfg", "ogre.log" );
		#elif OGRE_PLATFORM == PLATFORM_LINUX
			new Root( "plugins-linux.cfg", "rastullah.cfg", "ogre.log" );
		#else
			new Root( "plugins-mac.cfg", "rastullah.cfg", "ogre.log" );
		#endif

        initializeResources();

        bool carryOn = setupConfiguration();
        
		if (!carryOn) 
			return false;

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipMaps(5);
        Log* log = LogManager::getSingleton().createLog( "rlCore.log" );
        log->setLogDetail( LL_BOREME );
		
		new GameLoop();
        PhysicsManager* pm = new PhysicsManager();
        GameLoop::getSingleton().addSynchronizedTask( pm );
        mWorld = new BSPWorld( );
		//mWorld = new NatureWorld();
		//mWorld = new TerrainWorld();
		new ActorFactory( );

		mWorld->loadScene("testraum.bsp");
		mWorld->setSkyBox(true, "rl/dsa07");
		//mWorld->loadScene("rastullah-testworld.cfg");
		//mWorld->loadScene("terrain.cfg");

        return true;
    }

	void CoreSubsystem::initializeResources(void)
    {
        // Load resource paths from config file
        ConfigFile cf;
        cf.load("resources.cfg");

        // Go through all settings in the file
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        String typeName, archName;
        while (i.hasMoreElements())
        {
            typeName = i.peekNextKey();
            archName = i.getNext();
            ResourceManager::addCommonArchiveEx( archName, typeName );
        }
    }

	World* CoreSubsystem::getWorld()
    { 
        return mWorld;
    }

    void  CoreSubsystem::makeScreenshot( const String& sName )
	{
		Ogre::Root::getSingleton().getAutoCreatedWindow()->
            writeContentsToTimestampedFile(sName, ".jpg");
	}

}
