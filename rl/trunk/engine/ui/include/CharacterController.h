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

#ifndef __CharacterController_H__
#define __CharacterController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"
#include <OgreNewt.h>

namespace rl {

    class Actor;
	class CommandMapper;
	class Creature;

    /**
     * This class handles character control via user input.
     */
	class _RlUiExport CharacterController : public GameTask
    {
	public:

		enum ControllerType 
		{
			CTRL_NONE = 1,
			CTRL_FREEFLIGHT,
			CTRL_MOVEMENT,
			CTRL_DIALOG,
			CTRL_CUTSCENE,
			CTRL_COMBAT,
		};

        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        CharacterController(Actor* camera, Actor* character);
		virtual ~CharacterController()= 0;

		void setCommandMapper(CommandMapper* mapper);

		virtual ControllerType getType() const = 0;

		virtual void toggleViewMode() = 0;
		virtual void resetCamera() = 0;

		virtual bool injectMouseDown(int mouseButtonMask) { return false; }
		virtual bool injectMouseUp(int mouseButtonMask) { return false; }
		virtual bool injectKeyDown(int keycode) { return false; }
		virtual bool injectKeyUp(int keycode) { return false; }	

        virtual const Ogre::String& getName() const;

	protected:
		static bool startAction(const CeGuiString& actionName, Creature* character = NULL);

        Actor* mCameraActor;
        Actor* mCharacterActor;

        OgreNewt::Body* mCamBody;
        OgreNewt::Body* mCharBody;

		CommandMapper* mCommandMapper;
    };
}
#endif
