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

#include "CharacterController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "Actor.h"
#include "CameraObject.h"
#include "MeshObject.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"
#include "MeshObject.h"
#include "ActorManager.h"
#include "Logger.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>
#include <OgreMath.h>

#include <numeric>

using namespace Ogre;

namespace rl {

    CharacterController::CharacterController(Actor* camera, Actor* character)
        : 
        mCamera(camera),
        mCharacter(character),
        mCamBody(),
        mCharBody(),
        mDesiredDistance(200),
        mDistanceRange(150, 500),
        mYaw(0),
        mPitch(20),
        mPitchRange(Degree(-75), Degree(85)),
        mLookAtOffset(),
        mMovementSpeed(360.0f),
        mRotationSpeed(4.0f),
        mDesiredVel(),
        mCurrentAnimationState(AS_STAND),
        mLastAnimationState(AS_STAND),
        mViewMode(VM_THIRD_PERSON),
        mIsAirBorne(true),
        mIsStopped(false),
        mStartJump(false),
        mMaxDelay(1.0/30.0),
        mObstractedFrameCount(0),
        mCameraJammedFrameCount(0),
        mRaycast(new PhysicsMaterialRaycast())
    {
        if (mCamera == 0 || mCharacter == 0)
        {
            Throw(NullPointerException, "Character and Camera must not be NULL.");
        }

        if (!mCharacter->_getSceneNode())
        {
            Throw(InvalidArgumentException, "character has to be placed in the scene beforehand");
        }

        mCharBody = mCharacter->getPhysicalThing()->_getBody();
        mCamBody = mCamera->getPhysicalThing()->_getBody();

        if (!mCamera->_getSceneNode())
        {
            mCamera->placeIntoNode(CoreSubsystem::getSingleton().
                getWorld()->getSceneManager()->getRootSceneNode());
        }

        // Reset camera position/orientation, as it is now controlled via scene nodes.
        Camera* ogreCam = static_cast<CameraObject*>(
            mCamera->getControlledObject())->getCamera();
        ogreCam->setPosition(Vector3::ZERO);
        ogreCam->setOrientation(Quaternion::IDENTITY);

        mCamera->_getSceneNode()->setFixedYawAxis(true);

        // Offset for the look at point,
        // so the cam does look at the characters head instead of the feet.
        MeshObject* charMesh = static_cast<MeshObject*>(mCharacter->getControlledObject());
        mLookAtOffset = Vector3(0, charMesh->getHeight() * 0.45, 0);

        // The actor should be controlled manually, so let the PM prepare it accordingly
        PhysicsManager::getSingleton().setPhysicsController(mCharacter->getPhysicalThing(), this);
        PhysicsManager::getSingleton().setPhysicsController(mCamera->getPhysicalThing(), this);

        resetCamera();
    }

    //------------------------------------------------------------------------
    CharacterController::~CharacterController()
    {
        // actors aren't controlled anymore
        PhysicsManager::getSingleton().setPhysicsController(mCharacter->getPhysicalThing(), 0);
        PhysicsManager::getSingleton().setPhysicsController(mCamera->getPhysicalThing(), 0);
    }

    //------------------------------------------------------------------------
    void CharacterController::run(Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();

        mDesiredVel = Vector3::ZERO;

        if (cmdmap->isMovementActive(MOVE_FORWARD))
            mDesiredVel.z = -mMovementSpeed;

        if (cmdmap->isMovementActive(MOVE_BACKWARD))
            mDesiredVel.z = mMovementSpeed;

        if (cmdmap->isMovementActive(MOVE_RIGHT))
            mDesiredVel.x = mMovementSpeed;

        if (cmdmap->isMovementActive(MOVE_LEFT))
            mDesiredVel.x = -mMovementSpeed;

        if (cmdmap->isMovementActive(MOVE_JUMP))
            mStartJump = true;

        if (cmdmap->isMovementActive(TURN_LEFT))
            mYaw += Degree(mRotationSpeed * 40.0 * elapsedTime);

        if (cmdmap->isMovementActive(TURN_RIGHT))
            mYaw -= Degree(mRotationSpeed * 40.0 * elapsedTime);

        if (cmdmap->isMovementActive(MOVE_RUN))
            mDesiredVel *= 4.0;

        mDesiredDistance -= im->getMouseRelativeZ() * 0.05;
        if (mDesiredDistance < mDistanceRange.first) mDesiredDistance = mDistanceRange.first;
        if (mDesiredDistance > mDistanceRange.second) mDesiredDistance = mDistanceRange.second;

        mPitch += Degree(im->getMouseRelativeY() * 30.0 * elapsedTime);
        if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
        if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;

        mYaw -= Degree(im->getMouseRelativeX() * 30.0 * elapsedTime);

        SceneNode* cameraNode = mCamera->_getSceneNode();
        cameraNode->lookAt(mCharacter->getWorldPosition() + mLookAtOffset*2.0, Node::TS_WORLD);

        while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
        while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);

        if (isCharacterOccluded()) ++mObstractedFrameCount;

        // if we have more than ten frames with no direct sight, reset camera
        if (mObstractedFrameCount > 10)
        {
            mObstractedFrameCount = 0;
            resetCamera();
        }

        if ((mCamera->getWorldPosition() - (mCharacter->getWorldPosition() + mLookAtOffset*2.0)).length() 
            > 2.0f * mDesiredDistance)
        {
            ++mCameraJammedFrameCount;
        }
        // if we have more than ten frames with camera distance higher
        // than desired distance, reset camera
        if (mCameraJammedFrameCount > 10)
        {
            mCameraJammedFrameCount = 0;
            resetCamera();
        }

