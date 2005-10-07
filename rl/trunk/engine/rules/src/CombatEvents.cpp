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

#include "CombatEvents.h"
#include "Combat.h"

using namespace std;

namespace rl {
	CombatEvent::CombatEvent(EventSource* combat) 
		: EventObject(combat)
	{ 
	}
	
	CombatEvent::~CombatEvent() 
	{ 
	}
	
	Combat* CombatEvent::getCombat() 
	{
		return static_cast<Combat*>(getSource());
	}


	AskForActionEvent::AskForActionEvent(
		Combat* combat, 
		Creature* actor,
		Ogre::Real timeToAct, 
		Ogre::Real slowMotionFactor) 
			: CombatEvent(combat),
			EventObject(combat),
			mTimeLeftToAct(timeToAct),
			mSlowMotionFactor(slowMotionFactor),
			mActor(actor)
	{ 
		
	}

	AskForActionEvent::~AskForActionEvent() 
	{ 
	}

	Ogre::Real AskForActionEvent::getTimeLeftToAct() 
	{
		return mTimeLeftToAct;
	}

	Ogre::Real AskForActionEvent::getSlowMotionFactor() 
	{
		return mSlowMotionFactor;
	}

	CombatEventType AskForActionEvent::getEventType() 
	{
		return CEV_ASK_FOR_ACTION;
	}

	bool AskForActionEvent::isGroupRelevant(int group)
	{
		return getCombat()->getGroupOf(mActor) == group;
	}

	Creature* AskForActionEvent::getActor()
	{
		return mActor;
	}

	AskForReactionEvent::AskForReactionEvent(
		Combat* combat,
		Creature* actor,
		Ogre::Real timeToAct, 
		Ogre::Real slowMotionFactor,
		Creature* opponent) 
		: AskForActionEvent(combat, actor, timeToAct, slowMotionFactor),
		EventObject(combat),
		mOpponent(opponent)
	{ 
	}

	Creature* AskForReactionEvent::getOpponent()
	{
		return mOpponent;
	}

	CombatEventType AskForReactionEvent::getEventType() 
	{
		return CEV_ASK_FOR_REACTION;
	}

	bool AskForReactionEvent::isGroupRelevant(int group)
	{
		return getCombat()->getGroupOf(mOpponent) == group;
	}

	CombatFinishEvent::CombatFinishEvent(Combat* combat)
		: CombatEvent(combat),
		EventObject(combat)
	{

	}

	CombatEventType CombatFinishEvent::getEventType()
	{
		return CEV_FINISH;
	}

	bool CombatFinishEvent::isGroupRelevant(int group)
	{
		return true;
	}

	CombatEventListener::CombatEventListener(int group)
		: mGroup(group),
		EventListener<CombatEvent>()
	{
	}
	
	int CombatEventListener::getGroup()
	{
		return mGroup;
	}

	bool CombatEventListener::eventRaised(CombatEvent *anEvent) 
	{
		if (anEvent->isGroupRelevant(getGroup()))
		{
			switch (anEvent->getEventType())
			{
			case CEV_ASK_FOR_REACTION:
				return eventRaised(static_cast<AskForReactionEvent*>(anEvent));
			case CEV_ASK_FOR_ACTION:
				return eventRaised(static_cast<AskForActionEvent*>(anEvent));
			case CEV_FINISH:
				return eventRaised(static_cast<CombatFinishEvent*>(anEvent));
			}
		}

		return false;
	}


}