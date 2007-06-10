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

#include <OISMouse.h>
#include <OISKeyboard.h>

namespace rl {

    class Actor;
	class CommandMapper;
	class Creature;
    class Person;

    /**
     * This class handles character control via user input.
     */
    class _RlUiExport CharacterController : public OIS::KeyListener, public OIS::MouseListener
    {
	public:

        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        CharacterController(CommandMapper* commandMapper, Actor* camera, Person* character,
            ControlStateType type);
		virtual ~CharacterController()= 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void run(Ogre::Real elapsedTime) = 0;

        virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
        virtual bool mouseMoved(const OIS::MouseEvent& evt);
        virtual bool keyPressed(const OIS::KeyEvent& evt);
        virtual bool keyReleased(const OIS::KeyEvent& evt);

	protected:
		static bool startAction(const CeGuiString& actionName, Creature* character = NULL);

        /// Returns true, if there is at least one window open,
        /// that requires keyboard and/or mouse input. 
        bool isCeguiActive() const;

        /// Returns true, if the key event should be injected into CEGUI
        /// This is the case, if an open window requests key input and it is
        /// an input or navigation key.
        bool sendKeyToCeGui(const OIS::KeyEvent& evt) const;

        Person* mCharacter;

        Actor* mCameraActor;
        Actor* mCharacterActor;

        OgreNewt::Body* mCamBody;
        OgreNewt::Body* mCharBody;

		CommandMapper* mCommandMapper;

        ControlStateType mType;
    };
}
#endif
