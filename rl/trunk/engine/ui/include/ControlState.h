/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CharacterController_H__
#define __CharacterController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"

#include <OgreNewt.h>

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <SaveGameManager.h>

namespace rl {

    class Actor;
	class CommandMapper;
	class Creature;
    class Person;

    /**
     * This class handles character control via user input.
     */
    class _RlUiExport ControlState// : public OIS::KeyListener, public OIS::MouseListener
    {
	public:
        static const Ogre::String PROPERTY_CHARACTERID;

        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        ControlState(CommandMapper* commandMapper, Actor* camera, Person* character,
            ControlStateType type);
		virtual ~ControlState()= 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void run(Ogre::Real elapsedTime) = 0;

        virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled); // if handled is true, we should not handle the event!
        virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled);
        virtual bool mouseMoved(const OIS::MouseEvent& evt, bool handled);
        virtual bool keyPressed(const OIS::KeyEvent& evt, bool handled);
        virtual bool keyReleased(const OIS::KeyEvent& evt, bool handled);

        const CommandMapper* getCommandMapper() const {return mCommandMapper;}
        ControlStateType getType() const { return mType;}

        static bool startAction(const CeGuiString& actionName, Creature* character = NULL);
        
        bool refetchCharacter();
	protected:

        /// returns wether cegui currently uses the mouse
        bool isMouseUsedByCegui() const;

        Person* mCharacter;
        int mCharacterId;

        Actor* mCameraActor;
        Actor* mCharacterActor;

        OgreNewt::Body* mCamBody;
        OgreNewt::Body* mCharBody;

		CommandMapper* mCommandMapper;

        ControlStateType mType;

    private:
        MessagePump::ScopedConnection mMessageType_GameObjectsLoaded_Handler;
    };
}
#endif
