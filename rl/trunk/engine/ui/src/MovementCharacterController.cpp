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

#include "MovementCharacterController.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>
#include <OgreMath.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneQuery.h>

#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "InputManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "PhysicsManager.h"
#include "PhysicsMaterialRaycast.h"
#include "PhysicalThing.h"
#include "World.h"


#include <numeric>

using namespace Ogre;

namespace rl {

	MovementCharacterController::CharacterState::CharacterState()
		: mIsAirBorne(false),
		mStartJump(false),
		mJumpTimer(0.0),
		mDesiredVel(Vector3::ZERO),
		mCurrentMovementState(MOVE_NONE),
		mLastMovementState(MOVE_NONE)
	{

	}

	MovementCharacterController::MovementCharacterController(Actor* camera, Creature* character)
		: CharacterController(camera, character->getActor()),
		mCharacterState(),
		mDesiredDistance(2.00),
		mDistanceRange(0.60, 7.00),
		mYaw(0),
		mPitch(20),
		mPitchRange(Degree(-75), Degree(85)),
		mLookAtOffset(),
		mRotationSpeed(4.0f),
		mSpeedModifier(1.0f),
		mViewMode(VM_THIRD_PERSON),
		mMaxDelay(1.0/30.0),
		mObstractedFrameCount(0),
		mObstractedTime(0.0f),
		mCameraJammedFrameCount(0),
		mCameraJammedTime(0.0f),
		mRaycast(new PhysicsMaterialRaycast()),
		mGravitation(Vector3(0.0f, -9.81f, 0.0f))
	{
		mMovementSpeed = 
			(float)character->getWert(WERT_GS) / 
			(float)Date::ONE_KAMPFRUNDE 
			* 1000.0f;	

		// Offset for the look at point,
		// so the cam does look at the characters head instead of the feet.
		MeshObject* charMesh = dynamic_cast<MeshObject*>(
			mCharacter->getControlledObject());
		mLookAtOffset = Vector3(0, charMesh->getHeight() * 0.45, 0);

		// The actor should be controlled manually,
		// so let the PM prepare it accordingly
		PhysicsManager::getSingleton().setPhysicsController(
			mCharacter->getPhysicalThing(), this);
		PhysicsManager::getSingleton().setPhysicsController(
			mCamera->getPhysicalThing(), this);

		resetCamera();

		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacter->getControlledObject());
		mesh->stopAllAnimations();
		mesh->startAnimation("Idle");
	}

	//------------------------------------------------------------------------
	MovementCharacterController::~MovementCharacterController()
	{
		// actors aren't controlled anymore
		PhysicsManager::getSingleton().setPhysicsController(
			mCharacter->getPhysicalThing(), 0);
		PhysicsManager::getSingleton().setPhysicsController(
			mCamera->getPhysicalThing(), 0);
	}

	//------------------------------------------------------------------------
	void MovementCharacterController::run(Real elapsedTime)
	{
		RL_LONGLONG start = CoreSubsystem::getSingleton().getClock();
		InputManager* im = InputManager::getSingletonPtr();

		// Fetch current movement state
		mCharacterState.mDesiredVel = Vector3::ZERO;
		int movement = im->getCommandMapper()->getActiveMovement();
		mCharacterState.mCurrentMovementState = movement;

		// Determine character's control state based on user input
		if (movement & MOVE_FORWARD)
			mCharacterState.mDesiredVel.z = -1;

		if (movement & MOVE_BACKWARD)
			mCharacterState.mDesiredVel.z = 1;

		if (movement & MOVE_RIGHT)
			mCharacterState.mDesiredVel.x = 1;

		if (movement & MOVE_LEFT)
			mCharacterState.mDesiredVel.x = -1;
		
		mCharacterState.mDesiredVel.normalise();
		mCharacterState.mDesiredVel *= mMovementSpeed;
		
		if (movement & MOVE_JUMP)
			mCharacterState.mStartJump = true;

		if (movement & TURN_LEFT)
			mYaw += Degree(mRotationSpeed * 30.0 * elapsedTime);

		if (movement & TURN_RIGHT)
			mYaw -= Degree(mRotationSpeed * 30.0 * elapsedTime);

		if (movement & MOVE_RUN)
			mCharacterState.mDesiredVel *= 3.0;

		mDesiredDistance -= im->getMouseRelativeZ() * 0.002;
		if (mDesiredDistance < mDistanceRange.first)
		{
			mDesiredDistance = mDistanceRange.first;
		}
		if (mDesiredDistance > mDistanceRange.second)
		{
			mDesiredDistance = mDistanceRange.second;
		}

		mPitch += Degree(im->getMouseRelativeY() * 25.0 * elapsedTime);
		if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
		if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;

		mYaw -= Degree(im->getMouseRelativeX() * 25.0 * elapsedTime);

		SceneNode* cameraNode = mCamera->_getSceneNode();
		cameraNode->lookAt(mCharacter->getWorldPosition()
			+ mLookAtOffset*2.0, Node::TS_WORLD);

		while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
		while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);

		if (isCharacterOccluded())
		{
			mObstractedTime += elapsedTime;
			++mObstractedFrameCount;
		}
		else
		{
			mObstractedTime = 0.0f;
			mObstractedFrameCount = 0;
		}

		// if we have more than 250ms and at least five frames with no direct sight, reset camera
		if (mObstractedTime > 0.250f && mObstractedFrameCount > 5)
		{
			mObstractedFrameCount = 0;
			resetCamera();
		}

		// if we have more than 250ms and at least five frames with camera distance higher
		// than desired distance, reset camera
		if ((mCamera->getWorldPosition()
			- (mCharacter->getWorldPosition() + mLookAtOffset*2.0)).length() 
			> 2.0f * mDesiredDistance)
		{
			mCameraJammedTime += elapsedTime;
			++mCameraJammedFrameCount;
		}
		else
		{
			mCameraJammedTime = 0.0f;
			mCameraJammedFrameCount = 0;
		}

		if (mCameraJammedTime > 0.250f && mCameraJammedFrameCount > 5)
		{
			mCameraJammedFrameCount = 0;
			resetCamera();
		}

		if (!InputManager::getSingleton().isCeguiActive())
		{
			updatePickedObject();
		}
		updateAnimationState();

		Logger::getSingleton().log(
			Logger::CORE, 
			Ogre::LML_TRIVIAL, 
			"    MCC time "
			 + Ogre::StringConverter::toString(
					Ogre::Real((double)(CoreSubsystem::getSingleton().getClock()-start))));
	}

	int MovementCharacterController::userProcess()
	{
		if (m_body0 == mCamBody || m_body1 == mCamBody)
		{
			// this is camera collision
			// empty so far
		}
		else
		{
			// this is character collision
			Vector3 point;
			Vector3 normal;
			getContactPositionAndNormal(point, normal);

			// determine if this contact is with the floor.
			// Meaning the contact normal has an angle to UNIT_Y of 20° or less.
			Degree angle = Math::ACos(normal.dotProduct(Vector3::UNIT_Y));

			if (angle <= Degree(30.0f))
			{
				mCharacterState.mIsAirBorne = false;

				Vector3 velocity = mCharBody->getVelocity();

				// calculate char velocity perpendicular to the contact normal
				Vector3 tangentVel = velocity - normal * (normal.dotProduct(velocity));

				// align the tangent at the contact point with the
				// tangent velocity vector of the char
				rotateTangentDirections(tangentVel);
			}

			setContactElasticity(0.2f); // was 0.3f
			setContactSoftness(0.01f);

			setContactFrictionState(1.0, 0);
			setContactFrictionState(1.0, 1);
		}

		// return one to tell Newton we want to accept this contact
		return 1;
	}

	void MovementCharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
	{
		OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
		OgreNewt::Body* body = thing->_getBody();

		// Get the current world timestep
		Real timestep = world->getTimeStep();
		mCharacterState.mJumpTimer += timestep;

		if (body == mCamBody)
		{
			Vector3 charPos;
			Quaternion charOri;
			mCharBody->getPositionOrientation(charPos, charOri);

			Vector3 camPos;
			Quaternion camOri;
			mCamBody->getPositionOrientation(camPos, camOri);

			// determine the optimal target position of the camera
			Vector3 targetCamPos = Vector3(0, Math::Sin(mPitch) * mDesiredDistance,
				Math::Cos(mPitch) * mDesiredDistance);
			targetCamPos = charOri * targetCamPos;
			targetCamPos += charPos + mLookAtOffset;

			Vector3 diff = targetCamPos - camPos;

			// cap the diff to the next obstacle
			OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
			OgreNewt::MaterialID* levelId =
				PhysicsManager::getSingleton()._getLevelMaterialID();
			Camera* camera = dynamic_cast<CameraObject*>(mCamera->getControlledObject())->getCamera();
			Vector3 target = targetCamPos
				+ 1.2f * camera->getNearClipDistance() * diff.normalisedCopy();
			RaycastInfo info = mRaycast->execute(world, levelId, camPos, target);
			if (info.mBody)
			{
				diff = info.mDistance * (target - camPos) -
					info.mDistance *
					(1.2f * camera->getNearClipDistance() * diff.normalisedCopy());
			}

			// determine velocity vector to get there.
			// how fast has the camera to be,
			// in order to get there in mMaxDelay seconds?
			Vector3 vel = diff / mMaxDelay;

			// adjust scale of camera collision according to the velocity vector
			OgreNewt::CollisionPrimitives::HullModifier* hc =
				static_cast<OgreNewt::CollisionPrimitives::HullModifier*>(
				mCamBody->getCollision());
			Matrix4 mat = Matrix4::getScale(
				Vector3(1.0f, 1.0f, 1.0f) + 3.0f * vel / vel.length());
			hc->setMatrix(mat);

			// calcuate force and apply it
			Real mass;
			Vector3 inertia;
			mCamBody->getMassMatrix(mass, inertia);
			Vector3 force = mass * (vel - mCamBody->getVelocity()) / timestep;
			mCamBody->setForce(force);
		}
		else
		{
			// apply character force
			Vector3 position;
			Quaternion orientation;
			body->getPositionOrientation(position, orientation);

			// get the charater mass
			Real mass;
			Vector3 inertia;
			body->getMassMatrix(mass, inertia);

			// apply gravity
			Vector3 force = mass * mGravitation;

			// Get the velocity vector
			Vector3 currentVel = body->getVelocity();

			// Gravity is applied above, so not needed here
			// prevent adding a counter force against gravity
			if (currentVel.y < 0.0f || mCharacterState.mJumpTimer < 2.0f) currentVel.y = 0.0f;

			force += mass*(orientation*mCharacterState.mDesiredVel - currentVel) / timestep;

			// apply the jump
			if (!mCharacterState.mIsAirBorne && mCharacterState.mStartJump)
			{
				mCharacterState.mStartJump = false;
				mCharacterState.mJumpTimer = 0.0f;
				force += mass*4.0f/timestep * Vector3::UNIT_Y;
			}

			body->setForce(force);

			// Calculate angular velocity

			// We first need the yaw rotation from actual yaw to desired yaw
			Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
			src.y = 0;
			Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::NEGATIVE_UNIT_Z;
			dst.y = 0;
			Radian yaw = src.getRotationTo(dst).getYaw();

			// Calculate omega in order to go this rotation in mMaxDelay seconds.
			Real newOmega = yaw.valueRadians() / mMaxDelay;
			body->setOmega(Vector3(0, newOmega, 0));

			//std::ostringstream ss;
			//ss << "orientation: " << orientation.getYaw().valueDegrees()
			//    << " mYaw: " << mYaw.valueDegrees()
			//    << " yaw: " << yaw.valueDegrees()
			//    << " no: " << newOmega;

			//Logger::getSingleton().log("RlUi", Ogre::LML_TRIVIAL, ss.str());

			// Assume we are air borne.
			// Might be set to false in the collision callback
			mCharacterState.mIsAirBorne = true;
		}
	}

	/// Do raycasts to determine whether sight to character is free
	bool MovementCharacterController::isCharacterOccluded() const
	{
		/// \todo remove hard coded numbers
		bool rval = true;
		int numPoints = 4;
		float xs[] = {0.0f, 30.0f, -35.0f, 0.0f};
		float ys[] = {mLookAtOffset.y*2.0, mLookAtOffset.y, mLookAtOffset.y, 20.0f};
		OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
		OgreNewt::MaterialID* levelId = PhysicsManager::getSingleton()._getLevelMaterialID();

		// Test the track points on the char.
		// Break early, if one ray doesn't hit an obstacle
		for (int i = 0; i < numPoints; ++i)
		{
			RaycastInfo info = mRaycast->execute(world, levelId,
				mCamera->getWorldPosition(),
				mCharacter->getWorldPosition() + Vector3(xs[i], ys[i], 0.0f));
			rval = rval && info.mBody;
			if (!rval) break;
		}

		return rval;
	}

	void MovementCharacterController::updatePickedObject() const
	{
		InputManager::getSingleton().updatePickedObject(0.5, 0.5);
	}

	void MovementCharacterController::updateAnimationState()
	{
		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacter->getControlledObject());

		if (mCharacterState.mCurrentMovementState != mCharacterState.mLastMovementState)
		{
			mesh->stopAllAnimations();
			if (mCharacterState.mCurrentMovementState & MOVE_SNEAK)
			{
				if (mCharacterState.mCurrentMovementState & MOVE_FORWARD)
				{
					mesh->startAnimation("hocke walk");
				}
				else
				{
					mesh->startAnimation("idle hocke");
				}
			}
			else if ((mCharacterState.mCurrentMovementState & MOVE_RUN) &&
				(mCharacterState.mCurrentMovementState != MOVE_RUN))
			{
				mesh->startAnimation("Run");
			}
			else if (mCharacterState.mCurrentMovementState == MOVE_NONE ||
				mCharacterState.mCurrentMovementState == MOVE_RUN)
			{
				mesh->startAnimation("Idle");
			}
			else
			{
				// standard walk in any direction
				if (mCharacterState.mCurrentMovementState & MOVE_FORWARD)
				{
					mesh->startAnimation("walk");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_BACKWARD)
				{
					mesh->startAnimation("Rueckwaerts");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_LEFT)
				{
					mesh->startAnimation("sidestep2");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_RIGHT)
				{
					mesh->startAnimation("Sidestep1");
				}
				else if (mCharacterState.mCurrentMovementState & TURN_LEFT)
				{
					mesh->startAnimation("TurnLeft");
				}
				else if (mCharacterState.mCurrentMovementState & TURN_RIGHT)
				{
					mesh->startAnimation("TurnRight");
				}
			}
			mCharacterState.mLastMovementState = mCharacterState.mCurrentMovementState;
		}
	}

	void MovementCharacterController::setViewMode(ViewMode mode)
	{
		mViewMode = mode;
		if (mode == VM_FIRST_PERSON)
		{
			mCharacter->_getSceneNode()->setVisible(false);
			mDesiredDistance = 0.0;
			mCamera->getPhysicalThing()->setPosition(mCharacter->getPhysicalThing()->getPosition());
		}
		else
		{
			mCharacter->_getSceneNode()->setVisible(true);
			mDesiredDistance = 150.0;
			resetCamera();
		}
	}

	MovementCharacterController::ViewMode MovementCharacterController::getViewMode()
	{
		return mViewMode;
	}

	void MovementCharacterController::toggleViewMode()
	{
		if (getViewMode() == VM_FIRST_PERSON)
			setViewMode(VM_THIRD_PERSON);
		else
			setViewMode(VM_FIRST_PERSON);
	}

	void MovementCharacterController::resetCamera()
	{
		// Position camera at char position
		mCamera->getPhysicalThing()->setPosition(mCharacter->getPhysicalThing()->getPosition());
		if (mViewMode == VM_THIRD_PERSON)
		{
			mPitch = Degree(30.0);
		}
	}
}
