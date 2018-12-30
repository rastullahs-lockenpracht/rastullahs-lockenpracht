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

#include <CEGUI/CEGUI.h>

#ifdef __APPLE__
#include <OgreCEGUIRenderer/OgreCEGUIRenderer.h>
#else
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/RendererModules/Ogre/ResourceProvider.h>
#endif

#include "Actor.h"
#include "AiMessages.h"
#include "CeGuiHelper.h"
#include "ConfigurationManager.h"
#include "ControlState.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "GameLoop.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "ItemDescriptionDragContainer.h"
#include "ItemIconDragContainer.h"
#include "Logger.h"
#include "RulesMessages.h"
#include "ScriptWrapper.h"
#include "SoundManager.h"
#include "UiMessages.h"
#include "WindowFactory.h"
#include "WindowManager.h"

using namespace Ogre;
template <> rl::UiSubsystem* Singleton<rl::UiSubsystem>::msSingleton = 0;

// this function needs to be in the CEGUI-namespace
namespace CEGUI
{
    void initializeOwnCeguiWindowFactories()
    {
        CEGUI::WindowFactoryManager& wfMgr = CEGUI::WindowFactoryManager::getSingleton();
        wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ItemDescriptionDragContainer)); // ohne rl:: davor hier!
        // wfMgr.addFalagardWindowMapping("ItemDescriptionDragContainer", "CEGUI/ItemDescriptionDragContainer", "",
        // "Falagard/Default");
        wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ItemIconDragContainer)); // ohne rl:: davor hier!
        // wfMgr.addFalagardWindowMapping("ItemIconDragContainer", "CEGUI/ItemIconDragContainer", "",
        // "Falagard/Default");
    }
}

namespace rl
{
    const char* UiSubsystem::CEGUI_ROOT = "RootWindow";

    UiSubsystem::UiSubsystem()
        : mCharacter(NULL)
        , mInputManager(NULL)
        , mWindowFactory(NULL)
        , mWindowManager(NULL)
        , mGuiRenderer(NULL)
        , mGuiResourceProvider(NULL)
        , mGuiSystem(NULL)
    {
        mWindowFactory = new WindowFactory();
        mSceneClearingConnection = MessagePump::getSingleton().addMessageHandler<MessageType_SceneClearing>(
            boost::bind(&UiSubsystem::onBeforeClearScene, this));
        mGameObjectsLoadedConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType<RLMSG_SAVEGAME_GOS_LOADED>>(
                boost::bind(&UiSubsystem::onGameObjectsLoaded, this));
        mBeforeLoadingGameObjectsConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType<RLMSG_SAVEGAME_LOADING>>(
                boost::bind(&UiSubsystem::onBeforeGameObjectsLoaded, this));
        mActiveCharacterChangedConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_ActivePlayerCharChanged>(
                boost::bind(&UiSubsystem::onActiveCharacterChanged, this, _1, _2));
        mAllPlayerCharactersDiedConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_AllPlayerCharsDied>(
                boost::bind(&UiSubsystem::onAllPlayerCharactersDied, this));
    }

    UiSubsystem::~UiSubsystem()
    {
        delete mInputManager;

        delete mWindowFactory;
        delete mWindowManager;

        mGuiSystem->destroy();
        mGuiRenderer->destroySystem();
    }

