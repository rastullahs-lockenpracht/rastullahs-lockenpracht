/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "GameController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "Actor.h"
#include "PhysicsManager.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>

#include "OgreNoMemoryMacros.h"
#include <ode/ode.h>
#include "OgreMemoryMacros.h"

using namespace Ogre;

namespace rl {

    GameController::GameController()
    {
        // Must not be used.
        Throw(OperationNotSupportedException, "Do ot use the standard constructor.");
    }
    
    GameController::GameController(
        Camera* camera, Actor* actor)
        : mSceneManager(CoreSubsystem::getSingletonPtr()->
        getWorld()->getSceneManager()),
        mControlNode(0),
        mLookAtNode(0),
        mCamera(camera),
        mActor(actor),
        mMoveScale(0),
        mRotScale(0),
        mMoveSpeed(200.0),
        mRotSpeed(80.0),
        mFallSpeed(0.1),
        mCurrentAnimationState(AS_STAND),
        mLastAnimationState(AS_STAND),
        mMaxPitch(Degree(60.0)),
        mMinPitch(-Degree(89.0)),
        mTargetPitch(-Degree(30.0)),
        mMaxDistance(400.0),
        mMinDistance(0.0),
        mTargetDistance(150.0),
        mDesiredDistance(mTargetDistance),
        mViewMode(VM_THIRD_PERSON)
    {
        if (actor == 0 || camera == 0)
        {
            Throw(NullPointerException, "actor und camera duerfen nicht NULL sein.");
        }

        SceneNode* root = mSceneManager->getRootSceneNode();

        mControlNode = root->createChildSceneNode("ControlNode");

        mLookAtNode = mControlNode->createChildSceneNode("LookAtNode");
        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));

        mCameraNode = mLookAtNode->createChildSceneNode("CameraNode");
        mCameraNode->attachObject(mCamera);
        mCameraNode->translate(Vector3(0, 0, mDesiredDistance), Node::TS_LOCAL);

        setup();
    }
    //------------------------------------------------------------------------

    GameController::~GameController()
    {
    }
    //------------------------------------------------------------------------

    void GameController::run(Real elapsedTime)
    {
        Vector3 translation = Vector3::ZERO;
        Real yaw = 0.0;

        calculateScalingFactors(elapsedTime);
        calculateCameraTranslation();
        calculateHeroTranslation(translation, yaw);
        updateAnimationState(translation);

        translation *= mMoveScale;

        // Runterfallen berücksichtigen.
        // Zuerst Fallgeschwindigkeit berechnen
        translation.y = translation.y - mFallSpeed * elapsedTime;
        mFallSpeed = mFallSpeed - PhysicsManager::getSingleton().getGravity().y * elapsedTime;

        translate(translation, Node::TS_LOCAL);

        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));
        mLookAtNode->_update(true, false);
        
        mCameraNode->setPosition(0, 0, mTargetDistance);
        mCameraNode->_update(true, false);
        mTargetDistance = mCameraNode->getPosition().z;
        
        Real camAdjustmentSpeed = elapsedTime * mMoveSpeed * 7;
        if (fabs(mTargetDistance - mDesiredDistance) < camAdjustmentSpeed)
            mTargetDistance = mDesiredDistance;
        else if (mTargetDistance < mDesiredDistance)
            mTargetDistance += camAdjustmentSpeed;
        else 
            mTargetDistance -= camAdjustmentSpeed;

        mControlNode->yaw(Degree(yaw));

        if (!InputManager::getSingleton().isCeguiActive())
        {
            updatePickedObject();
        }
    }
    //------------------------------------------------------------------------

    void GameController::calculateScalingFactors(Real timePassed)
    {
        if (timePassed == 0)
        {
            mMoveScale = 0.5;
            mRotScale = 0.1;
        }
        else
        {
            mMoveScale = mMoveSpeed * timePassed;
            mRotScale = mRotSpeed * timePassed;
        }
    }
    //------------------------------------------------------------------------

    void GameController::calculateCameraTranslation()
    {	
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();
                        
        mDesiredDistance -= im->getMouseRelativeZ() * 0.05;
        mDesiredDistance = mDesiredDistance > mMaxDistance ?
            mMaxDistance : mDesiredDistance;
        mDesiredDistance = mDesiredDistance < mMinDistance ?
            mMinDistance : mDesiredDistance;
            
        mTargetPitch -= Degree(im->getMouseRelativeY() * 0.13);
        mTargetPitch = mTargetPitch > mMaxPitch ?
            mMaxPitch : mTargetPitch;
        mTargetPitch = mTargetPitch < mMinPitch ?
            mMinPitch : mTargetPitch;        
        
    }
    //------------------------------------------------------------------------

    void GameController::calculateHeroTranslation(Ogre::Vector3& translation,
        Ogre::Real& yaw)
    {
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();

        yaw = -im->getMouseRelativeX() * 0.13;
        
        if (cmdmap->isMovementActive(TURN_LEFT))
            yaw += mRotScale*2;

        if (cmdmap->isMovementActive(TURN_RIGHT))
            yaw -= mRotScale*2;
        
        if (cmdmap->isMovementActive(MOVE_FORWARD))
            translation.z = -mMoveScale;

        if (cmdmap->isMovementActive(MOVE_BACKWARD))
            translation.z = mMoveScale;

        if (cmdmap->isMovementActive(MOVE_RIGHT))
            translation.x = mMoveScale;

        if (cmdmap->isMovementActive(MOVE_LEFT))
            translation.x = -mMoveScale;
            
        if (cmdmap->isMovementActive(MOVE_JUMP) && fabs(mFallSpeed) <= 0.1)
            mFallSpeed = -500;

        if (im->isKeyDown(KC_P))
            CoreSubsystem::getSingleton().makeScreenshot("rastullah");
            
        translation.normalise();
        if (cmdmap->isMovementActive(MOVE_RUN))
            translation *= 2;
    }
    //------------------------------------------------------------------------

    void GameController::updateAnimationState(const Vector3& translation)
    {
        mCurrentAnimationState =
            translation != Vector3::ZERO ? AS_WALK_FORWARD : AS_STAND;

        if (mCurrentAnimationState != mLastAnimationState)
        {
            //if (mCurrentAnimationState == AS_WALK_FORWARD)
            //{
            //    mActor->startAnimation("gehloop");
            //}
            //else
            //{
            //    mActor->stopAnimation("gehloop");
            //}
            //mLastAnimationState = mCurrentAnimationState;
        }
    }
    //------------------------------------------------------------------------

    Actor* GameController::getControlledActor()
    {
        return mActor;
    }
    //------------------------------------------------------------------------

    void GameController::setControlledActor(Actor* actor)
    {
        if (actor == 0)
        {
            Throw(NullPointerException, "actor darf nicht NULL sein.");
        }
        mActor = actor;
        setup();
    }
    //------------------------------------------------------------------------

    Ogre::Camera* GameController::getCamera()
    {
        return mCamera;
    }
    //------------------------------------------------------------------------

    void GameController::setup()
    {
        if (mActor != 0)
        {
            SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->getRootSceneNode();
            Vector3 pos = mActor->getPosition();

            // ControlNode auf etwa 10% Abstand bezogen auf die Höhe
            // des GameActors bringen.
            ///@todo Folgendes neumachen.
            /*
            pos.y = pos.y + extent.y * 0.9;
            mControlNode->setPosition(pos);
            mControlNode->addChild(mActor->getSceneNode());
            mActor->getSceneNode()->setPosition(Vector3::ZERO);
            mActor->getSceneNode()->translate(
                Vector3(0, -extent.y * 0.9, 0), Node::TS_PARENT);
            */
        }
    }
    //------------------------------------------------------------------------

    Vector3 GameController::ogrePosToOdePos(
        const Vector3& pos, const Vector3& extent)
    {
        return Vector3(pos.x, pos.y + extent.y / 2.0, pos.z);
    }
    //------------------------------------------------------------------------

    void GameController::setCamera(Ogre::Camera* camera)
    {
        if (camera == 0)
        {
            Throw(NullPointerException, "camera darf nicht NULL sein.");
        }
        mCameraNode->detachObject(mCamera);
        mCameraNode->attachObject(camera);
        mCamera = camera;
    }
    //------------------------------------------------------------------------

    void GameController::updatePickedObject() const
    {
        InputManager::getSingleton().updatePickedObject(0.5, 0.5);
    }
    //------------------------------------------------------------------------
    
    void GameController::translate(const Vector3& translation,
        Node::TransformSpace ts)
    {
        mControlNode->translate(translation, ts);
        mControlNode->_update(true, false);
    }
    //------------------------------------------------------------------------
    
    void GameController::setPosition(const Vector3& position)
    {
        ///@todo implementieren
    }
    //------------------------------------------------------------------------
    
    void GameController::setViewMode(ViewMode mode)
    {
        mViewMode = mode;
        if (mode == VM_FIRST_PERSON)
        {
            mActor->_getSceneNode()->setVisible(false);
            mDesiredDistance = 0.0;
        }
        else
        {
            mActor->_getSceneNode()->setVisible(true);
            mDesiredDistance = 150.0;
            resetCamera();
        }
    }
    //------------------------------------------------------------------------
    GameController::ViewMode GameController::getViewMode()
    {
        return mViewMode;
    }
    //------------------------------------------------------------------------
    
    void GameController::resetCamera()
    {
        if (mViewMode == VM_THIRD_PERSON)
        {
            mTargetPitch = Degree(-30.0);
            mTargetDistance = 0;
        }
    }
    //------------------------------------------------------------------------

}
