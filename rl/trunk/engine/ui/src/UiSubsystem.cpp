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
#include "CeGuiHelper.h"
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

        GameLoop::getSingleton().removeTask(mCharacterController);
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
            "Initializing CEGUI Renderer.", "UiSubsystem::initializeUiSubsystem");
		mGuiRenderer = new OgreCEGUIRenderer(window,
            Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);

		LOG_MESSAGE2(Logger::UI,
            "Initializing CEGUI System.", "UiSubsystem::initializeUiSubsystem");
        mGuiResourceProvider = new OgreCEGUIResourceProvider();
        CEGUI::System::setDefaultXMLParserName("XercesParser");
		mGuiSystem = new System(mGuiRenderer, mGuiResourceProvider,
            NULL, NULL, (utf8*)"cegui.config", ConfigurationManager::getSingleton().getCeguiLogFile());
		CEGUI::Logger::getSingleton().setLoggingLevel(
            rl::Logger::getSingleton().getCeGuiLogDetail());
		LOG_MESSAGE2(Logger::UI,
            "CEGUI System initialized.", "UiSubsystem::initializeUiSubsystem");

		// load scheme and set up defaults
		///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
		System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook-Images",
            (utf8*)"MouseArrow");
		LOG_MESSAGE2(Logger::UI, "Mouse arrow loaded.",
            "UiSubsystem::initializeUiSubsystem");
		Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet",
            (utf8*)CEGUI_ROOT);
		LOG_MESSAGE2(Logger::UI, "CEGUI Root Window created.",
            "UiSubsystem::initializeUiSubsystem");
		sheet->setSize(
            CeGuiHelper::asAbsolute(CEGUI::Vector2(
                Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(),
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight())));
		sheet->setPosition(CeGuiHelper::asAbsolute(CEGUI::Point(0, 0)));
		System::getSingleton().setGUISheet(sheet);
        sheet->setZOrderingEnabled(true);
        sheet->moveToBack();
		System::getSingleton().setDefaultTooltip("RastullahLook/Tooltip");
        LOG_MESSAGE2(Logger::UI, "CEGUI initialized.",
            "UiSubsystem::initializeUiSubsystem");

		mWindowManager = new WindowManager();

		//Initializing InputManager
        mInputManager = new InputManager(Ogre::Root::getSingleton().getAutoCreatedWindow());
		LOG_MESSAGE2(Logger::UI, "InputManager started.",
            "UiSubsystem::initializeUiSubsystem");

		mInputManager->loadKeyMapping(ConfigurationManager::getSingleton().getKeymap());
		LOG_MESSAGE2(Logger::UI, "Keymap geladen",
            "UiSubsystem::initializeUiSubsystem");
        mInputManager->loadCommandMapping(ConfigurationManager::getSingleton().getInputConfigPath());
		LOG_MESSAGE2(Logger::UI, "UI-Manager loaded.",
            "UiSubsystem::initializeUiSubsystem");

        mWindowFactory->initialize();

        ///\XXX Not sensible to call setCharacterController each frame.
        /// A better control over removed and added tasks will resolve this.
        /// TG_INPUT is a bit of a stretch, but this doesn't fit anywhere else.
        GameLoop::getSingleton().addTask(this, GameLoop::TG_INPUT);
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

			GameLoop::getSingleton().removeTask(mCharacterController);
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
        GameLoop::getSingleton().addTask(mCharacterController, GameLoop::TG_INPUT);
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
			setActiveCharacter(NULL);
		}
    }

    const Ogre::String& UiSubsystem::getName() const
    {
        static String NAME = "UiSubsystem";

        return NAME;
    }

    void UiSubsystem::linkKeyToRubyCommand(const CeGuiString &key, const CeGuiString &command)
    {
        InputManager::getSingleton().linkKeyToRubyCommand(key,command);
    }

}
