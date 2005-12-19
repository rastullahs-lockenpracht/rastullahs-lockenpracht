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

#ifndef __QUEST_H__
#define __QUEST_H__

#include "RulesPrerequisites.h"
#include <vector>

namespace rl {

class Quest;
typedef std::vector<Quest*> QuestVector;

class _RlRulesExport Quest
{
public:
	enum State
	{
		UNKNOWN = 0,
		OPEN,
		FAILED,
		DONE
	};

	Quest(const CeGuiString& id, const CeGuiString& name, const CeGuiString& description);
	~Quest();

	const CeGuiString& getId();
	const CeGuiString& getName();
	const CeGuiString& getDescription();
	int getPartsToDo();
	void setPartsToDo(int partsToDo);
	int getPartsDone();
	void setPartsDone(int partsDone);
	Quest::State getState();
	void setState(Quest::State state);

	QuestVector getSubquests();
	void addSubquest(Quest* quest);
	

private:
	QuestVector mSubquests;
	CeGuiString mId;
	CeGuiString mName;
	CeGuiString mDescription;
	int mPartsToDo;
	int mPartsDone;
	State mState;
};

}
#endif
