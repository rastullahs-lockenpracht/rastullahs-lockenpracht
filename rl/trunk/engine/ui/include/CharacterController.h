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

#ifndef __CharacterController_H__
#define __CharacterController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"
#include <OgreNewt.h>

namespace rl {

    class Actor;
    /**
     * This class handles character control via user input.
     */
	class _RlUiExport CharacterController : public GameTask
    {
    public:
        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        CharacterController(Actor* camera, Actor* character);
		virtual ~CharacterController()= 0 {};

		virtual void toggleViewMode() = 0;
		virtual void resetCamera() = 0;

	protected:
        Actor* mCamera;
        Actor* mCharacter;

        OgreNewt::Body* mCamBody;
        OgreNewt::Body* mCharBody;

    };
}
#endif
