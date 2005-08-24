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

#include "UiPrerequisites.h"

#include "UiSubsystem.h"

#include <OgreCEGUIRenderer.h>
#include <OgreCEGUIResourceProvider.h>

#include "RubyInterpreter.h"
#include "CoreSubsystem.h"
#include "Logger.h"
#include "Console.h"
#include "DebugWindow.h"
#include "GameController.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "CommandMapperWindow.h"
#include "MessageWindow.h"
#include "MainMenuWindow.h"
#include "GameLoggerWindow.h"
#include "TargetSelectionWindow.h"
#include "CharacterStateWindow.h"
#include "ContainerContentWindow.h"
#include "InGameMenuWindow.h"

#include "GameLoop.h"
#include "ActorManager.h"
#include "Actor.h"
#include "World.h"

#include "ScriptObjectRepository.h"

// BEGIN TEST
#include "Person.h"
#include "CharacterSheetWindow.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "DialogWindow.h"
#include "PlaylistWindow.h"
#include "Primitive.h"
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
		log(Ogre::LML_TRIVIAL, "Init Start");
		initializeUiSubsystem();
		log(Ogre::LML_TRIVIAL, "Init Ende");
	}

    UiSubsystem::~UiSubsystem() 
    {  
		delete Console::getSingletonPtr();
        delete InputManager::getSingletonPtr();

        GameLoopManager::getSingleton().removeSynchronizedTask(mGameController);
    }
	
    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

        log(Ogre::LML_TRIVIAL, "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();
		
		Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
		log(Ogre::LML_TRIVIAL, "Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
		OgreCEGUIRenderer* rend = 
			new OgreCEGUIRenderer(window, 
								Ogre::RENDER_QUEUE_OVERLAY, 
								false, 
								3000,
								sceneMgr);

		log(Ogre::LML_TRIVIAL, "Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
		new System(rend, NULL, new OgreCEGUIResourceProvider(), (utf8*)"cegui.config"); 
		log(Ogre::LML_TRIVIAL, "CEGUI-System initialisiert", "UiSubsystem::initializeUiSubsystem");
        
		// load scheme and set up defaults
		///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
		System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook", (utf8*)"MouseArrow");
		log(Ogre::LML_TRIVIAL, "Mauszeiger", "UiSubsystem::initializeUiSubsystem");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)CEGUI_ROOT);
		log(Ogre::LML_TRIVIAL, "Rootfenster", "UiSubsystem::initializeUiSubsystem");
		sheet->setSize(
			Absolute, 
			CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);
		System::getSingleton().setTooltip("RastullahLook/Tooltip");
        log(Ogre::LML_TRIVIAL, "CEGUI geladen", "UiSubsystem::initializeUiSubsystem");

		//Initializing InputManager
		new CommandMapper();
        new InputManager();
		log(Ogre::LML_TRIVIAL, "UI-Manager geladen", "UiSubsystem::initializeUiSubsystem");

		InputManager::getSingleton().loadKeyMapping("keymap-german.xml");
		log(Ogre::LML_TRIVIAL, "Keymap geladen", "UiSubsystem::initializeUiSubsystem");

		new DebugWindow();
		new Console();
		((RubyInterpreter*)CoreSubsystem::getSingleton().getInterpreter() )->initializeInterpreter( (VALUE(*)(...))&UiSubsystem::consoleWrite );

		new TargetSelectionWindow();
			      
		mGameLogger = new GameLoggerWindow();
		mCharacterStateWindow = new CharacterStateWindow();
		mInGameMenuWindow = new InGameMenuWindow();
		mCharacterSheet = new CharacterSheetWindow();

        runTest();
    }

    void UiSubsystem::requestExit()
    {
		log(Ogre::LML_TRIVIAL, "Start", "UiSubsystem::requestExit");
		//TODO: Vorher mal nachfragen, ob wirklich beendet werden soll
    	GameLoopManager::getSingleton().quitGame();
	}
    
    void UiSubsystem::writeToConsole(Ogre::String text)
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
        // Nur wenn es sich verändert hat
        if( person != mCharacter )
        {
            if( mCharacter != NULL )
                ScriptObjectRepository::getSingleton().disown( mCharacter );

            ScriptObjectRepository::getSingleton().own( person );

            // @todo alte Sachen löschen
            mCharacter = person;
            Actor* camera = ActorManager::getSingleton().getActor("DefaultCamera");
            mGameController = new GameController(camera, person->getActor());
		    log(Ogre::LML_TRIVIAL, "GameController created.");
            GameLoopManager::getSingleton().addSynchronizedTask(mGameController, FRAME_STARTED);
            log(Ogre::LML_TRIVIAL, "GameController task added.");
            World* world = CoreSubsystem::getSingletonPtr()->getWorld();
            world->setActiveActor(person->getActor());
		    mCharacterStateWindow->setCharacter(person);
		    mCharacterStateWindow->update();
            log(Ogre::LML_TRIVIAL, "Actor set");
        }
	}

	void UiSubsystem::showActionChoice(GameObject* obj)
	{
		ActionChoiceWindow* w = new ActionChoiceWindow(UiSubsystem::getSingleton().getActiveCharacter());
		int numActions = w->showActionsOfObject(obj);
		if (numActions > 0)
			w->setVisible(true);
	}

	void UiSubsystem::showCharacterActionChoice()
	{
		showActionChoice(getActiveCharacter());
	}

	void UiSubsystem::showPickedObjectActions()
	{
		GameObject* pickedObject = InputManager::getSingleton().getPickedObject();

		if (pickedObject != NULL)
			showActionChoice(pickedObject);
	}

	void UiSubsystem::showContainerContent(Container* container)
	{
		ContainerContentWindow* wnd = new ContainerContentWindow(container);
		wnd->setVisible(true);
	}

	bool UiSubsystem::showInputOptionsMenu(Creature* actionHolder)
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

	void UiSubsystem::showTargetWindow()
	{
		TargetSelectionWindow::getSingleton().setAction(NULL);
		TargetSelectionWindow::getSingleton().setVisible(true);
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

	void UiSubsystem::toggleDialogWindow()
	{
		DialogWindow* dwnd = DialogWindow::getSingletonPtr();
		dwnd->setVisible(!dwnd->isVisible());
	}

	void UiSubsystem::toggleGameLogWindow()
	{
		mGameLogger->setVisible(!mGameLogger->isVisible());
	}

	void UiSubsystem::toggleObjectPicking()
	{
		InputManager::getSingleton().setObjectPickingActive(true);
	}

	void UiSubsystem::showCharacterSheet()
	{
		if (mCharacterSheet->isVisible())
		{
			mCharacterSheet->setCharacter(NULL);
			mCharacterSheet->setVisible(false);
		}
		else
		{
			mCharacterSheet->setCharacter(getActiveCharacter());
			mCharacterSheet->setVisible(true);
		}
	}

	void UiSubsystem::showCharacterSheet(Person* chara)
	{
		CharacterSheetWindow* wnd = new CharacterSheetWindow();
		wnd->setCharacter(chara);
		wnd->setVisible(true);
	}

	void UiSubsystem::toggleCharacterStateWindow()
	{
		mCharacterStateWindow->setVisible(!mCharacterStateWindow->isVisible());
	}

	void UiSubsystem::toggleInGameGlobalMenu()
	{
		mInGameMenuWindow->setVisible(!mInGameMenuWindow->isVisible());
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
		DialogWindow* dialog = new DialogWindow("startup.xml");  
	}

	void UiSubsystem::update()
	{
		mInGameMenuWindow->update();
	}
	
    GameController* UiSubsystem::getGameController()
    {
        return mGameController;
    }

	GameLoggerWindow* UiSubsystem::getGameLogger()
	{
		return mGameLogger;
	}

    void UiSubsystem::showPlaylist()
    {
        PlaylistWindow* wnd = new PlaylistWindow();
        wnd->setVisible(true);
    }

	void UiSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
	{
		Logger::getSingleton().log(level, "Ui", msg, ident);
	}
}
