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

#ifndef __MovementCharacterController_H__
#define __MovementCharacterController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include "CharacterController.h"
#include "DebugVisualisable.h"
#include "Selector.h"

#include <deque>

namespace rl {

    class Actor;
	class Creature;
    class MeshObject;
	class PhysicsMaterialRaycast;

    /**
     * This class handles character control via user input.
     */
    class _RlUiExport MovementCharacterController
	  : public CharacterController,
		public PhysicsController,
        public PhysicsGenericContactCallback,
        public DebugVisualisable
    {
    public:
        typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON, VM_FREE_CAMERA} ViewMode;
        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        MovementCharacterController(Actor* camera, Creature* character);
        virtual ~MovementCharacterController();
		virtual ControllerType getType() const;

        void run(Ogre::Real elapsedTime);

        /// This is the OgreNewt contact process callback for the combination
        /// Character <-> Level
        int userProcess();

        /// Newton force and torque callback
        void OnApplyForceAndTorque(PhysicalThing* thing);

        /// First oder Third person view.
        void setViewMode(ViewMode mode);
        ViewMode getViewMode();
        void toggleViewMode();

        /** Setzt die Camera in einen 30-Grad-Winkel dem Helden auf den Hinterkopf
        *  schauend im aktuellen Abstand vom Helden, wie durch den Spieler bestimmt.
        */
        void resetCamera();

		virtual bool injectMouseDown(int mouseButtonMask);
		virtual bool injectMouseUp(int mouseButtonMask);
		virtual bool injectKeyDown(int keycode);
		virtual bool injectKeyUp(int keycode);		

        // Overrides from DebugVisualisable
        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();

    protected:
        virtual void doCreatePrimitive();

    private:

		/// private struct for holding state info of the controller
		struct CharacterState
		{
            typedef enum {Stand, Crouch} Pose;
			CharacterState();
            Pose mPose;
			bool mIsAirBorne;
            bool mHasFloorContact;
			bool mStartJump;

			Ogre::Real mJumpTimer;

			Ogre::Vector3 mDesiredVel;

			int mCurrentMovementState;
			int mLastMovementState;
		};

        static Ogre::String msDebugWindowPageName;

		CharacterState mCharacterState;
		Creature* mCharacter;

        // camera control params
        /// optimal distance to the character
        Ogre::Real mDesiredDistance;
        std::pair<Ogre::Real, Ogre::Real> mDistanceRange;
        Ogre::Degree mYaw;
        Ogre::Degree mCamYaw; // f�r VM_FREE_CAMERA
        Ogre::Degree mPitch;
        std::pair<Ogre::Degree, Ogre::Degree> mPitchRange;

        Ogre::Vector3 mLookAtOffset;
        Ogre::Real mMovementSpeed;
        Ogre::Real mRotationSpeed;
        Ogre::Real mSpeedModifier;

		Ogre::Vector3 mGravitation;

        ViewMode mViewMode;

        int mObstractedFrameCount;
        Ogre::Real mObstractedTime;
        int mCameraJammedFrameCount;
        Ogre::Real mCameraJammedTime;

        PhysicsMaterialRaycast* mRaycast;
        HalfSphereSingleSelector mSelector;

        /// the maximum amount of time, the character or camera should need,
        /// in order to reach the position, desired by the user.
        /// the smaller, the tighter the feel.
        //Ogre::Real mMaxDelay;


        /// Explained where these are set
        Ogre::Real mLinearSpringK;
        Ogre::Real mLinearDampingK;
        Ogre::Real mRotLinearSpringK;
        Ogre::Real mRotLinearDampingK;        

		bool isRunMovement(int movement);

        void updateSelection();
        void updateAnimationState();

        /** Does all camera-stuff, moves the camera to the right position 
        * and does pathfinding (in a very simple way)
        * @warning this does only work well, if the character's material is not used for other objects!
        */
		void calculateCamera(const Ogre::Real& timestep);

        Ogre::Vector3 calculateOptimalCameraPosition(void);
    };
}
#endif
