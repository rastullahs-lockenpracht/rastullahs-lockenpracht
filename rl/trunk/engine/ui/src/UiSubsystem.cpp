#include "DialogWindow.h"
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
#include "CommandMapper.h"

#include "GameLoop.h"
#include "ActorManager.h"
#include "GameActor.h"
#include "CameraActor.h"
#include "World.h"

// BEGIN TEST
#include "Person.h"
#include "CharacterSheetWindow.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "DsaManager.h"
// END TEST

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
		new CommandMapper();
        new InputManager();
		new DebugWindow();
		new CeConsole();
		((RubyInterpreter*)CoreSubsystem::getSingleton().getInterpreter() )->initializeInterpreter( (VALUE(*)(...))&CeConsole::consoleWrite );
		
        CameraActor* camera = dynamic_cast<CameraActor*>(
            ActorManager::getSingleton().getActor("DefaultCamera"));
		
        GameActor* hero = dynamic_cast<GameActor*>(
            ActorManager::getSingleton().createGameActor("Held","held.mesh"));
        Ogre::Vector3 pos = world->getSceneManager()->getSuggestedViewpoint().position;
        hero->setPosition(pos.x, pos.y, pos.z);

        mGameController = new ThirdPersonGameController(
            camera->getOgreCamera(), hero);
        GameLoop::getSingleton().addSynchronizedTask(mGameController);
		world->setActiveActor(hero);
	      
        //runTest();
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

	void UiSubsystem::showActionChoice(GameObject* obj)
	{
		ActionChoiceWindow* w = new ActionChoiceWindow();
		w->showActionsOfObject(obj);
		w->setVisible(true);
	}

	void UiSubsystem::runTest()
	{
		InputManager::getSingleton().setObjectPickingActive(true);
		
		/*DialogWindow* dialog=new DialogWindow("startup.xml");
		dialog->setName("Rattenkind");
		dialog->setQuestion("Wad? Wer bist du denn?\nHusch, husch!\nAb ins Körbchen!!!");*/
/*		dialog->setQuestion("Wie ist dein Name?");
		dialog->addLine("Hab ich vergessen");
		dialog->addLine("Ich heisse %NAME%");
		dialog->addLine("Mein Name ist %NAME%");
		dialog->addLine("Man nennt mich %NAME%, oder auch %NAME%");
		dialog->addLine("Ich habe nichts zu sagen");

		dialog->setVariableValue("NAME", "Heinz-Peter");
		dialog->setName("Klaus-Dieter");
		dialog->show();*/		

		Person* held = DsaManager::getSingleton().getPerson(10000);
		CharacterSheetWindow* sheet = new CharacterSheetWindow();
		sheet->setCharacter(held);
		//sheet->hide();
		
		/*GameObject* o = new GameObject(4711, "Heiltrank", "Testheiltrank");
		Action* a1 = new Action("Trinken", "Heiltrank trinken");
		Action* a2 = new Action("Werfen", "Heiltrank werfen");
		Action* a3 = new Action("Ansehen", "Heiltrank ansehen");
		Action* a4 = new Action("Talent:Schätzen", "Wert schätzen");
		Action* a5 = new Action("Talent:Alchemie", "Heiltrank mit Alchemie untersuchen");
		ActionManager::getSingleton().registerAction(a1);
		ActionManager::getSingleton().registerAction(a2);
		ActionManager::getSingleton().registerAction(a3);
		ActionManager::getSingleton().registerAction(a4);
		ActionManager::getSingleton().registerAction(a5);
		o->addAction(a1);
		o->addAction(a2);
		o->addAction(a3);
		o->addAction(a4);
		o->addAction(a5);
		
		ActionChoiceWindow* wnd = new ActionChoiceWindow();
		wnd->showActionsOfObject(o);
		wnd->show();*/
	}
}
