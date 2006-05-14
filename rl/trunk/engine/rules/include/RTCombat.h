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
#ifndef __RTCombat_H__
#define __RTCombat_H__

#include "RulesPrerequisites.h"

#include <map>
#include <vector>

#include "GameTask.h"

namespace rl {

	class CombatLogger;
	class Creature;
	class RTCombatController;

	class _RlRulesExport RTCombat : public GameTask
	{
	public:
		RTCombat();

		void run(Ogre::Real elapsedTime);
		void start();

		void add(Creature* participant, int group);
		void addController(RTCombatController* controller);
		void setLogger(CombatLogger* logger);
		
		void attack(RTCombatController* controller, Creature* attacker, Creature* target);
		void paree(RTCombatController* controller, Creature* defender, Creature* target);

		void move(RTCombatController* controller, Creature* creature, Ogre::Vector3 movement);
		void turn(RTCombatController* controller, Creature* creature, Ogre::Degree angle);

	private:
		class CreatureData
		{
		public:
			CreatureData(Creature* creature, int group);
			Creature* creature;
			int group;

			int numParees;
			Time nextAttackTime;
			Creature* nextPareeTarget;
			bool convertAt;
			bool convertPa;
		};
		typedef std::map<Creature*, CreatureData*> CreatureDataMap;
		CreatureDataMap mCreatureData;

		/**
		 * Gibt alle Mitglieder einer Kampfpartei zurueck
		 * @param group die Partei
		 * @param eine Vector mit allen Mitgliedern der Partei
		 */
		std::vector<CreatureData*> getGroupMembers(int group);

		std::vector<RTCombatController*> mControllers;

		CreatureData* getData(Creature* participant);
		RTCombatController* getController(int group);
		void initKR();
		bool isOver();
		void release();

		void displayAtPa(Creature* attacker, Creature* defender);
		void displayAtHit(Creature* attacker, Creature* defender);

		bool isAttackPossible(CreatureData* attacker, CreatureData* defender);
		bool isPareeActivated(CreatureData* attacker, CreatureData* defender);

		const Time getCurrentTime() const;

		Ogre::Real mTimeKR;
		CombatLogger* mLogger;
	};
}

#endif // __RTCombat_H__
