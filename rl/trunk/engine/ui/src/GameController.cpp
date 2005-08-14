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

#include "GameController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "Actor.h"
#include "MeshObject.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"
#include "MeshObject.h"
#include "ActorManager.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>


using namespace Ogre;

namespace rl {

    GameController::GameController()
    {
        // Must not be used.
        Throw(OperationNotSupportedException, "Do not use the standard constructor.");
    }
    
    GameController::GameController(Actor* cameraActor, Actor* actor)
        : mSceneManager(CoreSubsystem::getSingletonPtr()->
        getWorld()->getSceneManager()),
        mControlNode(0),
        mLookAtNode(0),
        mCameraActor(cameraActor),
        mActor(actor),
        mActorMesh(static_cast<MeshObject*>(actor->getControlledObject())),
        mMoveScale(0),
        mRotScale(0),
        mMoveSpeed(600.0),
        mRotSpeed(80.0),
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
        if (actor == 0 || cameraActor == 0)
        {
            Throw(NullPointerException, "actor und camera duerfen nicht NULL sein.");
        }

        SceneNode* root = mSceneManager->getRootSceneNode();

        mControlNode = root->createChildSceneNode("ControlNode");

        mLookAtNode = mControlNode->createChildSceneNode("LookAtNode");
        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));

        if (mCameraActor->_getSceneNode())
        {
            mCameraActor->removeFromScene();
        }
        mCameraActor->placeIntoNode(mLookAtNode);
        mCameraActor->translate(Vector3(0, 0, mDesiredDistance), Node::TS_LOCAL);
        
        setup();

		PhysicsManager::getSingleton().setActor(
			actor->getPhysicalThing()->getGeometry(), mControlNode);
		PhysicsManager::getSingleton().setCamera(
			cameraActor->getPhysicalThing()->getGeometry(), mCameraActor->_getSceneNode() );
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
        calculateActorTranslation(translation, yaw);
        updateAnimationState(translation);

        translation *= mMoveScale;

        // Runterfallen berücksichtigen.
        // Zuerst Fallgeschwindigkeit berechnen
		Ogre::Real fallSpeed = PhysicsManager::getSingleton().getFallSpeed();
		translation.y = translation.y - fallSpeed * elapsedTime;

		PhysicsManager::getSingleton().setFallSpeed(
			fallSpeed - 
			PhysicsManager::getSingleton().getWorld()->getGravity().y * elapsedTime * 20); //*200

        mControlNode->translate(translation, Node::TS_LOCAL);

        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));
        mLookAtNode->_update(true, false);
        
        mCameraActor->setPosition(0, 0, mTargetDistance);
        mTargetDistance = mCameraActor->getPosition().z;
        
        Real camAdjustmentSpeed = elapsedTime * mMoveSpeed * 7;
        if (fabs(mTargetDistance - mDesiredDistance) < camAdjustmentSpeed)
            mTargetDistance = mDesiredDistance;
        else if (mTargetDistance < mDesiredDistance)
            mTargetDistance += camAdjustmentSpeed;
        else 
            mTargetDistance -= camAdjustmentSpeed;

        mControlNode->yaw(Degree(yaw));
        mActor->_update();
        mCameraActor->_update();

        if (!InputManager::getSingleton().isCeguiActive())
        {
            updatePickedObject();
        }
        /*DebugWindow::getSingleton().setText(
            StringConverter::toString(mCameraActor->getWorldPosition()) +
            " / " +
            StringConverter::toString(mControlNode->getWorldPosition())+
            " / " +
            StringConverter::toString(mOdeLevel->getPosition()));*/
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
                        
        if (mViewMode != VM_FIRST_PERSON)
        {
            mDesiredDistance -= im->getMouseRelativeZ() * 0.05;
            mDesiredDistance = mDesiredDistance > mMaxDistance ?
                mMaxDistance : mDesiredDistance;
            mDesiredDistance = mDesiredDistance < mMinDistance ?
                mMinDistance : mDesiredDistance;
        }
            
        mTargetPitch -= Degree(im->getMouseRelativeY() * 0.13);
        mTargetPitch = mTargetPitch > mMaxPitch ?
            mMaxPitch : mTargetPitch;
        mTargetPitch = mTargetPitch < mMinPitch ?
            mMinPitch : mTargetPitch;        
        
    }
    //------------------------------------------------------------------------

    void GameController::calculateActorTranslation(Ogre::Vector3& translation,
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
            
		if (cmdmap->isMovementActive(MOVE_JUMP) && PhysicsManager::getSingleton().getFallSpeed() <= 0.1)
			PhysicsManager::getSingleton().setFallSpeed(-500);

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
            if (mCurrentAnimationState == AS_WALK_FORWARD)
            {
                mActorMesh->startAnimation("gehloop", 2.0, 0);
            }
            else
            {
                mActorMesh->stopAnimation("gehloop");
            }
            mLastAnimationState = mCurrentAnimationState;
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

    Actor* GameController::getCamera()
    {
        return mCameraActor;
    }
    //------------------------------------------------------------------------

    void GameController::setup()
    {
        if (mActor != 0)
        {
            MeshObject* mesh = reinterpret_cast<MeshObject*>(mActor->getControlledObject());
            Vector3 extent = mesh->getSize();
            SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->getRootSceneNode();
            Vector3 pos = mActor->getPosition();

            // ControlNode auf etwa 10% Abstand bezogen auf die Höhe
            // des GameActors bringen.
            pos.y = pos.y + extent.y * 0.9;
            mControlNode->setPosition(pos);
            if (mActor->_getSceneNode())
            {
                mActor->removeFromScene();
            }
            
            mActor->placeIntoNode(mControlNode);
            mActor->_getSceneNode()->translate(Vector3(0, -extent.y * 0.9, 0), Node::TS_PARENT);                    
        }
    }
    //------------------------------------------------------------------------

    Vector3 GameController::ogrePosToOdePos(
        const Vector3& pos, const Vector3& extent)
    {
        return Vector3(pos.x, pos.y + extent.y / 2.0, pos.z);
    }
    //------------------------------------------------------------------------

    void GameController::updatePickedObject() const
    {
        InputManager::getSingleton().updatePickedObject(0.5, 0.5);
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

	void GameController::toggleViewMode()
	{
		if (getViewMode() == VM_FIRST_PERSON)
			setViewMode(VM_THIRD_PERSON);
		else
			setViewMode(VM_FIRST_PERSON);
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
