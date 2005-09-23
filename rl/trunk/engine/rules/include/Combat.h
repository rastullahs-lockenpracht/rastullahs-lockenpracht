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

	class Creature;

	/**
	 * Verwaltungsklasse fuer einen Kampf
	 */
	class _RlRulesExport Combat : public EventSource, public GameTask
	{
	public:
		enum MoveType {
			MT_NO_WALK,
			MT_WALK_IN_PA_PHASE,
			MT_WALK_IN_AT_PHASE,
			MT_WALK_IN_AT_PA_PHASE,
			MT_RUN_IN_AT_PA_PHASE
		};
		enum ActionTypeAktion {
			AT_ATTACKE,
			AT_PARADE,
			AT_WALK,
			AT_RUN,
		};

		Combat();
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

		Ogre::Real getMaxMoveDistance(MoveType action);
		void doAttacke(Creature* creature);

		Creature* getNext();
		Creature* getNext(int group);

		void addCombatEventListener(CombatEventListener* listener);
		void removeCombatEventListener(CombatEventListener* listener);


	private:
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

			ActionTypeAktion nextAttackeAction;
			ActionTypeAktion nextParadeAction;
		};
		typedef std::map<Creature*, Participant*> CombatMap;

		Participant* getParticipant(Creature* creature);


		void initialize(Participant* creature);

		int mCurrentInitiative;
		int mKampfrunde;
		CombatMap mParticipants;

		EventCaster<CombatEvent> mEventCaster;

		void run(Ogre::Real elapsedTime);
	};

}

#endif //__Combat_H__
