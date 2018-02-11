/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CombatCharacterController_H__
#define __CombatCharacterController_H__

#include "UiPrerequisites.h"

#include "ControlState.h"
#include "Combatant.h"
#include "MessagePump.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include "Selector.h"


namespace rl {

	class CameraObject;
    class Combat;
    class CombatGui;
    class CombatManager;

	class _RlUiExport CombatControlState :
            public ControlState,
            public Combatant,
            public PhysicsController,
            public PhysicsGenericContactCallback
	{
	public:
        /** 
         * Different view-modes:
         * @VM_COMBAT_CENTERED: "cameraLookAt" to center of combat (middle of all combatant positions)
         * @VM_THIRD_PERSON: zooms to one combatant (not only hero!)
        */
        typedef enum {VM_COMBAT_CENTERED, VM_THIRD_PERSON} ViewMode;


		/**
		*  @throw NullPointerException if camera is NULL.
		*  @warning Ownership of combat is taken by the CombatControlState.
		*/
		CombatControlState(CommandMapper* cmdMapper, Actor* camera, Creature* character);
		virtual ~CombatControlState();

		// Control state overrides

        virtual void pause();
        virtual void resume();
        virtual bool keyPressed(const OIS::KeyEvent& evt, bool handled);
        virtual bool keyReleased(const OIS::KeyEvent& evt, bool handled);

		void run(Ogre::Real elapsedTime);

        // Combatant overrides

        virtual Ogre::String getCombatantTypeName() const;
        virtual void requestCombatantAction();


        // camera collision: OgreNewt::ContactCallback overides
        int onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex);
        void userProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timestep, int threadIndex);

        // camera movement: Newton force and torque callback
        void OnApplyForceAndTorque(PhysicalThing *pt, float timstep);

        // Change ViewMode
        void setViewMode(ViewMode mode);
        ViewMode getViewMode();
        void toggleViewMode();
        // set the combatant (must be part of this combat), the camera should focus on (VM_THIRD_PERSON) (only used internally?)
        void setCameraFocusedCombatant(Combatant* combatant);

    private:
		enum State {REQUEST_USER_INPUT, ROUND_EXECUTION};

		MessagePump::ScopedConnection mCombatIoAttackOpponentConnection;
		MessagePump::ScopedConnection mCombatIoParryOpponentConnection;
		MessagePump::ScopedConnection mCombatIoEndTurnRequestedConnection;

	    MessagePump::ScopedConnection mEnemyLeftCombatConnection;
	    MessagePump::ScopedConnection mCombatEndConnection;

		Combatant* mAttackedOpponent;
		Combatant* mParriedOpponent;

        CombatManager* mCombatManager;
		CombatGui* mCombatGui;
        HalfSphereSelector mEnemySelector;

        CameraObject* mCamera;

		State mState;

        // Camera:
        Ogre::Degree mCameraYaw, mCameraPitch;
        Ogre::Real mCameraMaxDistance, mCameraCombatCenteredMinDistance, mCameraThirdPersonMinDistance;
        int mMovementState; // keyboard input
        Ogre::Real mCameraLinearDampingK, mCameraLinearSpringK;
        ViewMode mViewMode;
        Ogre::Vector3 mCameraLookAt; // set by calculateOptimalCameraPositionAndLookAt
        Ogre::Vector3 mCameraOptPos; // set by calculateOptimalCameraPositionAndLookAt
        Ogre::Real mCameraDistance; // only used in third-person, changed by calculateOptimalCameraAndPosition
        Ogre::Real mCameraSwitchDist; // set by calculateOptimalCameraPositionAndLookAt, dist at which switches to third-person
        Ogre::Real mCameraSwitchTransitionDist; // relative value (0-1) of mCameraSwitchDist
        Combatant* mCameraFocusedCombatant;  // the combatant currently focused by camera (VM_THIRD_PERSON)
        // when switching from one view-mode to another or when changing focused combatant, these variables are used
        // to provide smooth camera movement
        bool mCameraTransitionLookAtActive;
        bool mCameraTransitionPositionActive;
        Ogre::Vector3 mCameraTransitionPosition;
        Ogre::Vector3 mCameraTransitionLookAt;
        // buffered values needed fo camera-movement, calculated once per frame:
        Ogre::Vector3 mCombatCenter; // result of calculateCombatCenterPosition
        Ogre::Real mCombatRadius; // result of calculateCombatRadius

        // Camera helper functions
        void resetCamera();
        void updateCameraLookAt(Ogre::Real timestep);
        Ogre::Vector3 calculateCombatCenterPosition();
        Ogre::Real calculateCombatRadius(Ogre::Vector3 center);
        // sets the variables mCameraLookAt, mCameraOptPos, mCameraDistance, usually called in OnApplyForceAndTorque
        void calculateOptimalCameraPositionAndLookAt();
        
        // Event handlers
		bool userRequestAttackOpponent(Combatant*);
		bool userRequestParryOpponent(Combatant*);
		bool userRequestEndTurn();

		bool enemyLeftCombat(Combatant*);
        bool combatEnded(bool);
	};
}
#endif