        if (!InputManager::getSingleton().isCeguiActive())
        {
            updatePickedObject();
        }
        updateAnimationState(mDesiredVel);
    }

    int CharacterController::userProcess()
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

            if (angle <= Degree(20.0f))
            {
                mIsAirBorne = false;

                Vector3 velocity = mCharBody->getVelocity();

                // calculate char velocity perpendicular to the contact normal
                Vector3 tangentVel = velocity - normal * (normal.dotProduct(velocity));

                // align the tangent at the contact point with the tangent velocity vector of the char
                rotateTangentDirections(tangentVel);

                // we do want bound back we hitting the floor
                setContactElasticity(0.1f); // was 0.3f
            }

            setContactFrictionState(0, 0);
            setContactFrictionState(0, 1);
        }

        // return one to tell Newton we want to accept this contact
        return 1;
    }

    void CharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
    {
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        OgreNewt::Body* body = thing->_getBody();

        // Get the current world timestep
        Real timestep = world->getTimeStep();

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
            OgreNewt::MaterialID* levelId = PhysicsManager::getSingleton()._getLevelMaterialID();
            Camera* camera = static_cast<Camera*>(mCamera->_getMovableObject());
            Vector3 target = targetCamPos + 1.2f * camera->getNearClipDistance() * diff.normalisedCopy();
            RaycastInfo info = mRaycast->execute(world, levelId, camPos, target);
            if (info.mBody)
            {
                diff = info.mDistance * (target - camPos) -
                    info.mDistance * (1.2f * camera->getNearClipDistance() * diff.normalisedCopy());
            }

            // determine velocity vector to get there.
            // how fast has the camera to be, in order to get there in mMaxDelay seconds?
            Vector3 vel = diff / mMaxDelay;

            // adjust scale of camera collision according to the velocity vector
            OgreNewt::CollisionPrimitives::HullModifier* hc =
                static_cast<OgreNewt::CollisionPrimitives::HullModifier*>(mCamBody->getCollision());
            Matrix4 mat = Matrix4::getScale(Vector3(1.0f, 1.0f, 1.0f) + 3.0f * vel / vel.length());
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
            Vector3 force = Vector3(0.0f, -mass * 980.0, 0.0f);

            // Get the velocity vector
            Vector3 currentVel = body->getVelocity();

            // Gravity is applied above, so not needed here
            currentVel.y = 0;

            force += mass*(orientation*mDesiredVel - currentVel) / timestep;

            // apply the jump
            if (!mIsAirBorne && mStartJump)
            {
                mStartJump = false;
                force += mass*800.f/timestep * Vector3::UNIT_Y;
            }

            body->setForce(force);


            // Calculate angular velocity
            
            // we first need the yaw rotation from actual yaw to desired yaw
            Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
            src.y = 0;
            Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::NEGATIVE_UNIT_Z;
            dst.y = 0;
            Radian yaw = src.getRotationTo(dst).getYaw();

            // calculate omega in order to go this rotation in mMaxDelay seconds.
            Real newOmega = yaw.valueRadians() / (mMaxDelay*0.25);
            body->setOmega(Vector3(0, newOmega, 0));

            //std::stringstream ss;
            //ss << "orientation: " << orientation.getYaw().valueDegrees()
            //    << " mYaw: " << mYaw.valueDegrees()
            //    << " yaw: " << yaw.valueDegrees()
            //    << " no: " << newOmega;

            //Logger::getSingleton().log(Ogre::LML_TRIVIAL, "Ui", ss.str());

            // assume we are air borne. Might be set to false in the collision callback
            mIsAirBorne = true;

            // did we apply force in horizontal direction? If not, the char is stopped.
            mIsStopped = Math::Abs(force.x) < 0.001f && Math::Abs(force.z) < 0.001f;
        }
    }

    /// Do raycasts to determine whether sight to character is free
    bool CharacterController::isCharacterOccluded() const
    {
        /// \todo remove hard coded numbers
        bool rval = true;
        int numPoints = 4;
        float xs[] = {0.0f, 30.0f, -35.0f, 0.0f};
        float ys[] = {mLookAtOffset.y*2.0, mLookAtOffset.y, mLookAtOffset.y, 20.0f};
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        OgreNewt::MaterialID* levelId = PhysicsManager::getSingleton()._getLevelMaterialID();

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

    void CharacterController::updatePickedObject() const
    {
        InputManager::getSingleton().updatePickedObject(0.5, 0.5);
    }

    void CharacterController::updateAnimationState(const Vector3& translation)
    {
        MeshObject* mesh = static_cast<MeshObject*>(mCharacter->getControlledObject());
        mCurrentAnimationState =
            translation != Vector3::ZERO ? AS_WALK_FORWARD : AS_STAND;

        if (mCurrentAnimationState != mLastAnimationState)
        {
            if (mCurrentAnimationState == AS_WALK_FORWARD)
            {
                mesh->startAnimation("gehloop", 2.0, 0);
            }
            else
            {
                mesh->stopAnimation("gehloop");
            }
            mLastAnimationState = mCurrentAnimationState;
        }
    }

    void CharacterController::setViewMode(ViewMode mode)
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

    CharacterController::ViewMode CharacterController::getViewMode()
    {
        return mViewMode;
    }

    void CharacterController::toggleViewMode()
    {
        if (getViewMode() == VM_FIRST_PERSON)
            setViewMode(VM_THIRD_PERSON);
        else
            setViewMode(VM_FIRST_PERSON);
    }

    void CharacterController::resetCamera()
    {
        // Position camera at char position
        mCamera->getPhysicalThing()->setPosition(mCharacter->getPhysicalThing()->getPosition());
        if (mViewMode == VM_THIRD_PERSON)
        {
            mPitch = Degree(30.0);
            mDesiredDistance = 150.0;
        }
    }
}
