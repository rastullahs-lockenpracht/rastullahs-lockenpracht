/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "UiSubsystem.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

#ifdef __APPLE__
#   include <OgreCEGUIRenderer/OgreCEGUIRenderer.h>
#else
#   include <OgreCEGUIRenderer.h>
#endif

#include "Actor.h"
#include "AiMessages.h"
#include "CeGuiHelper.h"
#include "ConfigurationManager.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"
#include "ControlState.h"
#include "Exception.h"
#include "GameLoop.h"
#include "InputManager.h"
#include "Logger.h"
#include "Person.h"
#include "ScriptWrapper.h"
#include "SoundManager.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "GameObjectManager.h"

using namespace Ogre;
template<> rl::UiSubsystem* Singleton<rl::UiSubsystem>::ms_Singleton = 0;

namespace rl {
    const char* UiSubsystem::CEGUI_ROOT = "RootWindow";

    UiSubsystem::UiSubsystem() :
        mCharacter(NULL),
        mInputManager(NULL),
        mWindowFactory(NULL),
        mWindowManager(NULL),
        mGuiRenderer(NULL),
        mGuiResourceProvider(NULL),
        mGuiSystem(NULL)
    {
        mSceneClearingConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_SceneClearing>(
			    boost::bind(&UiSubsystem::onBeforeClearScene, this));
        mGameObjectsLoadedConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType<RLMSG_SAVEGAME_GOS_LOADED> >(
                boost::bind(&UiSubsystem::onGameObjectsLoaded, this));
        mBeforeLoadingGameObjectsConnection = 
            MessagePump::getSingleton().addMessageHandler<MessageType<RLMSG_SAVEGAME_LOADING> >(
                boost::bind(&UiSubsystem::onBeforeGameObjectsLoaded, this));
        mWindowFactory = new WindowFactory();
    }

    UiSubsystem::~UiSubsystem()
    {
        delete mWindowFactory;
        delete mWindowManager;

        delete mInputManager;

        delete mGuiSystem;
        delete mGuiRenderer;
    }

    void UiSubsystem::initializeSubsystem()
    {
        using namespace CEGUI;

        LOG_MESSAGE2(Logger::UI,
            "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();

        LOG_MESSAGE2(Logger::UI,
            "Initializing CEGUI Renderer.", "UiSubsystem::initializeUiSubsystem");
        mGuiRenderer = new OgreCEGUIRenderer(CoreSubsystem::getSingleton().getRenderWindow(),
            Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);

        LOG_MESSAGE2(Logger::UI,
            "Initializing CEGUI System.", "UiSubsystem::initializeUiSubsystem");
        mGuiResourceProvider = mGuiRenderer->createResourceProvider();
        //CEGUI::System::setDefaultXMLParserName("XercesParser");
        mGuiSystem = new System(mGuiRenderer, mGuiResourceProvider,
            NULL, NULL, (utf8*)"cegui.config", ConfigurationManager::getSingleton().getCeguiLogFile());
        CEGUI::Logger::getSingleton().setLoggingLevel(
            rl::Logger::getSingleton().getCeGuiLogDetail());
        LOG_MESSAGE2(Logger::UI,
            "CEGUI System initialized.", "UiSubsystem::initializeUiSubsystem");

        // load scheme and set up defaults
        ///@todo Hier sollte was Lookunabhaengiges rein!!! FIXME TODO BUG!
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
                CoreSubsystem::getSingleton().getRenderWindow()->getWidth(),
                CoreSubsystem::getSingleton().getRenderWindow()->getHeight())));
        sheet->setPosition(CeGuiHelper::asAbsolute(CEGUI::Point(0, 0)));
        System::getSingleton().setGUISheet(sheet);
        sheet->setZOrderingEnabled(true);
        sheet->moveToBack();
        System::getSingleton().setDefaultTooltip("RastullahLook/Tooltip");
        LOG_MESSAGE2(Logger::UI, "CEGUI initialized.",
            "UiSubsystem::initializeUiSubsystem");

        mWindowManager = new WindowManager();

        //Initializing InputManager
        mInputManager = new InputManager(CoreSubsystem::getSingleton().getRenderWindow());
        LOG_MESSAGE2(Logger::UI, "InputManager started.",
            "UiSubsystem::initializeUiSubsystem");

        mWindowFactory->initialize();
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

        // Nur wenn es sich veraendert hat
        if( person != mCharacter )
        {
            if( mCharacter != NULL )
            {
                ScriptWrapper::getSingleton().disowned( mCharacter );
                mCharacter->getActor()->detach(SoundManager::getSingleton().getListenerActor());
                mCharacter->setQueryFlags(mCharacter->getQueryFlags() & (~QUERYFLAG_PLAYER));
            }

            if (person == NULL)
            {
                mCharacter = NULL;
                mInputManager->clearControlStates();
            }
            else
            {
                ScriptWrapper::getSingleton().owned( person );
                mCharacter = person;
                mCharacter->addQueryFlag(QUERYFLAG_PLAYER);

                mWindowFactory->setActiveCharacter(person);

                mCharacter->getActor()->attach(SoundManager::getSingleton().getListenerActor());
                LOG_MESSAGE(Logger::UI, "SoundListener attached.");

                // Reset control stack for the new Character and set to movement.
                mInputManager->setControlState(CST_MOVEMENT);
            }
        }
    }

    bool UiSubsystem::onBeforeClearScene()
    {
        setActiveCharacter(NULL);
        // Remove control states here too, in case that there has not yet been a
        // person set active.
        mInputManager->clearControlStates();

        return true;
    }

    bool UiSubsystem::onGameObjectsLoaded()
    {
        if(mCharacterId != -1)
        {
            Person* person = static_cast<Person*>(GameObjectManager::getSingleton().getGameObject(mCharacterId));

            ScriptWrapper::getSingleton().owned( person );
            mCharacter = person;

            mWindowFactory->setActiveCharacter(person);

            mCharacter->getActor()->attach(SoundManager::getSingleton().getListenerActor());
            LOG_MESSAGE(Logger::UI, "SoundListener attached.");

            // Reset control stack for the new Character and set to movement.
            //mInputManager->setControlState(CST_MOVEMENT);
        }
        return false;
    }

    bool UiSubsystem::onBeforeGameObjectsLoaded()
    {
        LOG_MESSAGE(Logger::UI, "UiSubsystem::onBeforeGameObjectsLoaded()");
        if(mCharacter)
        {
            ScriptWrapper::getSingleton().disowned( mCharacter );
            mCharacter->getActor()->detach(SoundManager::getSingleton().getListenerActor());

            mWindowFactory->setActiveCharacter(NULL);

            mCharacterId = mCharacter->getId();
        }
        else
            mCharacterId = -1;

        //mInputManager->clearControlStates();
        return false;
    }
}
