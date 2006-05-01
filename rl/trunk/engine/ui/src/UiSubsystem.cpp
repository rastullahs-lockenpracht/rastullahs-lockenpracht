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

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "CutsceneCharacterController.h"
#include "DialogCharacterController.h"
#include "DsaManager.h"
#include "Exception.h"
#include "FreeFlightCharacterController.h"
#include "GameObject.h"
#include "GameLoggerWindow.h"
#include "GameLoop.h"
#include "InputManager.h"
#include "Logger.h"
#include "MovementCharacterController.h"
#include "Person.h"
#include "RBCombat.h"
#include "RTCombat.h"
#include "RTCombatCharacterController.h"
#include "ScriptWrapper.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

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
        mCharacterController(0),
        mHero(0),
        mCharacter(0),
		mInCombat(false)
	{
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Init Start");
		initializeUiSubsystem();
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Init Ende");
	}

    UiSubsystem::~UiSubsystem() 
    {  
        CoreSubsystem::getSingletonPtr()->getWorld()->removeSceneChangeListener(this);

		delete mWindowFactory;
		delete mWindowManager;

        GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
		delete mCharacterController;
		delete mCommandMapper;

		delete mInputManager;
	}
	
    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

        Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();
		
		Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
		OgreCEGUIRenderer* rend = 
			new OgreCEGUIRenderer(window, 
								Ogre::RENDER_QUEUE_OVERLAY, 
								false, 
								3000,
								sceneMgr);

		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
		new System(rend, NULL, new OgreCEGUIResourceProvider(), (utf8*)"cegui.config");
		CEGUI::Logger::getSingleton().setLoggingLevel(rl::Logger::getSingleton().getCeGuiLogDetail());
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CEGUI-System initialisiert", "UiSubsystem::initializeUiSubsystem");
        
		// load scheme and set up defaults
		///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
		System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook-Images", (utf8*)"MouseArrow");
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Mauszeiger", "UiSubsystem::initializeUiSubsystem");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)CEGUI_ROOT);
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Rootfenster", "UiSubsystem::initializeUiSubsystem");
		sheet->setSize(
			Absolute, 
			CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);
		System::getSingleton().setTooltip("RastullahLook/Tooltip");
        Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CEGUI geladen", "UiSubsystem::initializeUiSubsystem");

		mWindowManager = new WindowManager();

		//Initializing InputManager
        mInputManager = new InputManager();
		mInputManager->loadKeyMapping(ConfigurationManager::getSingleton().getKeymap());
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Keymap geladen", "UiSubsystem::initializeUiSubsystem");

		mCommandMapper = new CommandMapper();
		mCommandMapper->loadCommandMap(ConfigurationManager::getSingleton().getInputConfigPath());
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "UI-Manager geladen", "UiSubsystem::initializeUiSubsystem");

		mWindowFactory = new WindowFactory();

        CoreSubsystem::getSingletonPtr()->getWorld()->addSceneChangeListener(this);
    }

	Person* UiSubsystem::getActiveCharacter() const
	{
		return mCharacter;
	}

	void UiSubsystem::setActiveCharacter(Person* person)
	{
        // Nur wenn es sich verändert hat
        if( person != mCharacter )
        {
            if( mCharacter != NULL )
			{
                ScriptWrapper::getSingleton().disowned( mCharacter );
				mCharacter->getActor()->detach(CoreSubsystem::getSingleton().getSoundListener());
			}

            ScriptWrapper::getSingleton().owned( person );
            mCharacter = person;
            World* world = CoreSubsystem::getSingletonPtr()->getWorld();
            world->setActiveActor(person->getActor());
		    
			mWindowFactory->setActiveCharacter(person);

			mCharacter->getActor()->attach(CoreSubsystem::getSingleton().getSoundListener());
			Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "SoundListener attached.");
            
            Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Actor set");

			setCharacterController(CharacterController::CTRL_MOVEMENT);
        }
	}

	void UiSubsystem::setCharacterController(CharacterController::ControllerType type)
	{
		if (mCharacterController != NULL)
		{
			if (mCharacterController->getType() == type)
				return;

			GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
			delete mCharacterController;
			Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Old CharacterController deleted.");
		}

        Actor* camera = ActorManager::getSingleton().getActor("DefaultCamera");
		switch(type)
		{
		case CharacterController::CTRL_MOVEMENT:
			mCharacterController = new MovementCharacterController(camera, mCharacter);
			if (!PhysicsManager::getSingleton().isEnabled())
			{
				PhysicsManager::getSingleton().setEnabled(true);
			}
			break;
		case CharacterController::CTRL_FREEFLIGHT:
			mCharacterController = new FreeFlightCharacterController(camera, CoreSubsystem::getSingleton().getWorld()->getActiveActor());
			break;
		case CharacterController::CTRL_DIALOG:
			mCharacterController = new DialogCharacterController(camera, CoreSubsystem::getSingleton().getWorld()->getActiveActor());
			break;
		case CharacterController::CTRL_CUTSCENE:
			mCharacterController = new CutsceneCharacterController(camera);
			break;
		case CharacterController::CTRL_RTCOMBAT:
			mCharacterController = new RTCombatCharacterController(camera, mCharacter);
			break;
		default:
			Throw(IllegalArgumentException, "Unknown CharacterControllerType.");
		}
		
		mCharacterController->setCommandMapper(mCommandMapper);
		mInputManager->setCharacterController(mCharacterController);

	    Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CharacterController created.");
		GameLoopManager::getSingleton().addSynchronizedTask(mCharacterController, FRAME_STARTED );
        Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CharacterController task added.");
	}

	void UiSubsystem::useDefaultAction(GameObject* obj, Creature* actor)
	{
		obj->doAction(obj->getDefaultAction(actor), actor, NULL); //TODO: Target
	}

	void UiSubsystem::usePickedObjectDefaultActions()
	{
		GameObject* pickedObject = mInputManager->getPickedObject();

		if (pickedObject != NULL)
			useDefaultAction(pickedObject, getActiveCharacter());
	}

	void UiSubsystem::toggleObjectPicking()
	{
		mInputManager->setObjectPickingActive(true);
	}

	void UiSubsystem::startRBCombat(RBCombat* combat)
	{
		setCombatMode(true);
		mWindowFactory->showCombatWindow(combat, getActiveCharacter());
	}

	void UiSubsystem::startRTCombat(RTCombat* combat)
	{
		combat->setLogger(mWindowFactory->getGameLogger());
		setCharacterController(CharacterController::CTRL_RTCOMBAT);
		GameLoopManager::getSingleton().addAsynchronousTask(combat);
		dynamic_cast<RTCombatCharacterController*>(mCharacterController)->setCombat(combat);
	}

	void UiSubsystem::setCombatMode(bool inCombat)
	{
		mInCombat = inCombat;
		//TODO: Irgendwann später, UI auf Kampfmodus umstellen
	}

	bool UiSubsystem::isInCombatMode()
	{
		return mInCombat;
	}

    CharacterController* UiSubsystem::getCharacterController() const
    {
        return mCharacterController;
    }

	CharacterController::ControllerType UiSubsystem::getCharacterControllerType() const
	{
		if (mCharacterController == NULL)
			return CharacterController::CTRL_NONE;

		return mCharacterController->getType();
	}

    void UiSubsystem::onBeforeClearScene()
    {
		if (mCharacterController != NULL)
		{
			GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
			delete mCharacterController;
			Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Old CharacterController deleted.");
            mCharacterController = NULL;
		}
    }
}
