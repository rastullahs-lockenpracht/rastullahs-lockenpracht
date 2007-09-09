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

#ifndef __RULESSUBSYSTEM_H__
#define __RULESSUBSYSTEM_H__

#include "RulesPrerequisites.h"

#include <string>

namespace rl
{
	class QuestBook;
    class ActionManager;
    class CombatManager;
    class DsaManager;
    class CreatureControllerManager;
    class SaveGameManager;
    class XdimlLoader;

	class _RlRulesExport RulesSubsystem : public Ogre::Singleton<RulesSubsystem>
    {
    public:
        RulesSubsystem();
        ~RulesSubsystem();
	
		QuestBook* getQuestBook();
		void resetQuestBook();

	private:
		QuestBook* mQuestBook;
        ActionManager* mActionManager;
        CombatManager* mCombatManager;
        DsaManager* mDsaManager;
        CreatureControllerManager *mMovingCreatureManager;
        SaveGameManager* mSaveGameManager;
        XdimlLoader* mXdimlLoader;
    };
}

#endif
