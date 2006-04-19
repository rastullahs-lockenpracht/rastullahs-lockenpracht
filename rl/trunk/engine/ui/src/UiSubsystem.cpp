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

#include "Exception.h"

#include "CoreSubsystem.h"
#include "Logger.h"
#include "DialogCharacterController.h"
#include "MovementCharacterController.h"
#include "FreeFlightCharacterController.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "WindowFactory.h"
#include "WindowManager.h"

#include "Combat.h"
#include "GameLoop.h"
#include "ActorManager.h"
#include "Actor.h"
#include "World.h"

#include "ScriptWrapper.h"

#include "Person.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "Primitive.h"
#include "Creature.h"

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
		delete mWindowFactory;
		delete mWindowManager;

        GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
		delete mCharacterController;

		delete InputManager::getSingletonPtr();
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
        new InputManager();
        new CommandMapper();
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "UI-Manager geladen", "UiSubsystem::initializeUiSubsystem");

		InputManager::getSingleton().loadKeyMapping("keymap-german.xml");
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Keymap geladen", "UiSubsystem::initializeUiSubsystem");

		mWindowFactory = new WindowFactory();
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

			setCharacterController(UiSubsystem::CTRL_MOVEMENT);
        }
	}

	void UiSubsystem::setCharacterController(ControllerType type)
	{
		mCharacterControllerType = type;

		if (mCharacterController != NULL)
		{
			GameLoopManager::getSingleton().removeSynchronizedTask(mCharacterController);
			delete mCharacterController;
			Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Old CharacterController deleted.");
		}

        Actor* camera = ActorManager::getSingleton().getActor("DefaultCamera");
		switch(type)
		{
		case CTRL_MOVEMENT:
			mCharacterController = new MovementCharacterController(camera, mCharacter);
			if (!PhysicsManager::getSingleton().isEnabled())
			{
				PhysicsManager::getSingleton().setEnabled(true);
			}
			break;
		case CTRL_FREEFLIGHT:
			mCharacterController = new FreeFlightCharacterController(camera, CoreSubsystem::getSingleton().getWorld()->getActiveActor());
			break;
		case CTRL_DIALOG:
			mCharacterController = new DialogCharacterController(camera, CoreSubsystem::getSingleton().getWorld()->getActiveActor());
			break;
		default:
			Throw(IllegalArgumentException, "Unknown CharacterControllerType.");
		}
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
		GameObject* pickedObject = InputManager::getSingleton().getPickedObject();

		if (pickedObject != NULL)
			useDefaultAction(pickedObject, getActiveCharacter());
	}

	void UiSubsystem::toggleObjectPicking()
	{
		InputManager::getSingleton().setObjectPickingActive(true);
	}

	void UiSubsystem::startCombat(Combat* combat)
	{
		setCombatMode(true);
		mWindowFactory->showCombatWindow(combat, getActiveCharacter());
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

	UiSubsystem::ControllerType UiSubsystem::getCharacterControllerType() const
	{
		return mCharacterControllerType;
	}
}
