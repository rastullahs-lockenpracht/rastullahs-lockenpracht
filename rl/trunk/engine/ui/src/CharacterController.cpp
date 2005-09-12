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

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>
#include <OgreMath.h>


using namespace Ogre;

namespace rl {

    CharacterController::CharacterController()
    {
        // Must not be used.
        Throw(OperationNotSupportedException, "Do not use the standard constructor.");
    }
    
    //------------------------------------------------------------------------

    CharacterController::CharacterController(Actor* camera, Actor* character)
        : 
        mCamera(camera),
        mCharacter(character),
        mDesiredDistance(200),
        mPitch(20),
        mYaw(0),
        mLookAtOffset(),
        mMovementSpeed(180.0f),
        mRotationSpeed(5),
        mDesiredVel(),
        mDesiredOmega(),
        mCurrentAnimationState(AS_STAND),
        mLastAnimationState(AS_STAND),
        mViewMode(VM_THIRD_PERSON),
        mIsAirBorne(true),
        mIsStopped(false),
        mStartJump(false)
    {
        if (mCamera == 0 || mCharacter == 0)
        {
            Throw(NullPointerException, "Character and Camera must not be NULL.");
        }

        if (!mCharacter->_getSceneNode())
        {
            Throw(InvalidArgumentException, "character has to be placed in the scene beforehand");
        }

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
        mLookAtOffset = Vector3(0, charMesh->getHeight() * 0.9, 0);

        // The actor should be controlled manually, so let the PM prepare it accordingly
        PhysicsManager::getSingleton().setPhysicsController(mCharacter->getPhysicalThing(), this);
    }

    //------------------------------------------------------------------------
    CharacterController::~CharacterController()
    {
    }

    //------------------------------------------------------------------------
    void CharacterController::run(Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();

        mDesiredVel = Vector3::ZERO;
        mDesiredOmega = 0.0f;

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
            mDesiredOmega += mRotationSpeed;

        if (cmdmap->isMovementActive(TURN_RIGHT))
            mDesiredOmega -= mRotationSpeed;

        if (cmdmap->isMovementActive(MOVE_RUN))
            mDesiredVel *= 4.0;

        mDesiredDistance -= im->getMouseRelativeZ() * 0.05;
        mPitch += Degree(im->getMouseRelativeY() * 0.13);

        mDesiredOmega -=im->getMouseRelativeX() * 0.25;

        SceneNode* cameraNode = mCamera->_getSceneNode();
        Radian yaw = mCharacter->getWorldOrientation().getYaw();

        Vector3 cameraPosition = Vector3(0, Math::Sin(mPitch) * mDesiredDistance, Math::Cos(mPitch) * mDesiredDistance);

        cameraPosition = mCharacter->getWorldOrientation()*cameraPosition;
        cameraPosition += mCharacter->getWorldPosition() + mLookAtOffset;

        cameraNode->setPosition(cameraPosition);
        cameraNode->lookAt(mCharacter->getWorldPosition() + mLookAtOffset, Node::TS_WORLD);
        mCamera->_update();

        if (!InputManager::getSingleton().isCeguiActive())
        {
            updatePickedObject();
        }
        updateAnimationState(mDesiredVel);
    }

    int CharacterController::userProcess()
    {
        // apply the default behaviuor
        PhysicsGenericContactCallback::userProcess();

        // determine what body of the two is the char.
        // Since the char has a mass and the level hasn't this is easy.
        Real mass;
        Vector3 inertia;
        m_body0->getMassMatrix(mass, inertia);
        OgreNewt::Body* characterBody = mass > 0.0f ? m_body0 : m_body1;

        // Get the pointer to the character
        Vector3 point;
        Vector3 normal;
        getContactPositionAndNormal(point, normal);

        // determine if this contact is with the floor.
        // Meaning the contact normal has an angle to UNIT_Y of 20° or less.
        Degree angle = Math::ACos(normal.dotProduct(Vector3::UNIT_Y));

        if (angle <= Degree(20.0f))
        {
            mIsAirBorne = false;

            Vector3 velocity = characterBody->getVelocity();

            // calculate char velocity perpendicular to the contact normal
            Vector3 tangentVel = velocity - normal * (normal.dotProduct(velocity));

            // align the tangent at the contact point with the tangent velocity vector of the char
            rotateTangentDirections(tangentVel);

            // we do want bound back we hitting the floor
            setContactElasticity(0.3f);
        }

        setContactFrictionState(0, 0);
        setContactFrictionState(0, 1);

        // return one to tell Newton we want to accept this contact
        return 1;
    }

    void CharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
    {
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        OgreNewt::Body* body = thing->_getBody();

        Vector3 position;
        Quaternion orientation;
        body->getPositionOrientation(position, orientation);

        // Get the current world timestep
        Real timestep = world->getTimeStep();

        // get the charater mass
        Real mass;
        Vector3 inertia;
        body->getMassMatrix(mass, inertia);

        // apply gravity
        Vector3 force = Vector3(0.0f, -mass * 988.0, 0.0f);

        // Get the velocity vector
        Vector3 currentVel = body->getVelocity();

        // Gravity is applied above, so not needed here
        currentVel.y = 0;

        force += mass*(orientation*mDesiredVel - currentVel) / timestep;

        // apply the jump
        if (!mIsAirBorne && mStartJump)
        {
            mStartJump = false;
            force += mass*400.f/timestep * Vector3::UNIT_Y;
        }

        body->setForce(force);

        // calculate the torque vector
        Vector3 omega = body->getOmega();
        Real k = 0.25f; // What exactly is this k, and why is it either 0.25 or 0.5
        Vector3 torque (0.0f, k * inertia.y * (mDesiredOmega-omega.y) / timestep, 0.0f);
        body->setTorque(torque);

        // assume we are air borne. Might be set to false in the collision callback
        mIsAirBorne = true;

        // did we apply force in horizontal direction? If not, the char is stopped.
        mIsStopped = Math::Abs(force.x) < 0.001f && Math::Abs(force.z) < 0.001f;
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
        if (mViewMode == VM_THIRD_PERSON)
        {
            mPitch = Degree(-30.0);
            mDesiredDistance = 150.0;
        }
    }
}
