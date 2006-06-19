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
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        //Singletons erzeugen
        mActionManager = new ActionManager(); 
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "ActionManager erzeugt");
        mDsaManager = new DsaManager();
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "DsaManager erzeugt");
		mTimerManager = new TimerManager(); 
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "TimerManager erzeugt");
		resetQuestBook();
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "Questverwaltung erzeugt");
		mItemManager = new ItemManager();
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "ItemManager erzeugt");

		//Daten laden
		DsaDataLoader::loadData("basis.xdi");
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "Basisdaten geladen");

		ItemDataLoader::loadData("items.xdi");
		
		Logger::getSingleton().log(Logger::RULES, Logger::LL_MESSAGE, "Erzeugen abgeschlossen");		
    }
	
	RulesSubsystem::~RulesSubsystem()
    {
        delete mQuestBook;
        delete mTimerManager;
        delete mDsaManager;
        delete mActionManager;
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
