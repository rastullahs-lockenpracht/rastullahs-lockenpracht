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
#include "ListenerMovable.h"
#include "Logger.h"
#include "MovementCharacterController.h"
#include "Person.h"
#include "ScriptWrapper.h"
#include "SoundManager.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <OgreCEGUIResourceProvider.h>

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
		mCharacterControllerType(CharacterController::CTRL_NONE),
        mHero(0),
        mCharacter(0),
		mInputManager(0),
		mWindowFactory(0),
		mWindowManager(0),
		mCommandMapper(0),
        mGuiRenderer(0),
        mGuiResourceProvider(0),
        mGuiSystem(0)
	{
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Init Start");
		initializeUiSubsystem();
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Init Ende");
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

        delete mGuiSystem;
        delete mGuiResourceProvider;
        delete mGuiRenderer;
	}
	
    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

        Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE,
            "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();
		
		Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE,
            "Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
		mGuiRenderer = new OgreCEGUIRenderer(window,
            Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);

		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE,
            "Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
        mGuiResourceProvider = new OgreCEGUIResourceProvider();
		mGuiSystem = new System(mGuiRenderer, NULL, mGuiResourceProvider, (utf8*)"cegui.config");
		CEGUI::Logger::getSingleton().setLoggingLevel(
            rl::Logger::getSingleton().getCeGuiLogDetail());
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE,
            "CEGUI-System initialisiert", "UiSubsystem::initializeUiSubsystem");
        
		// load scheme and set up defaults
		///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
		System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook-Images",
            (utf8*)"MouseArrow");
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Mauszeiger",
            "UiSubsystem::initializeUiSubsystem");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet",
            (utf8*)CEGUI_ROOT);
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Rootfenster",
            "UiSubsystem::initializeUiSubsystem");
		sheet->setSize(
			Absolute, 
			CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);
		System::getSingleton().setTooltip("RastullahLook/Tooltip");
        Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "CEGUI geladen",
            "UiSubsystem::initializeUiSubsystem");

		mWindowManager = new WindowManager();

		//Initializing InputManager
        mInputManager = new InputManager();
		mInputManager->loadKeyMapping(ConfigurationManager::getSingleton().getKeymap());
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Keymap geladen",
            "UiSubsystem::initializeUiSubsystem");

		mCommandMapper = new CommandMapper();
		mCommandMapper->loadCommandMap(ConfigurationManager::getSingleton().getInputConfigPath());
		Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "UI-Manager geladen",
            "UiSubsystem::initializeUiSubsystem");

		mWindowFactory = new WindowFactory();

        CoreSubsystem::getSingletonPtr()->getWorld()->addSceneChangeListener(this);
		GameLoopManager::getSingleton().addSynchronizedTask(this, FRAME_ENDED);
    }


	CEGUI::OgreCEGUIRenderer* UiSubsystem::getGUIRenderer()
	{
		return mGuiRenderer;
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
				mCharacter->getActor()->detach(SoundManager::getSingleton().getListenerActor());
			}

            World* world = CoreSubsystem::getSingletonPtr()->getWorld();

			if (person == NULL)
			{
				mCharacter = NULL;
                world->setActiveActor( ActorManager::getSingleton().getActor("DefaultCamera") );
			}
			else
			{
				ScriptWrapper::getSingleton().owned( person );
				mCharacter = person;
				
				world->setActiveActor(person->getActor());
				mWindowFactory->setActiveCharacter(person);

				mCharacter->getActor()->attach(SoundManager::getSingleton().getListenerActor());
				Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "SoundListener attached.");
	            
				setCharacterController(CharacterController::CTRL_MOVEMENT);
			}
        }
	}

	void UiSubsystem::requestCharacterControllerSwitch(CharacterController::ControllerType type)
	{
		mCharacterControllerType = type;
	}

	void UiSubsystem::setCharacterController(CharacterController::ControllerType type)
	{
		if (mCharacterController != NULL)
		{
			if (mCharacterController->getType() == type)
				return;

			GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
			delete mCharacterController;
            mCharacterController = NULL;
			Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE,
                "Old CharacterController deleted.");
		}
        
        if( mCharacter == NULL )
            type = CharacterController::CTRL_FREEFLIGHT;

   		if( type == CharacterController::CTRL_NONE )
        {
			mCharacterController = NULL;
			return;
        }

        Actor* camera = ActorManager::getSingleton().getActor("DefaultCamera");
        if (camera == NULL)
        {
            requestCharacterControllerSwitch(type);
            return;
        }

		if (type == CharacterController::CTRL_MOVEMENT)
        {
			mCharacterController = new MovementCharacterController(camera, mCharacter);
			if (!PhysicsManager::getSingleton().isEnabled())
			{
				PhysicsManager::getSingleton().setEnabled(true);
			}
        }
		else if (type == CharacterController::CTRL_FREEFLIGHT)
        {
			mCharacterController = new FreeFlightCharacterController(camera,
                CoreSubsystem::getSingleton().getWorld()->getActiveActor());
        }
		else if (type == CharacterController::CTRL_DIALOG)
        {
			mCharacterController = new DialogCharacterController(camera,
                CoreSubsystem::getSingleton().getWorld()->getActiveActor());
        }
		else if (type == CharacterController::CTRL_CUTSCENE)
        {
			mCharacterController = new CutsceneCharacterController(camera);
        }
        else
        {
			Throw(IllegalArgumentException, "Unknown CharacterControllerType.");
		}
		mCharacterControllerType = type;
		
		mCharacterController->setCommandMapper(mCommandMapper);
		mInputManager->setCharacterController(mCharacterController);

	    Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "CharacterController created.");
		GameLoopManager::getSingleton().addSynchronizedTask(mCharacterController, FRAME_STARTED );
        Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "CharacterController task added.");
	}

	void UiSubsystem::run(Ogre::Real timeElapsed)
	{
		setCharacterController(mCharacterControllerType);
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
            setCharacterController(CharacterController::CTRL_NONE);
			Logger::getSingleton().log(
                Logger::UI, 
                Logger::LL_MESSAGE,
                "Old CharacterController deleted.");
			setActiveCharacter(NULL);
		}
    }
}
