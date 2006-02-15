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

#ifndef __GameController_H__
#define __GameController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"
#include "PhysicsController.h"
#include "PhysicsMaterialRaycast.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneQuery.h>
#undef sleep

#include <deque>

namespace rl {

    class Actor;
    class MeshObject;

    /**
     * This class handles character control via user input.
     */
    class _RlUiExport CharacterController : public GameTask,
        public PhysicsController
    {
    public:
        typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON} ViewMode;
        /**
         *  @throw NullPointerException if camera or character is NULL.
         *  @throw InvalidArgumentException if character is not placed in the scene.
         */
        CharacterController(Actor* camera, Actor* character);
        virtual ~CharacterController();

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

    private:
        typedef enum {AS_STAND, AS_WALK_FORWARD} AnimationState;

        Actor* mCamera;
        Actor* mCharacter;

        OgreNewt::Body* mCamBody;
        OgreNewt::Body* mCharBody;

        // camera control params
        /// optimal distance to the character
        Ogre::Real mDesiredDistance;
        std::pair<Ogre::Real, Ogre::Real> mDistanceRange;
        Ogre::Degree mYaw;
        Ogre::Degree mPitch;
        std::pair<Ogre::Degree, Ogre::Degree> mPitchRange;

        Ogre::Vector3 mLookAtOffset;
        Ogre::Real mMovementSpeed;
        Ogre::Real mRotationSpeed;
        Ogre::Real mSpeedModifier;

        Ogre::Vector3 mDesiredVel;
		Ogre::Vector3 mGravitation;

        AnimationState mCurrentAnimationState;
        AnimationState mLastAnimationState;

        ViewMode mViewMode;

        bool mIsAirBorne;
        bool mIsStopped;
        bool mStartJump;
        Ogre::Real mJumpTimer;

        int mObstractedFrameCount;
        Ogre::Real mObstractedTime;
        int mCameraJammedFrameCount;
        Ogre::Real mCameraJammedTime;

        PhysicsMaterialRaycast* mRaycast;

        /// the maximum amount of time, the character or cmaera should need,
        /// in order to reach the position, desired by the user.
        /// the smaller, the tighter the feel.
        Ogre::Real mMaxDelay;

        void updatePickedObject() const;
        void updateAnimationState(const Vector3& translation);

        bool isCharacterOccluded() const;
    };
}
#endif
