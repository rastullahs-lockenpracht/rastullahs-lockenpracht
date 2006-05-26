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
#include "CharacterController.h"

#undef sleep

#include <vector>

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
        public PhysicsController
    {
    public:
        typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON} ViewMode;
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

        virtual bool injectMouseClicked(int mouseButtonMask);
        virtual bool injectMouseDown(int mouseButtonMask);
        virtual bool injectMouseUp(int mouseButtonMask);
        virtual bool injectKeyClicked(int keycode);
        virtual bool injectKeyDown(int keycode);
        virtual bool injectKeyUp(int keycode);

    private:

        /// private struct for holding state info of the controller
        /// this struct only contains data, derived from user input,
        /// Not depending on actual character/camera position.
        struct ControlState
        {
            typedef enum {Stand, Crouch} Pose;
            ControlState();

            Pose pose;
            int inputState;
            bool startJump;

            Ogre::Radian azimuth;
            Ogre::Radian altitude;
            Ogre::Real distance;

            Ogre::Vector3 charVelocity;
        };

        /// This struct holds the state of current movement
        /// of the character. This data is the physical
        /// state of the controller system. It is derived from
        /// former state and user input.
        /// Decoupling is necessary, as user input and physics
        /// control are asynchronous.
        struct CharacterState
        {
            CharacterState();

            bool isAirBorne;
            Ogre::Real jumpTimer;

            Ogre::Vector3 targetPosition;
            Ogre::Vector3 currentPosition;

            Ogre::Radian targetAzimuth;
            Ogre::Radian currentAzimuth;
        };

        /// Camera state is decoupled from both. This is, because
        /// Camera control is affected by user input in the
        /// render loop and by physical collisions which are updated
        /// independently.
        struct CameraState
        {
            CameraState();

            Ogre::Vector3 position;
            Ogre::Vector3 velocity;

            Ogre::Radian azimuthVelocity;
            Ogre::Radian altitudeVelocity;

            Ogre::Vector3 lookAtPosition;
            Ogre::Radian deltaAzimuth;
        };

        typedef std::vector<ControlState> ControlStateVector;
        /// Ring buffer of ControlStates, stored for extrapolation
        ControlStateVector mControlStates;
        /// current state pointer. Incremented on the beginning of run()
        ControlStateVector::size_type mControlStateIdx;

        typedef std::vector<CharacterState> CharacterStateVector;
        /// Ring buffer of CharacterStates, stored for extrapolation
        /// and as a short-term memory for pathfinding
        CharacterStateVector mCharacterStates;
        /// current state pointer. Incremented on the beginning of OnApplyForceAndTorque()
        CharacterStateVector::size_type mCharacterStateIdx;

        typedef std::vector<CameraState> CameraStateVector;
        /// Ring buffer of CameraStates, stored for extrapolation
        /// and as a short-term memory for pathfinding
        CameraStateVector mCameraStates;
        /// current state pointer. Incremented inside of run() and on collision with the world
        CameraStateVector::size_type mCameraStateIdx;

        Creature* mCharacter;

        std::pair<Ogre::Real, Ogre::Real> mDistanceRange;
        std::pair<Ogre::Degree, Ogre::Degree> mAltitudeRange;

        Ogre::Vector3 mLookAtOffset;
        Ogre::Real mMovementSpeed;
        Ogre::Real mRotationSpeed;
        Ogre::Real mSpeedModifier;

        Ogre::Real mLinearSpringK;
        Ogre::Real mLinearDampingK;

        Ogre::Real mAngleDriveK;
        Ogre::Real mAngleDampingK;

        Ogre::Vector3 mGravitation;

        ViewMode mViewMode;

        int mObstractedFrameCount;
        Ogre::Real mObstractedTime;
        int mCameraJammedFrameCount;
        Ogre::Real mCameraJammedTime;

        PhysicsMaterialRaycast* mRaycast;

        /// the maximum amount of time, the character or camera should need,
        /// in order to reach the position, desired by the user.
        /// the smaller, the tighter the feel.
        Ogre::Real mMaxDelay;

        Ogre::String mDebugPageName;

        bool mAnimationStateDirty;

        void updatePickedObject() const;
        void updateAnimationState();

        void updateDebugPage() const;

        bool isCharacterOccluded() const;

        void positionCamera(Ogre::Real elapsedTime);

        void incrementControlStateSlot();
        void incrementCharacterStateSlot();
        void incrementCameraStateSlot();

        size_t getLastControlStateIdx();
    };
}
#endif
