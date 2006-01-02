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
#ifndef __QuestStateChangeEvent_H__
#define __QuestStateChangeEvent_H__

#include "RulesPrerequisites.h"

#include "EventObject.h"

namespace rl {

class _RlRulesExport Quest;
class _RlRulesExport QuestBook;

class _RlRulesExport QuestStateChangeEvent : public virtual EventObject 
{
public:
	QuestStateChangeEvent( QuestBook* src );
    virtual ~QuestStateChangeEvent() {};

	void setQuest(Quest* quest);
	Quest* getQuest();

private:
	Quest* mQuest;
};

}

#endif // __QuestStateChangeEvent_H__
