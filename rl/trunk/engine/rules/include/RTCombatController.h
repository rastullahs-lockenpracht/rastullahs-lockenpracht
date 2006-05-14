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
#ifndef __RTCombatController_H__
#define __RTCombatController_H__


#include "RulesPrerequisites.h"

#include "RTCombat.h"

namespace rl {

	class Creature;	

	class _RlRulesExport RTCombatController 
	{
	public:
		RTCombatController();
		virtual ~RTCombatController();

		int getGroup();
		void setGroup(int group);

		Creature* getCurrentCreature();
		void setCurrentCreature(Creature* creature);
		RTCombat* getCombat();
		void setCombat(RTCombat* combat);

		virtual void notifyActionEnabled(bool enabled) = 0;
		virtual void notifyReactionEnabled(bool enabled) = 0;
		virtual void notifyCombatEnd() = 0;

		virtual void attack(Creature* target);
		virtual void block(Creature* target);

		void move(Ogre::Vector3 movement);
		void turn(Ogre::Degree angle);

	private:
		bool isInAtRange(Creature* target);
		Creature* findAtTarget();

		int mGroup;
		Creature* mCurrentCreature;
		RTCombat* mCombat;
	};

}

#endif //__RTCombatController_H__
