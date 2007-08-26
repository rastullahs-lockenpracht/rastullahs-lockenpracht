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

#ifndef __FreeFlightCharacterController_H__
#define __FreeFlightCharacterController_H__

#include "UiPrerequisites.h"
#include "ControlState.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include <OgreNewt.h>

namespace rl {

	/**
	* This class provides a no-clip free fly controller.
	*/
	class _RlUiExport FreeflightControlState : 
        public ControlState,
        public PhysicsController,
        public PhysicsGenericContactCallback
	{
	public:
		/**
		*  @throw NullPointerException if camera or character is NULL.
		*  @throw InvalidArgumentException if character is not placed in the scene.
		*/
		FreeflightControlState(CommandMapper* cmdMapper, Actor* camera, Person* character);
		virtual ~FreeflightControlState();

        virtual void pause();
        virtual void resume();

		virtual void run(Ogre::Real elapsedTime);

        virtual bool keyPressed(const OIS::KeyEvent& evt);
        virtual bool keyReleased(const OIS::KeyEvent& evt);

	    void toggleCameraCollision();
	    void resetCamera();

        /// This is the OgreNewt contact process callback for the combination
        /// Character <-> Level
        int userProcess();

        /// Newton force and torque callback
        void OnApplyForceAndTorque(PhysicalThing* thing);

	private:		
		int mCurrentMovementState;

		Ogre::Real mMovementSpeed;
		std::pair<Ogre::Real, Ogre::Real> mSpeedRange;
		Ogre::Real mSpeedIncrement;
		Ogre::Real mRotationSpeed;
        Ogre::Vector3 mDesiredVelocity;
        Ogre::Radian mYaw;
        Ogre::Radian mPitch;
        bool mCollisionsEnabled;
        Ogre::Real mMouseSensitivity;
        bool mInvertedMouse;
        std::pair<Ogre::Degree, Ogre::Degree> mPitchRange;
	};
}
#endif
