#include <CEGUI.h>
#include <renderers/OgreGUIRenderer/ogrerenderer.h>

#include "FixRubyHeaders.h"

#include "UiSubsystem.h"
#include "CoreSubsystem.h"

#include "RubyInterpreter.h"
#include "CeConsole.h"
#include "DebugWindow.h"
#include "ThirdPersonGameController.h"
#include "InputManager.h"

#include "GameLoop.h"
#include "ActorFactory.h"
#include "CameraActor.h"
#include "World.h"
#include "DialogWindow.h"

template<> rl::UiSubsystem* Singleton<rl::UiSubsystem>::ms_Singleton = 0;

namespace rl {
	const char* UiSubsystem::CEGUI_ROOT = "RootWindow";

	UiSubsystem& UiSubsystem::getSingleton(void)
	{
		return Singleton<UiSubsystem>::getSingleton();
	}

	UiSubsystem* UiSubsystem::getSingletonPtr(void)
	{
		return Singleton<UiSubsystem>::getSingletonPtr();
	}

	UiSubsystem::UiSubsystem()
	{
        mRequestExit = false;

        initializeUiSubsystem();
	}

    UiSubsystem::~UiSubsystem() 
    {  
		delete CeConsole::getSingletonPtr();
        delete InputManager::getSingletonPtr();

        GameLoop::getSingleton().removeSynchronizedTask(mGameController);
    }

    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

        World* world = CoreSubsystem::getSingleton().getWorld();

		CEGUI::OgreRenderer* rend = 
			new CEGUI::OgreRenderer(Ogre::Root::getSingleton().getAutoCreatedWindow(), 
									Ogre::RENDER_QUEUE_OVERLAY, 
									false, 
									3000,
									CoreSubsystem::getSingleton().getWorld()->getSceneManager());
		new System(rend);

		// load scheme and set up defaults
		CEGUI::SchemeManager::getSingleton().loadScheme((utf8*)"testscheme.xml");
		System::getSingleton().setDefaultMouseCursor((utf8*)"TaharezImagery", (utf8*)"MouseArrow");
		System::getSingleton().setDefaultFont((utf8*)"Tahoma-8");
		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)CEGUI_ROOT);
		sheet->setSize(
			Absolute, 
			CEGUI::Size(
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);

		//Initializing InputManager
        new InputManager();
		
		//new DebugWindow();
        //DebugWindow::getSingleton().show();

		new CeConsole();
        CeConsole::getSingleton().setInterpreter( new RubyInterpreter() );
        CeConsole::getSingleton().getInterpreter()->initializeInterpreter();

        //InputManager::getSingleton().addKeyListener(DebugWindow::getSingletonPtr());

        CameraActor* camera = dynamic_cast<CameraActor*>(
            ActorFactory::getSingleton().getActor("DefaultCamera"));
		
        if( camera != 0 )
        {
            mGameController = new ThirdPersonGameController(camera->getOgreCamera(),
                0, //mHero->getEntity(),
                world->getSceneManager()->getSuggestedViewpoint().position);
            GameLoop::getSingleton().addSynchronizedTask(mGameController);
        }

//		DialogWindow::runTest();
    }

    void UiSubsystem::requestExit()
    {
        mRequestExit = true;
    }
    
    bool UiSubsystem::isRequestingExit() const
    {
        return mRequestExit;
    }

	void UiSubsystem::writeToConsole(std::string text)
	{
		CeConsole::getSingleton().write(text);
	}
}
