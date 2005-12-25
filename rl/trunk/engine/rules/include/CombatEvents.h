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

#ifndef __CombatEvents_H__
#define __CombatEvents_H__

#include "RulesPrerequisites.h"
#include "EventObject.h"
#include "EventListener.h"

namespace rl {

	class _RlRulesExport Combat;
	class _RlRulesExport CombatEvent;
	class _RlRulesExport CombatEventListener;
	class _RlRulesExport AskForReactionEvent;
	class _RlRulesExport AskForActionEvent;
	class _RlRulesExport CombatFinishEvent;
	class _RlRulesExport Creature;

	/**
	 * Events im Kampf
	 */
	class _RlRulesExport CombatEvent : public virtual EventObject
	{
	public:

		enum CombatEventType {
			CEV_ASK_FOR_ACTION,
			CEV_ASK_FOR_REACTION,
			CEV_FINISH
		};


		CombatEvent(EventSource* combat);
		virtual ~CombatEvent();

		Combat* getCombat();
		virtual bool isGroupRelevant(int group) = 0;

		virtual CombatEventType getEventType() = 0;

	private:
		Combat* mCombat;
	};

	class _RlRulesExport AskForActionEvent : public CombatEvent
	{
	public:
		AskForActionEvent(
			Combat* combat, 
			Creature* actor,
			Ogre::Real timeLeftToAct, 
			Ogre::Real slowMotionFactor);
		~AskForActionEvent();

		Ogre::Real getTimeLeftToAct();
		Ogre::Real getSlowMotionFactor();
		Creature* getActor();

		virtual bool isGroupRelevant(int group);

		virtual CombatEventType getEventType();

	private:
		Ogre::Real mTimeLeftToAct;
		Ogre::Real mSlowMotionFactor;
		Creature* mActor;
	};

	class _RlRulesExport AskForReactionEvent : public AskForActionEvent
	{
	public:
		AskForReactionEvent(
			Combat* combat, 
			Creature* actor,
			Ogre::Real timeToAct, 
			Ogre::Real slowMotionFactor,
			Creature* opponent);

		Creature* getOpponent();
		virtual bool isGroupRelevant(int group);
		virtual CombatEventType getEventType();

	private:
		Creature* mOpponent;
	};

	class _RlRulesExport CombatFinishEvent : public CombatEvent
	{
	public:
		CombatFinishEvent(
			Combat* combat);
		virtual bool isGroupRelevant(int group);
		virtual CombatEventType getEventType();
	};

	class _RlRulesExport CombatEventListener : public EventListener<CombatEvent>
	{
	private:
		int mGroup;

	public:
		CombatEventListener(int group);

		int getGroup();

		bool eventRaised(CombatEvent *anEvent);

		virtual bool eventRaised(AskForReactionEvent* anEvent) = 0;
		virtual bool eventRaised(AskForActionEvent* anEvent) = 0;
		virtual bool eventRaised(CombatFinishEvent* anEvent) = 0;
	};
}

#endif //__Combat_H__
