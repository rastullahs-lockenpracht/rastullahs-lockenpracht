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
#ifndef __QuestEvent_H__
#define __QuestEvent_H__

#include "RulesPrerequisites.h"

#include "EventObject.h"

namespace rl
{

    class _RlRulesExport Quest;
    class _RlRulesExport QuestBook;

    class _RlRulesExport QuestEvent : public virtual EventObject
    {
    public:
        static const unsigned int QUEST_PARTSDONE = 360;
        static const unsigned int QUEST_SUBQUEST = 361;
        static const unsigned int QUEST_STATE = 362;
        static const unsigned int QUEST_KNOWN = 363;

        QuestEvent(QuestBook* src, int reason);
        virtual ~QuestEvent(){};

        void setQuest(Quest* quest);
        Quest* getQuest();

    private:
        Quest* mQuest;
    };
}

#endif // __QuestEvent_H__
