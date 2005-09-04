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

namespace rl {

	class Creature;

	/**
	 * Verwaltungsklasse für einen Kampf
	 */
	class _RlRulesExport Combat
	{
	public:
		Combat();
		~Combat();

		/**
		 * Läßt eine Creature am Kampf teilnehmen
		 * 
		 * @param creature die Kreatur
		 * @param group die Partei, in der die Kreatur kämpft
		 */
		void add(Creature* creature, int group);

		/**
		 * Gibt alle Mitglieder einer Kampfpartei zurück
		 * @param group die Partei
		 * @param eine Vector mit allen Mitgliedern der Partei
		 */
		std::vector<Creature*> getGroupMembers(int group);

		/**
		 * Gibt die Partei zurück, der ein Wesen in diesem Kampf angehört
		 */
		int getGroupOf(Creature* creature);

	private:
		/**
		 * Speichert alle Daten, die eine Kreatur in diesem Kampf hat
		 */
		class Participant 
		{
		public:
			Participant(Creature* creature, int group);

			Creature* creature;
			int group;
			int initiative;

			static const int NO_INI = -9999999;
		};
		typedef std::vector<Participant*> CombatVector;


		void initialize(Participant* creature);

		int mCurrentInitiative;
		int mKampfrunde;
		CombatVector mParticipants;
	};

}

#endif //__Combat_H__