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

#include "RulesSubsystem.h"

#include "ActionManager.h"
#include "CombatManager.h"
#include "ConfigurationManager.h"
#include "CreatureControllerManager.h"
#include "DsaDataLoader.h"
#include "DsaManager.h"
#include "EffectFactory.h"
#include "EffectManagementTask.h"
#include "GameEventLog.h"
#include "GameLoop.h"
#include "GameObjectManager.h"
#include "GameTimeSource.h"
#include "GlobalProperties.h"
#include "Logger.h"
#include "PartyManager.h"
#include "QuestBook.h"

template <> rl::RulesSubsystem* Singleton<rl::RulesSubsystem>::msSingleton = 0;

namespace rl
{
    RulesSubsystem::RulesSubsystem()
        : mQuestBook(NULL)
        , mActionManager(NULL)
        , mCombatManager(NULL)
        , mDsaManager(NULL)
        , mGameEventLog(NULL)
        , mMovingCreatureManager(NULL)
        , mXdimlLoader(NULL)
        , mGlobalProperties(NULL)
        , mEffectManagementTask(NULL)
    {
        LOG_MESSAGE(Logger::RULES, "Start");
        // Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        TimeSourceManager::getSingleton().registerTimeSource(new GameTimeSource());

        // Singletons erzeugen
        mActionManager = new ActionManager();
        LOG_MESSAGE(Logger::RULES, "ActionManager erzeugt");
        mCombatManager = new CombatManager();
        LOG_MESSAGE(Logger::RULES, "CombatManager erzeugt");
        mDsaManager = new DsaManager();
        LOG_MESSAGE(Logger::RULES, "DsaManager erzeugt");
        mGameEventLog = new GameEventLog();
        LOG_MESSAGE(Logger::RULES, "GameEventLog erzeugt");
        mMovingCreatureManager = new CreatureControllerManager();
        LOG_MESSAGE(Logger::RULES, "CreatureControllerManager erzeugt");
        resetQuestBook();
        LOG_MESSAGE(Logger::RULES, "Questverwaltung erzeugt");
        mGlobalProperties = new GlobalProperties();

        mEffectManagementTask = new EffectManagementTask();
        GameLoop::getSingleton().addTask(mEffectManagementTask, GameLoop::TG_LOGIC);

        // Daten laden
        mXdimlLoader = new XdimlLoader();
        LOG_MESSAGE(Logger::RULES, "Basisdaten geladen");

        new EffectFactoryManager();
        new GameObjectManager();

        mPartyManager = new PartyManager();
        LOG_MESSAGE(Logger::RULES, "Erzeugen abgeschlossen");
    }

    RulesSubsystem::~RulesSubsystem()
    {
        delete mPartyManager;
        GameLoop::getSingleton().removeTask(mEffectManagementTask);
        delete mEffectManagementTask;
        delete mGlobalProperties;
        if (mQuestBook)
        {
            Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mQuestBook);
        }
        delete mQuestBook;
        delete mGameEventLog;
        delete mDsaManager;
        delete mCombatManager;
        delete mActionManager;
        delete mXdimlLoader;
        delete GameObjectManager::getSingletonPtr();
        delete mMovingCreatureManager; // this manager should be deleted after the GameObjectManager
        delete EffectFactoryManager::getSingletonPtr();
    }

    QuestBook* RulesSubsystem::getQuestBook()
    {
        return mQuestBook;
    }

    void RulesSubsystem::resetQuestBook()
    {
        if (mQuestBook)
        {
            Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mQuestBook);
        }
        delete mQuestBook;
        mQuestBook = new QuestBook();
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(mQuestBook);
    }
}
