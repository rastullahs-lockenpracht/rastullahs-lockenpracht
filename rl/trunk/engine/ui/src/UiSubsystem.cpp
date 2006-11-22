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

#include "UiSubsystem.h"

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
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
#include "VanityModeCharacterController.h"
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
        mCharacterController(NULL),
		mCharacterControllerType(CharacterController::CTRL_NONE),
        mHero(NULL),
        mCharacter(NULL),
		mInputManager(NULL),
		mWindowFactory(NULL),
		mWindowManager(NULL),
        mGuiRenderer(NULL),
        mGuiResourceProvider(NULL),
        mGuiSystem(NULL)
	{
        CoreSubsystem::getSingletonPtr()->getWorld()->addSceneChangeListener(this);
		mWindowFactory = new WindowFactory();
	}

    UiSubsystem::~UiSubsystem() 
    {  
        CoreSubsystem::getSingletonPtr()->getWorld()->removeSceneChangeListener(this);

		delete mWindowFactory;
		delete mWindowManager;

        GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
		delete mCharacterController;

		delete mInputManager;

        delete mGuiSystem;
        delete mGuiResourceProvider;
        delete mGuiRenderer;
	}
	
    void UiSubsystem::initializeSubsystem()
    {
		using namespace CEGUI;

        LOG_MESSAGE2(Logger::UI,
            "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();
		
		Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
		LOG_MESSAGE2(Logger::UI,
            "Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
		mGuiRenderer = new OgreCEGUIRenderer(window,
            Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);

		LOG_MESSAGE2(Logger::UI,
            "Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
        mGuiResourceProvider = new OgreCEGUIResourceProvider();
		mGuiSystem = new System(mGuiRenderer, NULL, mGuiResourceProvider, (utf8*)"cegui.config");
		CEGUI::Logger::getSingleton().setLoggingLevel(
            rl::Logger::getSingleton().getCeGuiLogDetail());
		LOG_MESSAGE2(Logger::UI,
            "CEGUI-System initialisiert", "UiSubsystem::initializeUiSubsystem");
        
		// load scheme and set up defaults
		///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
		System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook-Images",
            (utf8*)"MouseArrow");
		LOG_MESSAGE2(Logger::UI, "Mauszeiger",
            "UiSubsystem::initializeUiSubsystem");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet",
            (utf8*)CEGUI_ROOT);
		LOG_MESSAGE2(Logger::UI, "Rootfenster",
            "UiSubsystem::initializeUiSubsystem");
		sheet->setSize(
			Absolute, 
			CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);
        sheet->setZOrderingEnabled(true);
        sheet->moveToBack();
		System::getSingleton().setTooltip("RastullahLook/Tooltip");
        LOG_MESSAGE2(Logger::UI, "CEGUI geladen",
            "UiSubsystem::initializeUiSubsystem");

		mWindowManager = new WindowManager();

		//Initializing InputManager
        mInputManager = new InputManager(Ogre::Root::getSingleton().getAutoCreatedWindow());
		mInputManager->loadKeyMapping(ConfigurationManager::getSingleton().getKeymap());
		LOG_MESSAGE2(Logger::UI, "Keymap geladen",
            "UiSubsystem::initializeUiSubsystem");
        mInputManager->loadCommandMapping(ConfigurationManager::getSingleton().getInputConfigPath());
		LOG_MESSAGE2(Logger::UI, "UI-Manager geladen",
            "UiSubsystem::initializeUiSubsystem");

        mWindowFactory->initialize();

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
        // Ensure we have a sound listener
        if (SoundManager::getSingleton().getListenerActor() == NULL)
        {
            SoundManager::getSingleton().createListenerActor();
        }

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
				LOG_MESSAGE(Logger::UI, "SoundListener attached.");
	            
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
			LOG_MESSAGE(Logger::UI,
                "Old CharacterController deleted.");
		}
        
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
		else if (type == CharacterController::CTRL_VANITY_MODE)
        {
			mCharacterController = new VanityModeCharacterController(camera,
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
		
		mInputManager->setCharacterController(mCharacterController);

	    LOG_MESSAGE(Logger::UI, "CharacterController created.");
		GameLoopManager::getSingleton().addSynchronizedTask(mCharacterController, FRAME_ENDED );
        LOG_MESSAGE(Logger::UI, "CharacterController task added.");
	}

	void UiSubsystem::run(Ogre::Real timeElapsed)
	{
		setCharacterController(mCharacterControllerType);
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
            LOG_MESSAGE(Logger::UI, 
                "Old CharacterController deleted.");
			setActiveCharacter(NULL);
		}
    }

    const Ogre::String& UiSubsystem::getName() const
    {
        static String NAME = "UiSubsystem";

        return NAME;
    }

}
