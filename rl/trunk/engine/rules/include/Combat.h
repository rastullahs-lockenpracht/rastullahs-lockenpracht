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

#include <map>
#include <vector>

namespace rl {

	class Creature;
	class CombatController;

	/**
	 * Verwaltungsklasse fuer einen Kampf
	 */
	class _RlRulesExport Combat
	{
	public:
		enum ActionOption {
			ACTION_ATTACK = 1,
			ACTION_MOVE = 2,
			ACTION_NONE = 3
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


		void addController(CombatController* controller);

		void start();
		void tick();

		void setActionOption(CombatController* controller, Creature* actor, ActionOption option);
		void setAttackTarget(CombatController* controller, Creature* actor, Creature* target);
		void setPareeTarget(CombatController* controller, Creature* actor, Creature* target);

	private:
		class CreatureData
		{
		public:
			CreatureData(Creature* creature, int group);
			Creature* creature;
			int group;

			int initiative;
			ActionOption nextAction;
			Creature* nextAttackTarget;
			Creature* nextPareeTarget;
		};
		typedef std::map<Creature*, CreatureData*> CreatureDataMap;

		void initialize(CreatureData* part);
		CreatureData* getData(Creature* creature);
		CreatureData* getNextActor();

		CreatureDataMap mCreatureData;
		std::vector<CombatController*> mControllers;

		static const int NO_INI = -99999;
		int mInitiative;
	};
}

#endif //__Combat_H__
