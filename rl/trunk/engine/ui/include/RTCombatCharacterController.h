/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#ifndef __RTCombatCharacterController_H__
#define __RTCombatCharacterController_H__

#include "UiPrerequisites.h"

#include "MovementCharacterController.h"
#include "RTCombatController.h"

namespace rl {

	class Creature;

	class _RlUiExport RTCombatCharacterController
		: public MovementCharacterController, public RTCombatController
	{
	public:
		RTCombatCharacterController(Actor* camera, Creature* character);
		virtual ~RTCombatCharacterController();
		virtual ControllerType getType() const;

		virtual void notifyActionEnabled(bool enabled);
		virtual void notifyReactionEnabled(bool enabled);
		virtual void notifyCombatEnd();

		virtual void run(Ogre::Real elapsedTime);

		virtual bool injectMouseClicked(int mouseButtonMask);
		virtual bool injectKeyClicked(int keycode);		
		virtual bool injectKeyDown(int keycode);
		virtual bool injectKeyUp(int keycode);	

		void attack();
		void block();

	private:
		Creature* mLastAttackTarget;
		Creature* mLastPareeTarget;
	};

} // namespace rl

#endif // __RTCombatCharacterController_H__
