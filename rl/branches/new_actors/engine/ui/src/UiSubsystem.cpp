/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest für VS)
#include <renderers/OgreGUIRenderer/ogrerenderer.h>
#include "UiSubsystem.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"
#include "Console.h"
#include "DebugWindow.h"
#include "GameController.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "CommandMapperWindow.h"
#include "MessageWindow.h"
#include "MainMenuWindow.h"
#include "WindowManager.h"

#include "GameLoop.h"
#include "ActorManager.h"
#include "Actor.h"
#include "World.h"

// BEGIN TEST
#include "Person.h"
#include "CharacterSheetWindow.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "DialogWindow.h"
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

	UiSubsystem::UiSubsystem() :
        mGameController(0),
        mHero(0),
        mCharacter(0),
		mInBattle(false)
	{
		log("Init Start");
		initializeUiSubsystem();
		log("Init Ende");
	}

    UiSubsystem::~UiSubsystem() 
    {  
		delete Console::getSingletonPtr();
        delete InputManager::getSingletonPtr();

        GameLoop::getSingleton().removeSynchronizedTask(mGameController);
    }
	
	void UiSubsystem::log(const String& msg, const String& ident)
	{
		if (ident.length() > 0)
			CoreSubsystem::getSingleton().log("Ui: ("+ident+") "+msg);
		else
			CoreSubsystem::getSingleton().log("Ui: "+msg);
	}

    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

        log("Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
		
		Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
		log("Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
		OgreRenderer* rend = 
			new OgreRenderer(window, 
								Ogre::RENDER_QUEUE_OVERLAY, 
								false, 
								3000,
								sceneMgr);

		log("Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
		new System(rend, NULL, (utf8*)"modules/common/gui/cegui.config");
        
		// load scheme and set up defaults
		System::getSingleton().setDefaultMouseCursor((utf8*)"TaharezLook", (utf8*)"MouseArrow");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)CEGUI_ROOT);
		sheet->setSize(
			Absolute, 
			Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, Point(0, 0));
		System::getSingleton().setGUISheet(sheet);
        log("CEGUI geladen", "UiSubsystem::initializeUiSubsystem");

		//Initializing InputManager
		new CommandMapper();
        new InputManager();
		new rl::WindowManager();
        log("UI-Manager geladen", "UiSubsystem::initializeUiSubsystem");

		InputManager::getSingleton().loadKeyMapping("keymap-german.xml");
		log("Keymap geladen", "UiSubsystem::initializeUiSubsystem");

		new DebugWindow();
		new Console();
		((RubyInterpreter*)CoreSubsystem::getSingleton().getInterpreter() )->initializeInterpreter( (VALUE(*)(...))&UiSubsystem::consoleWrite );
			      
        //runTest();
    }

    void UiSubsystem::requestExit()
    {
		log("Start", "UiSubsystem::requestExit");
		//TODO: Vorher mal nachfragen, ob wirklich beendet werden soll
    	GameLoop::getSingleton().quitGame();
	}
    
    void UiSubsystem::writeToConsole(std::string text)
	{
		Console::getSingleton().write(text);
	}

	VALUE UiSubsystem::consoleWrite(VALUE self, VALUE str)
	{
		Console::getSingleton().write(RubyInterpreter::val2str(str) + " \n");
		return Qnil;
	}

	Person* UiSubsystem::getActiveCharacter()
	{
		return mCharacter;
	}

	void UiSubsystem::setActiveCharacter(Person* person)
	{
		mCharacter = person;		
		Camera* camera = CoreSubsystem::getSingletonPtr()->getWorld()
		    ->getActiveCamera();
		CoreSubsystem::getSingleton().log("Camera created.");
		mGameController = new GameController(
            camera, person->getActor());
        CoreSubsystem::getSingleton().log("GameController created.");
		GameLoop::getSingleton().addSynchronizedTask(mGameController);
		CoreSubsystem::getSingleton().log("GameController task added.");
		World* world = CoreSubsystem::getSingletonPtr()->getWorld();
		world->setActiveActor(person->getActor());
		CoreSubsystem::getSingleton().log("Actor set");		
	}

	void UiSubsystem::showActionChoice(GameObject* obj)
	{
		ActionChoiceWindow* w = new ActionChoiceWindow(UiSubsystem::getSingleton().getActiveCharacter());
		w->showActionsOfObject(obj);
		w->setVisible(true);
	}

	void UiSubsystem::showCharacterActionChoice()
	{
		showActionChoice(getActiveCharacter());
	}

	bool UiSubsystem::showInputOptionsMenu(GameObject* actionHolder)
	{
		CommandMapperWindow* wnd = new CommandMapperWindow(actionHolder);
		wnd->setVisible(true);

		return true;
	}

	void UiSubsystem::showMessageWindow(const CeGuiString& message)
	{
		MessageWindow* w = new MessageWindow();
		w->setText(message);
		w->setVisible(true);
	}

	void UiSubsystem::showMainMenu(GameObject* actionHolder)
	{
		(new MainMenuWindow(actionHolder))->setVisible(true);
	}

	void UiSubsystem::toggleConsole()
	{
		Console* cons = Console::getSingletonPtr();
		cons->setVisible(!cons->isVisible());
	}

	void UiSubsystem::toggleDebugWindow()
	{
		DebugWindow* dbgwnd = DebugWindow::getSingletonPtr();
		dbgwnd->setVisible(!dbgwnd->isVisible());
	}

	void UiSubsystem::setBattleMode(bool inBattle)
	{
		mInBattle = inBattle;
		//TODO: Irgendwann später, UI auf Kampfmodus umstellen
	}

	bool UiSubsystem::isInBattleMode()
	{
		return mInBattle;
	}

	void UiSubsystem::runTest()
	{
		InputManager::getSingleton().setObjectPickingActive(true);
	}
	
    GameController* UiSubsystem::getGameController()
    {
        return mGameController;
    }
}
