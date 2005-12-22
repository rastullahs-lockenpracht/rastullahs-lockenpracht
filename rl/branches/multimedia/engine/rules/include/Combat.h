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


	/**
	 * Verwaltungsklasse fuer einen Kampf
	 */
	class _RlRulesExport Combat : public EventSource, public GameTask
	{
	public:

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
		void setNextAction(Creature* creature, CombatAction* action);
		void setNextReaction(Creature* creature, CombatAction* action);

		Ogre::Real getMaxMoveDistance(MoveType action);
		void doAttacke(Creature* creature);
		bool isInAttackDistance(Creature* attacker, Creature* target);

		Creature* getNext();
		Creature* getNext(int group);
		bool isActionPhaseDone(Creature* actor);

		void addCombatEventListener(CombatEventListener* listener);
		void removeCombatEventListener(CombatEventListener* listener);


	private:
		static const int INI_START = 99999999;

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
			static const int NO_INI = -9999999;

			//Naechste Vorhaben
			Creature* attackeTarget;
			Creature* paradeTarget;

			CombatAction* nextAction;
			CombatAction* nextReaction;
		};
		typedef std::map<Creature*, Participant*> CombatMap;

		typedef std::pair<int, long> CombatTime; // INI, vergangene Zeit in dieser INI-Phase
		typedef std::map<CombatTime, Participant*> CombatEventList;
		CombatEventList mEventList;

		Participant* getParticipant(Creature* creature);

		void initialize(Participant* creature);
		void initializeKampfrunde();

		int mCurrentInitiative;
		int mKampfrunde;
		Ogre::Real mCurrentIniTime;

		CombatMap mParticipants;

		int mSlowMotionFactor;
		int mTimeOfAction;

		EventCaster<CombatEvent> mEventCaster;

		void run(Ogre::Real elapsedTime);
		CombatEventList::const_iterator findNextCombatEvent(const CombatEventList& eventList);
	};

}

#endif //__Combat_H__
