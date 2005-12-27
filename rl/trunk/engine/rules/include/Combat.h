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

#ifndef __Combat_H__
#define __Combat_H__

#include "RulesPrerequisites.h"

#include <stack>
#include <vector>

#include "EventSource.h"
#include "EventCaster.h"
#include "CombatEvents.h"
#include "GameTask.h"

namespace rl {
	enum MoveType {
		MT_NO_WALK,
		MT_WALK,
		MT_RUN,
		MT_WALK_IN_PA_PHASE,
		MT_WALK_IN_AT_PHASE,
		MT_WALK_IN_AT_PA_PHASE,
		MT_RUN_IN_AT_PA_PHASE
	};

	class Creature;
	class CombatAction;
	class CombatActionAttack;
	class CombatActionMove;
	class CombatActionParee;

	class CombatTest;

	/**
	 * Verwaltungsklasse fuer einen Kampf
	 */
	class _RlRulesExport Combat : public EventSource, public GameTask
	{
		friend class CombatTest;

	public:
		static const int ACTION_TIME_OVER = 0;

		Combat(int slowMotionFactor = 1.0);
		~Combat();

		/**
		 * Laesst eine Creature am Kampf teilnehmen
		 * 
		 * @param creature die Kreatur
		 * @param group die Partei, in der die Kreatur kaempft
		 */
		void add(Creature* creature, int group);

		/**
		 * Gibt alle Mitglieder einer Kampfpartei zurueck
		 * @param group die Partei
		 * @param eine Vector mit allen Mitgliedern der Partei
		 */
		std::vector<Creature*> getGroupMembers(int group);

		/**
		 * Gibt die Partei zurueck, der ein Wesen in diesem Kampf angehoert
		 */
		int getGroupOf(Creature* creature);

		Creature* getAttackeTarget(Creature* creature);
		void setAttackeTarget(Creature* creature, Creature* target);
		Creature* getParadeTarget(Creature* creature);
		void setParadeTarget(Creature* creature, Creature* target);
		void setNextAction(Creature* creature, CombatAction* action, int eventId = -1);
		void setNextReaction(Creature* creature, CombatAction* action, int eventId = -1);

		Ogre::Real getMaxMoveDistance(MoveType action);
		void doAttacke(Creature* creature);
		bool isInAttackDistance(Creature* attacker, Creature* target);

		Creature* getNext();
		Creature* getNext(int group);
		bool isActionPhaseDone(Creature* actor);

		void addCombatEventListener(CombatEventListener* listener);
		void removeCombatEventListener(CombatEventListener* listener);

		void run(Ogre::Real elapsedTime);

		void start();

	private:
		static const int INI_START = 99999999;
		static const int NO_INI = -9999999;

		/**
		 * Speichert alle Daten, die eine Kreatur in diesem Kampf hat
		 */
		class Participant 
		{
		public:
			Participant(Creature* creature, int group, int no);

			//Wesen und Gruppe
			Creature* creature;
			int group;
			int id;

			//DSA-Daten
			int initiative;

			int pareesLeft;
			
			//Naechste Vorhaben
			Creature* attackeTarget;
			Creature* paradeTarget;

			CombatAction* nextAction;
			CombatAction* nextReaction;

			CombatAction* nextKRAction;
			CombatAction* nextKRReaction;
		};
		typedef std::map<Creature*, Participant*> CombatMap;

		class CombatEvt
		{
		public:
			CombatEvt(int ini, int iniIdx, bool isAction, Participant* part, CombatAction* action);

			int ini; 
			int iniIdx; 
			int id;
			bool isAction; 
			Participant* part;
			CombatAction* action;

			static int sId;
		};

		typedef std::pair<int, long> CombatTime; // INI, vergangene Zeit in dieser INI-Phase
		typedef std::map<CombatTime, CombatEvt*> CombatEventList;
		CombatEventList mEventList;

		Participant* getParticipant(Creature* creature);

		void initialize(Participant* creature);
		void initializeKampfrunde();

		int mCurrentInitiative;
		int mKampfrunde;
		
		Ogre::Real mCurrentIniTime;
		Ogre::Real mCurrentEventStart;
		Ogre::Real mTimeInKampfrunde;
		std::vector<CombatEvt*> mEventStack;

		CombatMap mParticipants;

		int mSlowMotionFactor;
		bool mIsSlowMotion;
		Ogre::Real mTimeOfAction;

		bool mStarted;

		std::map<int, EventCaster<CombatEvent> > mEventCasters;

		CombatEventList::const_iterator findNextCombatEvent(const CombatEventList& eventList);

		bool processEvent(CombatEvt* evt);
		void sendStartEvent(CombatEvt* evt);
		void sendStopEvent(CombatEvt* evt);
		bool processAttack(Participant* part, CombatActionAttack* attack);
		bool processMove(Participant* part, CombatActionMove* move);
		bool processParee(Participant* part, CombatActionParee* paree, CombatActionAttack* attack);

		CombatEvt* getLastEvent();
		CombatEvt* getSecondLastEvent();

		bool canDefend(Participant* defender, Participant* attacker);
		void executeAttacke(Participant* attacker, Participant* defender);

		int getAttackeModifier(Participant* attacker);
		int getParadeModifier(Participant* defender);

		void popEventStack();
	};

}

#endif //__Combat_H__
