/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CombatCharacterController_H__
#define __CombatCharacterController_H__

#include "UiPrerequisites.h"
#include "CharacterController.h"

namespace rl {

	class _RlUiExport CombatCharacterController : public CharacterController
	{
	public:
		/**
		*  @throw NullPointerException if camera is NULL.
		*/
		CombatCharacterController(Actor* camera, Actor* character);
		virtual ~CombatCharacterController();

		virtual ControllerType getType() const;

		void run(Ogre::Real elapsedTime);

		void toggleViewMode();
        void resetCamera();

        virtual bool injectKeyUp(int keycode);
    private:
	};
}
#endif