    void UiSubsystem::initializeSubsystem()
    {
        using namespace CEGUI;

        LOG_MESSAGE2(Logger::UI, "Initialisiere UI", "UiSubsystem::initializeUiSubsystem");
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();

        //        CEGUI::System::setDefaultXMLParserName("XercesParser");

        LOG_MESSAGE2(Logger::UI, "Initializing CEGUI Renderer.", "UiSubsystem::initializeUiSubsystem");
        mGuiRenderer = &OgreRenderer::create(*CoreSubsystem::getSingleton().getRenderWindow());

        LOG_MESSAGE2(Logger::UI, "Initializing CEGUI System.", "UiSubsystem::initializeUiSubsystem");
        mGuiResourceProvider = &mGuiRenderer->createOgreResourceProvider();

        mGuiSystem = &System::create(*mGuiRenderer, mGuiResourceProvider, nullptr, nullptr, nullptr, "cegui.config",
            ConfigurationManager::getSingleton().getCeguiLogFile());
        CEGUI::Logger::getSingleton().setLoggingLevel(rl::Logger::getSingleton().getCeGuiLogDetail());
        LOG_MESSAGE2(Logger::UI, "CEGUI System initialized.", "UiSubsystem::initializeUiSubsystem");

        // load scheme and set up defaults
        ///@todo Hier sollte was Lookunabhaengiges rein!!! FIXME TODO BUG!
        //        System::getSingleton().setDefaultMouseCursor("RastullahLook-Images", "MouseArrow");
        LOG_MESSAGE2(Logger::UI, "Mouse arrow loaded.", "UiSubsystem::initializeUiSubsystem");
        Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", CEGUI_ROOT);
        LOG_MESSAGE2(Logger::UI, "CEGUI Root Window created.", "UiSubsystem::initializeUiSubsystem");
        sheet->setSize(CEGUI::USize(CEGUI::UDim(0, CoreSubsystem::getSingleton().getRenderWindow()->getWidth()),
            CEGUI::UDim(0, CoreSubsystem::getSingleton().getRenderWindow()->getHeight())));
        sheet->setPosition(CeGuiHelper::asAbsolute(0, 0));
        System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

        sheet->setZOrderingEnabled(true);
        sheet->moveToBack();
        System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("RastullahLook/Tooltip");

        CEGUI::initializeOwnCeguiWindowFactories();

        LOG_MESSAGE2(Logger::UI, "CEGUI initialized.", "UiSubsystem::initializeUiSubsystem");

        mWindowManager = new WindowManager();

        // Initializing InputManager
        mInputManager = new InputManager(CoreSubsystem::getSingleton().getRenderWindow());
        LOG_MESSAGE2(Logger::UI, "InputManager started.", "UiSubsystem::initializeUiSubsystem");

        mWindowFactory->initialize();
        LOG_MESSAGE2(Logger::UI, "WindowFactory initialized.", "UiSubsystem::initializeUiSubsystem");
    }

    CEGUI::OgreRenderer* UiSubsystem::getGUIRenderer()
    {
        return mGuiRenderer;
    }

    bool UiSubsystem::onActiveCharacterChanged(Creature* oldActive, Creature* newActive)
    {
        // Ensure we have a sound listener
        if (SoundManager::getSingleton().getListenerActor() == NULL)
        {
            SoundManager::getSingleton().createListenerActor();
        }

        if (oldActive)
        {
            ScriptWrapper::getSingleton().disowned(oldActive);
            if (oldActive->getActor())
            {
                oldActive->getActor()->detach(SoundManager::getSingleton().getListenerActor());
            }
            oldActive->setQueryFlags(oldActive->getQueryFlags() & (~QUERYFLAG_PLAYER));
        }

        if (!newActive)
        {
            mInputManager->clearControlStates();
        }
        else
        {
            ScriptWrapper::getSingleton().owned(newActive);
            newActive->addQueryFlag(QUERYFLAG_PLAYER);

            mWindowFactory->setActiveCharacter(newActive);

            newActive->getActor()->attach(SoundManager::getSingleton().getListenerActor());
            LOG_MESSAGE(Logger::UI, "SoundListener attached.");

            // Reset control stack for the new Character and set to movement.
            mInputManager->setControlState(CST_MOVEMENT);
        }

        return true;
    }

    bool UiSubsystem::onBeforeClearScene()
    {
        // Remove control states here too, in case that there has not yet been a
        // person set active.
        mInputManager->clearControlStates();

        return true;
    }

    bool UiSubsystem::onGameObjectsLoaded()
    {
        if (mCharacterId != -1)
        {
            Creature* character = static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(mCharacterId));

            ScriptWrapper::getSingleton().owned(character);
            mCharacter = character;

            mWindowFactory->setActiveCharacter(character);

            mCharacter->getActor()->attach(SoundManager::getSingleton().getListenerActor());
            LOG_MESSAGE(Logger::UI, "SoundListener attached.");

            // Reset control stack for the new Character and set to movement.
            // mInputManager->setControlState(CST_MOVEMENT);
        }
        return false;
    }

    bool UiSubsystem::onBeforeGameObjectsLoaded()
    {
        LOG_MESSAGE(Logger::UI, "UiSubsystem::onBeforeGameObjectsLoaded()");
        if (mCharacter)
        {
            ScriptWrapper::getSingleton().disowned(mCharacter);
            mCharacter->getActor()->detach(SoundManager::getSingleton().getListenerActor());

            mWindowFactory->setActiveCharacter(NULL);

            mCharacterId = mCharacter->getId();
        }
        else
        {
            mCharacterId = -1;
        }

        // mInputManager->clearControlStates();
        return false;
    }

    bool UiSubsystem::onAllPlayerCharactersDied()
    {
        LOG_MESSAGE(Logger::UI, "All player chars are dead, show game over");
        WindowFactory::getSingleton().showGameOverWindow();
        return true;
    }
}
