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

#ifndef __RULESSUBSYSTEM_H__
#define __RULESSUBSYSTEM_H__

#include "RulesPrerequisites.h"

#include <string>

namespace rl
{
	class QuestBook;
    class ActionManager;
    class DsaManager;
    class TimerManager;
	class ItemManager;

	class _RlRulesExport RulesSubsystem : public Ogre::Singleton<RulesSubsystem>
    {
    public:
        RulesSubsystem();
        ~RulesSubsystem();
	
        static RulesSubsystem& getSingleton(void);
        static RulesSubsystem* getSingletonPtr(void);

		QuestBook* getQuestBook();
		void resetQuestBook();

	private:
		QuestBook* mQuestBook;
        ActionManager* mActionManager;
        DsaManager* mDsaManager;
        TimerManager* mTimerManager;
		ItemManager* mItemManager;
    };
}

#endif
