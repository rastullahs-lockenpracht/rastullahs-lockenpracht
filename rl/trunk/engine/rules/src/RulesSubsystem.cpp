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

#include "RulesSubsystem.h"
#include "ActionManager.h"
#include "CombatManager.h"
#include "DsaManager.h"
//#include "XdimlLoader.h"
#include "DsaDataLoader.h"
#include "EffectFactory.h"
#include "Logger.h"
#include "CreatureControllerManager.h"
#include "TimerManager.h"
#include "GameObjectManager.h"
#include "QuestBook.h"

template <>
rl::RulesSubsystem* Singleton<rl::RulesSubsystem>::ms_Singleton = 0;
namespace rl
{
    RulesSubsystem::RulesSubsystem()
		: mQuestBook(NULL),
        mActionManager(NULL),
        mCombatManager(NULL),
        mDsaManager(NULL),
        mMovingCreatureManager(NULL),
        mTimerManager(NULL),
        mXdimlLoader(NULL)
    {
		LOG_MESSAGE(Logger::RULES, "Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        //Singletons erzeugen
        mActionManager = new ActionManager(); 
		LOG_MESSAGE(Logger::RULES, "ActionManager erzeugt");
        mCombatManager = new CombatManager(); 
		LOG_MESSAGE(Logger::RULES, "CombatManager erzeugt");
        mDsaManager = new DsaManager();
		LOG_MESSAGE(Logger::RULES, "DsaManager erzeugt");
        mMovingCreatureManager = new CreatureControllerManager();
		LOG_MESSAGE(Logger::RULES, "CreatureControllerManager erzeugt");
        mTimerManager = new TimerManager(); 
		LOG_MESSAGE(Logger::RULES, "TimerManager erzeugt");
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
        delete mQuestBook;
        delete mTimerManager;
        delete mDsaManager;
        delete mMovingCreatureManager;
        delete mCombatManager;
        delete mActionManager;
        delete mXdimlLoader;
        delete GameObjectManager::getSingletonPtr();
        delete EffectFactoryManager::getSingletonPtr();
    }

	QuestBook* RulesSubsystem::getQuestBook()
	{
		return mQuestBook;
	}

	void RulesSubsystem::resetQuestBook()
	{
		delete mQuestBook;
		mQuestBook = new QuestBook();
	}
}
