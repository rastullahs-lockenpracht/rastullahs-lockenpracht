/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "DsaManager.h"
#include "DsaDataLoader.h"
#include "EffectFactory.h"
#include "GameEventLog.h"
#include "Logger.h"
#include "CreatureControllerManager.h"
#include "GameObjectManager.h"
#include "QuestBook.h"
#include "GameTimeSource.h"
#include <ConfigurationManager.h>

template <>
rl::RulesSubsystem* Singleton<rl::RulesSubsystem>::ms_Singleton = 0;
namespace rl
{
    RulesSubsystem::RulesSubsystem()
		: mQuestBook(NULL),
        mActionManager(NULL),
        mCombatManager(NULL),
        mDsaManager(NULL),
		mGameEventLog(NULL),
        mMovingCreatureManager(NULL),
        mXdimlLoader(NULL)
    {
		LOG_MESSAGE(Logger::RULES, "Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        TimeSourceManager::getSingleton().registerTimeSource(
            new GameTimeSource());

        //Singletons erzeugen
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

		//Daten laden
		mXdimlLoader = new XdimlLoader();
		LOG_MESSAGE(Logger::RULES, "Basisdaten geladen");

        new EffectFactoryManager();
        new GameObjectManager();
		LOG_MESSAGE(Logger::RULES, "Erzeugen abgeschlossen");
    }

	RulesSubsystem::~RulesSubsystem()
    {
        if(mQuestBook)
            Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mQuestBook);
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
        if(mQuestBook)
            Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mQuestBook);
		delete mQuestBook;
		mQuestBook = new QuestBook();
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(mQuestBook);
	}
}
