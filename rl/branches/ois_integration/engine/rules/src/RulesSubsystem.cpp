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

#include "RulesSubsystem.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "DsaDataLoader.h"
#include "Logger.h"
#include "TimerManager.h"
#include "ItemManager.h"
#include "ItemDataLoader.h"
#include "QuestBook.h"

template <>
rl::RulesSubsystem* Singleton<rl::RulesSubsystem> ::ms_Singleton = 0;
namespace rl
{
    RulesSubsystem& RulesSubsystem::getSingleton(void)
    {
        return Singleton<RulesSubsystem>::getSingleton();
    }

    RulesSubsystem* RulesSubsystem::getSingletonPtr(void)
    {
        return Singleton<RulesSubsystem>::getSingletonPtr();
    }

    RulesSubsystem::RulesSubsystem()
		: mQuestBook(NULL),
        mActionManager(NULL),
        mDsaManager(NULL),
        mTimerManager(NULL),
		mItemManager(NULL)
    {
		LOG_MESSAGE(Logger::RULES, "Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        //Singletons erzeugen
        mActionManager = new ActionManager(); 
		LOG_MESSAGE(Logger::RULES, "ActionManager erzeugt");
        mDsaManager = new DsaManager();
		LOG_MESSAGE(Logger::RULES, "DsaManager erzeugt");
		mTimerManager = new TimerManager(); 
		LOG_MESSAGE(Logger::RULES, "TimerManager erzeugt");
		resetQuestBook();
		LOG_MESSAGE(Logger::RULES, "Questverwaltung erzeugt");
		mItemManager = new ItemManager();
		LOG_MESSAGE(Logger::RULES, "ItemManager erzeugt");

		//Daten laden
		DsaDataLoader::loadData("basis.xdi");
		LOG_MESSAGE(Logger::RULES, "Basisdaten geladen");

		ItemDataLoader::loadData("items.xdi");
		
		LOG_MESSAGE(Logger::RULES, "Erzeugen abgeschlossen");		
    }
	
	RulesSubsystem::~RulesSubsystem()
    {
        delete mQuestBook;
        delete mTimerManager;
        delete mDsaManager;
        delete mActionManager;
        delete mItemManager;
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
