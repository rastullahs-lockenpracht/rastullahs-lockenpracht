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

#include "stdinc.h"

#include "MovementControlState.h"

#include <utility>

#include "AbstractMovement.h"
#include "Actor.h"
#include "ActorManager.h"
#include "AnimationManager.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "CreatureControllerManager.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "LineSetPrimitive.h"
#include "Logger.h"
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "MovementControlState.h"
#include "PhysicalThing.h"
#include "PhysicsManager.h"
#include "PhysicsMaterialRaycast.h"
#include "RulesMessages.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

#include <numeric>

using namespace Ogre;

namespace rl
{

    Ogre::String MovementControlState::msDebugWindowPageName = "MovementControlState";

    MovementControlState::CharacterState::CharacterState()
        : mCurrentMovementState(MOVE_NONE)
        , mLastMovementState(MOVE_NONE)
    {
    }

    MovementControlState::MovementControlState(CommandMapper* cmdMapper, Actor* camera, Creature* character)
        : ControlState(cmdMapper, camera, character, CST_MOVEMENT)
        , mController(nullptr)
        , mCharacterState()
        , mDesiredDistance(2.00)
        , mDistanceRange(0.60, 7.00)
        , mCamYaw(0)
        , mCamVirtualYaw(0)
        , mNewCamVirtualYaw(0)
        , mCamRearViewYaw(0)
        , mPitch(20)
        , mRoll(0)
        , mPitchRange(Degree(-75), Degree(85))
        , mLinearSpringK(400.0f)
        , mLinearDampingK(Math::NEG_INFINITY)
        , mCamMoveAwayVelocity(4.0f)
        , mCamMoveAwayStartTime(0.25f)
        , mCamMoveAwayRange(8.0f)
        , mLookAtOffset()
        , mRotationSpeed(Degree(120.0f))
        , mMouseSensitivity(4.0f)
        , mViewMode(VM_THIRD_PERSON)
        , mRaycast(new PhysicsMaterialRaycast())
        , mConvexcast(new PhysicsMaterialConvexcast())
        , mSelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager())
        , mCombatSelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(), QUERYFLAG_CREATURE)
        , mCharPositionsBuffer(100)
        , mCharPositionsBufferIdx(-1)
        , mCharacterOccludedTime(0)
        , mCharacterOccludedFrameCount(0)
        , mLastDistance(0.0f)
        , mTimeOfLastCollision(0.0f)
        , mIsPathfinding(false)
        , mLastReachableBufPos(1)
        , mLastCameraCollision(0)
    {
        DebugWindow::getSingleton().registerPage(msDebugWindowPageName);

        mMouseSensitivity = ConfigurationManager::getSingleton().getIntSetting("Input", "Mouse Sensitivity");
        mInvertedMouse = ConfigurationManager::getSingleton().getBoolSetting("Input", "Mouse Invert");

        // The relationCoefficient determines the relation between spring accel in target direction
        // and damping in velocity direction. 1.0 means equilibrium is reached in optimal time
        // smaller 1.0 means spring accel is stronger and thus cam shoots over the target, resulting
        // in a damped ozillation before reaching equilibrium.
        // Values greater than 1.0 mean damping is stronger and thus camera takes a detour.
        Real relationCoefficient = 0.8f;
        mLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mLinearSpringK);

        // Offset for the look at point,
        // so the cam does look at the characters head instead of the feet.
        MeshObject* charMesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        AxisAlignedBox aabb = charMesh->getDefaultSize();

        // this will be recalculated in calculateOptimalCameraPosition
        mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);

        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mCombatSelector.setFilter(filter);

        mSelector.setFilter(new InSceneSelectionFilter());

        mMessageType_GameObjectsLoaded_Handler
            = MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectsLoaded>(
                boost::bind(&MovementControlState::updateAfterGameObjectLoading, this));

        mMessageType_SaveGameLoading_Handler
            = MessagePump::getSingleton().addMessageHandler<MessageType_SaveGameLoading>(
                boost::bind(&MovementControlState::beforeLoadingSaveGame, this));

        mCameraCastCollision = std::dynamic_pointer_cast<OgreNewt::ConvexCollision>(mCamBody->getCollision());
        RlAssert1(mCameraCastCollision);
    }

    //------------------------------------------------------------------------
    MovementControlState::~MovementControlState()
    {
        delete mCombatSelector.getFilter();
        mCombatSelector.setFilter(nullptr);
        delete mSelector.getFilter();
        mSelector.setFilter(nullptr);
        delete mRaycast;
        delete mConvexcast;

        if (DebugWindow::getSingletonPtr())
        {
            DebugWindow::getSingletonPtr()->unregisterPage(msDebugWindowPageName);
        }

        // Remove debug scene node from character node, if debugview was used.
        if (mSceneNode != nullptr && mSceneNode->getParent() != nullptr)
        {
            mCharacterActor->_getSceneNode()->removeChild(mSceneNode);
        }
    }

    //------------------------------------------------------------------------
    void MovementControlState::pause()
    {
        mController = nullptr;

        // actors aren't controlled anymore
        mCameraActor->getPhysicalThing()->setPhysicsController(nullptr);
        mCameraActor->getPhysicalThing()->freeze();
        // cam<->Level collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"));
        // cam<->Default collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"));
        PhysicsManager::getSingleton().resetMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"));

        // Unhighlight selected object, if any.
        GameObject* go = mSelector.getFirstSelectedObject();
        if (go != nullptr && go->isHighlighted())
        {
            go->setHighlighted(false);
        }
    }

    //------------------------------------------------------------------------
    void MovementControlState::resume()
    {
        if (mController == nullptr)
        {
            mController = CreatureControllerManager::getSingleton().getCreatureController(mCharacter);
        }
        mCharBody = mCharacterActor->getPhysicalThing()->_getBody();

        // We want to check for visibility from char's POV.
        mSelector.setCheckVisibility(true, mCharacter);
        mSelector.track(mCharacter);
        mSelector.setRadius(3.0);

        // Same for combat selector
        mCombatSelector.setCheckVisibility(true, mCharacter);
        mCombatSelector.track(mCharacter);
        mCombatSelector.setRadius(10.0);

        // control camera
        mCameraActor->getPhysicalThing()->setMaterialID(PhysicsManager::getSingleton().getMaterialID("camera"));
        mCameraActor->getPhysicalThing()->unfreeze();
        mCameraActor->getPhysicalThing()->setPhysicsController(this);

        // We also handle cam<->level, cam<->default cam<->char collision from now on
        OgreNewt::MaterialPair* mat_pair = nullptr;
        mat_pair
            = PhysicsManager::getSingleton().createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("default"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0, 0);
        mat_pair->setDefaultSoftness(0.8f);
        mat_pair->setDefaultElasticity(0.4f);
        mat_pair
            = PhysicsManager::getSingleton().createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("level"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0, 0);
        mat_pair->setDefaultSoftness(0.8f);
        mat_pair->setDefaultElasticity(0.4f);
        mat_pair
            = PhysicsManager::getSingleton().createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("character"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0, 0);
        mat_pair->setDefaultSoftness(0.8f);
        mat_pair->setDefaultElasticity(0.4f);

        mCharacterState.mCurrentMovementState = MOVE_NONE;

        setViewMode(VM_THIRD_PERSON);
    }

    //------------------------------------------------------------------------
    void MovementControlState::run(Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();

        updateCharacter(elapsedTime);
        updateCameraLookAt(elapsedTime);
        updateSelection();

        // camera pitch
        if (!isMouseUsedByCegui())
        {
            if (mInvertedMouse)
                mPitch -= 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
            else
                mPitch += 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
            if (mPitch < mPitchRange.first)
                mPitch = mPitchRange.first;
            if (mPitch > mPitchRange.second)
                mPitch = mPitchRange.second;
        }

        mCharacterState.mLastMovementState = mCharacterState.mCurrentMovementState;

        if (isEnemyNear() && !(mCharacter->getLifeState() & Effect::LS_NO_COMBAT))
        {
            InputManager::getSingleton().pushControlState(CST_COMBAT);
        }
    }

    //------------------------------------------------------------------------
    void MovementControlState::updateCharacter(Ogre::Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();
        int movement = mCharacterState.mCurrentMovementState;
        if (mController != nullptr)
        {
            Degree rotation(0);

            AbstractMovement* drehen = mController->getMovementFromId(CreatureController::MT_DREHEN);
            Real baseVelocity = 0;
            if (drehen->calculateBaseVelocity(baseVelocity))
            {
                if (!(movement & MOVE_RIGHT || movement & MOVE_LEFT))
                {
                    Degree baseVel(baseVelocity * 360);
                    if (mViewMode != VM_PNYX_MODE)
                    {
                        if (movement & TURN_LEFT)
                            rotation = elapsedTime * baseVel;
                        if (movement & TURN_RIGHT)
                            rotation = -elapsedTime * baseVel;
                    }

                    // mouse
                    if (!isMouseUsedByCegui())
                    {
                        if (mViewMode == VM_FIRST_PERSON || mViewMode == VM_THIRD_PERSON)
                        {
                            if (!(movement & TURN_LEFT || movement & TURN_RIGHT))
                            {
                                rotation
                                    = -mMouseSensitivity / 3.0f * Degree(im->getMouseRelativeX()) / 200.0 * baseVel;
                            }
                        }
                    }
                }

                if (mViewMode != VM_PNYX_MODE) //&& mViewMode != VM_FIRST_PERSON )
                {
                    // virtual yaw
                    if (mCamVirtualYaw != mNewCamVirtualYaw)
                    {
                        mCamVirtualYaw = mNewCamVirtualYaw;
                    }
                    if (((movement & MOVE_FORWARD) && (movement & MOVE_RIGHT) && !(movement & MOVE_LEFT))
                        || ((movement & MOVE_BACKWARD) && (movement & MOVE_LEFT) && !(movement & MOVE_RIGHT)))
                    {
                        mNewCamVirtualYaw = Degree(45);
                    }
                    else if (((movement & MOVE_FORWARD) && (movement & MOVE_LEFT) && !(movement & MOVE_RIGHT))
                        || ((movement & MOVE_BACKWARD) && (movement & MOVE_RIGHT) && !(movement & MOVE_LEFT)))
                    {
                        mNewCamVirtualYaw = Degree(-45);
                    }
                    else
                    {
                        mNewCamVirtualYaw = Degree(0);
                    }
                    if (mCamVirtualYaw != mNewCamVirtualYaw)
                    {
                        rotation += mCamVirtualYaw - mNewCamVirtualYaw;
                    }
                }
                /*
                                if( mViewMode == VM_FIRST_PERSON )
                                {
                                    if( ((movement & MOVE_FORWARD) && (movement & MOVE_RIGHT) && !(movement &
                   MOVE_LEFT)) ||
                                        ((movement & MOVE_BACKWARD) && (movement & MOVE_RIGHT) && !(movement &
                   MOVE_LEFT)) )
                                    {
                                        mCamVirtualYaw -= Degree(270)*elapsedTime;
                                        if( mCamVirtualYaw <= Degree(-90) )
                                            mCamVirtualYaw = Degree(-90);
                                    }
                                    else if( ((movement & MOVE_FORWARD) && (movement & MOVE_LEFT) && !(movement &
                   MOVE_RIGHT)) ||
                                             ((movement & MOVE_BACKWARD) && (movement & MOVE_LEFT) && !(movement &
                   MOVE_RIGHT)) )
                                    {
                                        mCamVirtualYaw += Degree(270)*elapsedTime;
                                        if( mCamVirtualYaw >= Degree(90) )
                                            mCamVirtualYaw = Degree(90);
                                    }
                                    else
                                    {
                                        if( mCamVirtualYaw > Degree(0) )
                                        {
                                            mCamVirtualYaw -= Degree(270)*elapsedTime;
                                            if( mCamVirtualYaw <= Degree(0) )
                                                mCamVirtualYaw = Degree(0);
                                        }
                                        else if( mCamVirtualYaw < Degree(0) )
                                        {
                                            mCamVirtualYaw += Degree(270)*elapsedTime;
                                            if( mCamVirtualYaw >= Degree(0) )
                                                mCamVirtualYaw = Degree(0);
                                        }
                                    }
                                }
                */
            }

            if (mViewMode != VM_PNYX_MODE)
            {
                if (mController->getMovementId() == CreatureController::MT_HOCHSPRUNG)
                {
                    // move forward or backward if wanted
                    Vector3 direction = Vector3::UNIT_Y;
                    if (movement & MOVE_FORWARD)
                        direction += Vector3::NEGATIVE_UNIT_Z;
                    else if (movement & MOVE_BACKWARD)
                        direction += Vector3::UNIT_Z;

                    mController->setMovement(
                        CreatureController::MT_HOCHSPRUNG, direction, Vector3(0, rotation.valueRadians(), 0));
                }
                else if (movement & MOVE_SNEAK)
                {
                    Vector3 direction(Vector3::ZERO);
                    if (movement & MOVE_FORWARD)
                        direction.z = -1;
                    else if (movement & MOVE_BACKWARD)
                        direction.z = 1;
                    mController->setMovement(
                        CreatureController::MT_SCHLEICHEN, direction, Vector3(0, rotation.valueRadians(), 0));
                }
                else if (movement & MOVE_JUMP
                    && mController->getMovementFromId(CreatureController::MT_HOCHSPRUNG)->isPossible())
                {
                    CreatureController::MovementType type = CreatureController::MT_HOCHSPRUNG;
                    Vector3 direction = Vector3::UNIT_Y;
                    if (movement & MOVE_FORWARD)
                    {
                        type = CreatureController::MT_WEITSPRUNG;
                        direction += Vector3::NEGATIVE_UNIT_Z;
                    }
                    mController->setMovement(type, direction, Vector3(0, rotation.valueRadians(), 0));
                }
                else if (movement & MOVE_FORWARD)
                {
                    CreatureController::MovementType type = CreatureController::MT_GEHEN;
                    if (movement & MOVE_RUN_LOCK)
                    {
                        if (movement & MOVE_RUN)
                            type = CreatureController::MT_RENNEN;
                        else
                            type = CreatureController::MT_LAUFEN;
                    }
                    else
                    {
                        if (movement & MOVE_RUN)
                            type = CreatureController::MT_GEHEN;
                        else
                            type = CreatureController::MT_JOGGEN;
                    }
                    mController->setMovement(type, Vector3(0, 0, -1), Vector3(0, rotation.valueRadians(), 0));
                }
                else if (movement & MOVE_BACKWARD)
                {
                    CreatureController::MovementType type = CreatureController::MT_RUECKWAERTS_GEHEN;
                    if (!(movement & MOVE_RUN))
                        type = CreatureController::MT_RUECKWAERTS_JOGGEN;
                    mController->setMovement(type, Vector3(0, 0, 1), Vector3(0, rotation.valueRadians(), 0));
                }
                else if (movement & MOVE_LEFT || movement & MOVE_RIGHT)
                {
                    Vector3 direction = Vector3::UNIT_X;
                    if (movement & MOVE_LEFT)
                        direction = Vector3::NEGATIVE_UNIT_X;
                    mController->setMovement(
                        CreatureController::MT_SEITWAERTS_GEHEN, direction, Vector3(0, rotation.valueRadians(), 0));
                }
                else
                {
                    mController->setMovement(
                        CreatureController::MT_STEHEN, Vector3(0, 0, 0), Vector3(0, rotation.valueRadians(), 0));
                }
            }
            else // VM_PNYX_MODE
            {
                // turn to the direction entered
                if (movement & MOVE_FORWARD || movement & MOVE_BACKWARD || movement & MOVE_LEFT
                    || movement & MOVE_RIGHT)
                {
                    // direction to turn to
                    int direction = movement & (MOVE_FORWARD | MOVE_BACKWARD | MOVE_RIGHT | MOVE_LEFT);
                    Degree yaw(0);
                    switch (direction)
                    {
                    case MOVE_FORWARD:
                        yaw = Degree(0);
                        break;
                    case MOVE_FORWARD | MOVE_LEFT:
                        yaw = Degree(45);
                        break;
                    case MOVE_FORWARD | MOVE_RIGHT:
                        yaw = Degree(-45);
                        break;
                    case MOVE_RIGHT:
                        yaw = Degree(-90);
                        break;
                    case MOVE_LEFT:
                        yaw = Degree(90);
                        break;
                    case MOVE_BACKWARD:
                        yaw = Degree(180);
                        break;
                    case MOVE_BACKWARD | MOVE_LEFT:
                        yaw = Degree(-225);
                        break;
                    case MOVE_BACKWARD | MOVE_RIGHT:
                        yaw = Degree(225);
                        break;
                    default:
                        break;
                    }
                    yaw += mCamYaw;

                    CreatureController::MovementType type = CreatureController::MT_JOGGEN;
                    if (movement & MOVE_SNEAK)
                        type = CreatureController::MT_SCHLEICHEN;
                    else
                    {
                        if (movement & MOVE_JUMP)
                            type = CreatureController::MT_WEITSPRUNG;
                        else
                        {
                            switch (movement & (MOVE_RUN | MOVE_RUN_LOCK))
                            {
                            case MOVE_RUN:
                                type = CreatureController::MT_GEHEN;
                                break;
                            case MOVE_RUN_LOCK:
                                type = CreatureController::MT_LAUFEN;
                                break;
                            case MOVE_RUN | MOVE_RUN_LOCK:
                                type = CreatureController::MT_RENNEN;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    mController->setMovement(
                        type, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_Y * (yaw - mController->getYaw()).valueRadians());
                }
                else
                {
                    // don't move
                    CreatureController::MovementType type = CreatureController::MT_STEHEN;
                    if (movement & MOVE_SNEAK)
                        type = CreatureController::MT_SCHLEICHEN;
                    else if (movement & MOVE_JUMP)
                        type = CreatureController::MT_HOCHSPRUNG;
                    mController->setMovement(type, Vector3::ZERO, Vector3::ZERO);
                }
            }
        }

        if (movement & REAR_VIEW)
        {
            if (mCamRearViewYaw < Degree(180))
                mCamRearViewYaw += elapsedTime * Degree(2 * 360);
            if (mCamRearViewYaw > Degree(180))
                mCamRearViewYaw = Degree(180);
        }
        else
        {
            if (mCamRearViewYaw > Degree(0))
                mCamRearViewYaw -= elapsedTime * Degree(2 * 360);
            if (mCamRearViewYaw < Degree(0))
                mCamRearViewYaw = Degree(0);
        }
    }

    // ------------------------------------------------------------------------
    void MovementControlState::updateCameraLookAt(Ogre::Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();

        // camera position (distance)
        if (!isMouseUsedByCegui())
        {
            mDesiredDistance -= im->getMouseRelativeZ() * 0.002;
            if (mDesiredDistance < mDistanceRange.first)
            {
                mDesiredDistance = mDistanceRange.first;
            }
            if (mDesiredDistance > mDistanceRange.second)
            {
                mDesiredDistance = mDistanceRange.second;
            }

            if (mViewMode == VM_FREE_CAMERA || mViewMode == VM_PNYX_MODE)
            {
                mCamYaw -= 2 * mMouseSensitivity / 4.0 * mRotationSpeed * Degree(im->getMouseRelativeX() / 15);

                while (mCamYaw.valueDegrees() > 360.0f)
                    mCamYaw -= Degree(360.0f);
                while (mCamYaw.valueDegrees() < -360.0f)
                    mCamYaw += Degree(360.0f);
            }
        }

        SceneNode* cameraNode = mCameraActor->_getSceneNode();

        Vector3 charPos;
        charPos = mCharacter->getActor()->getWorldPosition();
        Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw + mCamRearViewYaw, Vector3::UNIT_Y);

        // Kamera-Gr�e beziehen
        CameraObject* ogreCam = static_cast<CameraObject*>(mCameraActor->getControlledObject());
        AxisAlignedBox aabb = ogreCam->getDefaultSize();
        // Radius berechnen
        Real radius = (aabb.getMaximum() - aabb.getMinimum()).length() / 2.0f;

        if (mViewMode == VM_FIRST_PERSON)
        {
            Quaternion camOri;
            camOri.FromAngleAxis(mPitch, Vector3::NEGATIVE_UNIT_X);
            cameraNode->lookAt(charPos + charOri * virtualCamOri * mLookAtOffset
                    + charOri * camOri * virtualCamOri * (-Vector3::UNIT_Z),
                Node::TS_WORLD);
        }
        else if (mViewMode == VM_THIRD_PERSON)
        {
            cameraNode->lookAt(charPos + charOri * virtualCamOri * mLookAtOffset
                    + charOri * virtualCamOri
                        * (-Vector3::UNIT_Z * radius), // doesn't work smoothly with strafe+forward
                Node::TS_WORLD);
        }
        else if (mViewMode == VM_FREE_CAMERA || mViewMode == VM_PNYX_MODE)
        {
            Quaternion camOri;
            camOri.FromAngleAxis(mCamYaw, Vector3::UNIT_Y);
            Real dist = (mCameraActor->getPosition() - charPos).length();
            cameraNode->lookAt(charPos + camOri * virtualCamOri * mLookAtOffset + camOri * (-Vector3::UNIT_Z * radius),
                Node::TS_WORLD);
        }

        // Character ausblenden, wenn Kamera zu nah.
        if (mViewMode != VM_FIRST_PERSON)
        {
            // here the real charOri of the object is needed
            Vector3 charPos;
            Quaternion charOri;
            mCharBody->getPositionOrientation(charPos, charOri);
            Vector3 camPos;
            Quaternion camOri;
            mCamBody->getPositionOrientation(camPos, camOri);
            Vector3 camPoint, charPoint, normal;
            int collisionPoints
                = OgreNewt::CollisionTools::CollisionClosestPoint(PhysicsManager::getSingleton()._getNewtonWorld(),
                    mCamBody->getCollision(), camOri, camPos, mCharBody->getCollision(), charOri, charPos, camPoint,
                    charPoint, normal, 0 // set threadindex to 0, I hope this is ok!
                );
            if (collisionPoints == 0)
                mCharacterActor->setVisible(false);
            else
            {
                // eigentlich muss hier transparent gemacht werden!
                mCharacterActor->setVisible(true);
            }
        }
        mCameraActor->setOrientation(cameraNode->getOrientation());
    }

    int MovementControlState::onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex)
    {
        if (mViewMode == VM_FIRST_PERSON)
            return 0;

        // test if this is cam-player-collide
        if ((body0 == mCamBody && body1 == mCharacterActor->getPhysicalThing()->_getBody())
            || (body1 == mCamBody && body0 == mCharacterActor->getPhysicalThing()->_getBody()))
        {
            return 0;
        }

        return 1;
    }

    void MovementControlState::userProcess(OgreNewt::ContactJoint& contactJoint, Real timestep, int)
    {
        mLastCameraCollision = 0;
    }

    //------------------------------------------------------------------------
    // character callback moved to CreatureController
    void MovementControlState::OnApplyForceAndTorque(PhysicalThing* thing, float timestep)
    {
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

        calculateCamera(timestep);

        ///@todo move to CreatureController?
        SceneNode* node = mCharacterActor->_getSceneNode();
        std::ostringstream ss;
        Vector3 bodpos, playpos = node->getPosition();
        Quaternion egal;
        mCamBody->getPositionOrientation(bodpos, egal);
        ss << "scene node : " << playpos << std::endl
           << "player velocity : " << -mController->getVelocity().z << std::endl
           << "player orientation : " << mController->getCreature()->getActor()->getOrientation() << std::endl
           << "camera posder : " << static_cast<Camera*>(mCameraActor->_getMovableObject())->getDerivedPosition()
           << std::endl
           << "camera orientation : " << mCameraActor->getWorldOrientation() << std::endl
           << "camera pos : " << bodpos << std::endl
           << "camera distance : " << mLastDistance << " ( " << mDesiredDistance << " ) " << std::endl
           << "is airborne: "
           << (mController->getAbstractLocation() == CreatureController::AL_AIRBORNE ? "true" : "false") << std::endl;

        LOG_DEBUG(Logger::UI, ss.str());
        DebugWindow::getSingleton().setPageText(msDebugWindowPageName, ss.str());
    }

    //------------------------------------------------------------------------
    void MovementControlState::calculateCamera(const Ogre::Real& timestep)
    {
        mLastCameraCollision += timestep;
        Vector3 charPos = mCharacter->getActor()->getWorldPosition();
        Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw + mCamRearViewYaw, Vector3::UNIT_Y);

        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);
        SceneNode* cameraNode = mCameraActor->_getSceneNode();

        Vector3 optimalCamPos = calculateOptimalCameraPosition(true, timestep);
        charPos = charPos + charOri * virtualCamOri * mLookAtOffset;

        // Ringbuffer mit Positionen des Characters
        mCharPositionsBufferIdx = (mCharPositionsBufferIdx + 1) % mCharPositionsBuffer.size();
        mCharPositionsBuffer[mCharPositionsBufferIdx] = charPos;

        // Kamera-Gr�e beziehen
        CameraObject* ogreCam = static_cast<CameraObject*>(mCameraActor->getControlledObject());
        AxisAlignedBox camAabb = ogreCam->getDefaultSize();
        // Radius berechnen
        Real camRadius = (camAabb.getMaximum().z - camAabb.getMinimum().z) / 2.0f;

        if (mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA || mViewMode == VM_PNYX_MODE)
        {

            // wir machen ein paar Raycasts um herauszufinden, ob wir von der jetzigen Position
            // so zur optimalen kommen
            const OgreNewt::MaterialID* charMaterialId = mCharBody->getMaterialGroupID();
            const OgreNewt::MaterialID* camMaterialId = mCamBody->getMaterialGroupID();
            PhysicsMaterialRaycast::MaterialVector materialVector;
            materialVector.push_back(charMaterialId);
            materialVector.push_back(camMaterialId);
            //                PhysicsManager::getSingleton()._getLevelMaterialID();
            OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

            Vector3 normToOptCamPos = (optimalCamPos - charPos);
            normToOptCamPos.normalise();

            RaycastInfo infoCastOptPos = mRaycast->execute(world, &materialVector,
                camPos + camRadius * normToOptCamPos, // Gr�e der Kamera einbeziehen
                optimalCamPos + camRadius * normToOptCamPos,
                true); // Gr�e der Kamera einbeziehen

            RaycastInfo infoCastChar = mRaycast->execute(world, &materialVector, camPos, charPos, true);

            // reset the camera if the character is to far away or cannot be seen any more (Raycast)
            Real maxdistance = Math::Pow(1.5f * mDesiredDistance + 1.4f, 2);
            if (infoCastChar.mBody || (camPos - charPos).squaredLength() > maxdistance)
            {
                mCharacterOccludedTime += timestep;
                mCharacterOccludedFrameCount++;

                // falls zu lange, Kamera resetten:
                if (mCharacterOccludedTime > 0.250f && mCharacterOccludedFrameCount > 5)
                {
                    resetCamera();
                    return;
                }
            }
            else
            {
                mCharacterOccludedTime = 0;
                mCharacterOccludedFrameCount = 0;
            }

            if (infoCastOptPos.mBody && !infoCastChar.mBody)
            {
                // andere Position ermitteln, die ziwschen optimaler und Character liegt
                // und erreichbar ist
                Real lenToOptCamPos = (optimalCamPos - charPos).length();

                RaycastInfo infoCastNewPos;
                Real delta = lenToOptCamPos / 2.0f;
                Vector3 temp = charPos + delta * normToOptCamPos;
                // Annaeherung in Schritten, an den Punkt, der von der aktuellen Position aus erreicht werden kann!
                while (delta > 0.05) // genauigkeit des gefundenen Punktes
                {
                    infoCastNewPos = mRaycast->execute(world, &materialVector,
                        camPos + camRadius * normToOptCamPos, // Groesse der Kamera!
                        temp, true);
                    delta = delta / 2.0f;
                    if (infoCastNewPos.mBody) // Hindernis gefunden, naeher an Char ran
                    {
                        temp = temp - delta * normToOptCamPos;
                    }
                    else // kein Hindernis gefunden, weiter von Char weg
                    {
                        temp = temp + delta * normToOptCamPos;
                    }
                }

                // Jetzt koennen wir sicher sein, dass diese Stelle erreichbar ist:
                temp = temp - 0.05 * normToOptCamPos;
                // Groesse der Kamera einbeziehen
                optimalCamPos = temp - camRadius * normToOptCamPos;
                // so ab hier kann ganz normal weiter gerechnet werden!
            }

            // gibt an, ob schon gebufferte Daten fuer den
            // neuen Weg existieren und dort weitergemacht werden kann,
            // oder ob neu nach einem Weg gesucht werden muss!
            if (infoCastChar.mBody && infoCastOptPos.mBody) // neue Position und Character nicht erreichbar
            {
                // anderen Weg finden
                // hier werden erstmal nur alte Player-Positionen betrachtet
                // es wird davon ausgegangen, dass diese "nah" genug aneinanderliegen
                // und durch "Geraden" miteinander verbunden werden koennen
                // durch das spring-Acc-Damping System sollten die Bewegungen trotzdem fluessig
                // und weich (keine scharfen Kurven) erscheinen

                size_t buffSize = mCharPositionsBuffer.size();

                if (!mIsPathfinding)
                {
                    LOG_DEBUG(Logger::UI, " Pathfinding der Kamera sollte jetzt anfangen!");

                    // letzte Character - Position suchen, die erreichbar ist...
                    // Ist vermutlicherweise ja die letzte, davor war ja noch alles ok!
                    unsigned int delta = 1;
                    while (delta < buffSize)
                    {
                        RaycastInfo info = mRaycast->execute(world, &materialVector, camPos,
                            mCharPositionsBuffer[(mCharPositionsBufferIdx - delta) % buffSize], true);

                        if (!info.mBody)
                            break;
                        delta++;
                    }
                    if (delta >= buffSize)
                    {
                        // is wohl irgendwas schiefgegangen!
                        LOG_MESSAGE(Logger::UI,
                            " Der Ringbuffer mit den Player-Positionen scheint zu klein zu sein; Pathfinding der "
                            "Kamera fehlgeschlagen! ");
                        mIsPathfinding = false;
                        resetCamera();
                        return;
                    }
                    mLastReachableBufPos = delta;

                    // auf zu der ermittelten Position!
                    optimalCamPos = mCharPositionsBuffer[(mCharPositionsBufferIdx - mLastReachableBufPos) % buffSize];
                }
                else
                {
                    LOG_DEBUG(Logger::UI, " Pathfinding der Kamera sollte weitergefhrt werden!");

                    // suche von lastReachableBufPos aus der letzten Frame nach neuen erreichbaren Buffer-Positionen
                    unsigned int delta = mLastReachableBufPos; // das ist die von der letzten Frame!
                    while (delta > 0) // delta = 0 braucht nicht ueberprft zu werden, wurde oben schon ausgeschlossen!
                    {
                        RaycastInfo info = mRaycast->execute(world, &materialVector, camPos,
                            mCharPositionsBuffer[(mCharPositionsBufferIdx - delta) % buffSize], true);

                        if (info.mBody)
                            break;
                        delta--;
                    }
                    mLastReachableBufPos = delta + 1;

                    // auf zu der ermittelten Position!
                    optimalCamPos = mCharPositionsBuffer[(mCharPositionsBufferIdx - mLastReachableBufPos) % buffSize];
                }
                mIsPathfinding = true; // so zum Testen noch keine Optimierung (doppelte Prfung gleicher sachen)
            }
            else
            {
                mIsPathfinding = false;
            }

            Vector3 diff = camPos - optimalCamPos;

            Vector3 cameraVelocity;
            cameraVelocity = mCamBody->getVelocity();
            // spring velocity
            Vector3 springAcc = -mLinearSpringK * diff - mLinearDampingK * cameraVelocity;

            // get the camera mass
            Real mass;
            Vector3 inertia;
            mCamBody->getMassMatrix(mass, inertia);

            // mCamBody->setPositionOrientation(newCamPos, camOri);
            mCamBody->setForce(springAcc * mass);
        }
        else if (mViewMode == VM_FIRST_PERSON)
        {
            mCamBody->setPositionOrientation(optimalCamPos, camOri);
        }
    }

    //------------------------------------------------------------------------
    Ogre::Vector3 MovementControlState::calculateOptimalCameraPosition(bool slowlyMoveBackward, const Real& timestep)
    {
        Vector3 targetCamPos;

        Vector3 charPos = mCharacter->getActor()->getWorldPosition();
        // Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion charOri(mController->getYaw(), Vector3::UNIT_Y);
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw + mCamRearViewYaw, Vector3::UNIT_Y);
        // get camera size
        CameraObject* ogreCam = static_cast<CameraObject*>(mCameraActor->getControlledObject());
        AxisAlignedBox camAabb = ogreCam->getDefaultSize();
        Real camRadius = (camAabb.getMaximum().z - camAabb.getMinimum().z) / 2.0f;

        if (mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA || mViewMode == VM_PNYX_MODE)
        {
            charPos = charPos + charOri * mLookAtOffset;
            if (mViewMode == VM_PNYX_MODE)
            {
                Quaternion camOri;
                camOri.FromAngleAxis(mCamYaw, Vector3::UNIT_Y);
                targetCamPos = charPos
                    + camOri * virtualCamOri
                        * Vector3(0, Math::Sin(mPitch) * mDesiredDistance, Math::Cos(mPitch) * mDesiredDistance);
            }
            else if (mViewMode == VM_THIRD_PERSON)
            {
                targetCamPos = charPos
                    + charOri * virtualCamOri
                        * Vector3(0, Math::Sin(mPitch) * mDesiredDistance, Math::Cos(mPitch) * mDesiredDistance);
            }
            else
            {
                Quaternion camOri;
                camOri.FromAngleAxis(mCamYaw, Vector3::UNIT_Y);
                targetCamPos = charPos
                    + charOri * camOri * virtualCamOri
                        * Vector3(0, Math::Sin(mPitch) * mDesiredDistance, Math::Cos(mPitch) * mDesiredDistance);
            }

            Vector3 diff = targetCamPos - charPos;

            // Convexcast
            PhysicsMaterialConvexcast::MaterialVector materialVector;
            materialVector.push_back(mCharBody->getMaterialGroupID());
            materialVector.push_back(mCamBody->getMaterialGroupID());
            OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

            ConvexcastInfo info = mConvexcast->execute(
                world, &materialVector, mCameraCastCollision, charPos, Quaternion::IDENTITY, targetCamPos, true);

            bool CollisionFound = false;
            if (info.mBody)
            {
                CollisionFound = true;
                Real hitBodyVel = info.mBody->getVelocity().dotProduct(diff.normalisedCopy());
                hitBodyVel = std::min(0.0f, hitBodyVel); // if the body moves, try to avoid it
                Real dist = std::max(info.mDistance + (hitBodyVel * timestep - 0.01) / diff.length(), 0.0);
                diff *= dist;

                mLastCameraCollision = 0;
            }

            // Langsames Entfernen vom Char:
            if (CollisionFound)
                mTimeOfLastCollision = 0.0f;
            else
                mTimeOfLastCollision += timestep;

            Real desiredDistance = diff.length();
            Vector3 camPos;
            Quaternion camOri;
            mCamBody->getPositionOrientation(camPos, camOri);

            if (slowlyMoveBackward && desiredDistance > mLastDistance)
            {

                diff.normalise();
                Real newDistance;
                Vector3 actDiff = camPos - charPos;
                actDiff.normalise();

                if (mLastCameraCollision <= 0.5) // there was a cam collision 0.5 seconds ago
                {
                    newDistance = mLastDistance;
                }
                else if (mTimeOfLastCollision > mCamMoveAwayStartTime
                    || diff.directionEquals(actDiff, mCamMoveAwayRange * timestep))
                    newDistance = mLastDistance + mCamMoveAwayVelocity * timestep;
                else
                    newDistance = mLastDistance;

                if (newDistance > desiredDistance)
                    newDistance = desiredDistance;

                diff = diff * newDistance;

                mLastDistance = newDistance;
            }
            else
                mLastDistance = desiredDistance;

            targetCamPos = charPos + diff;
        }
        else // FIRST_PERSON
        {
            // determine the optimal target position of the camera
            targetCamPos = charPos + charOri * virtualCamOri * mLookAtOffset
                + charOri * virtualCamOri
                    * Vector3(0, Math::Sin(mPitch) * mDesiredDistance, Math::Cos(mPitch) * mDesiredDistance);
        }

        return targetCamPos;
    }

    //------------------------------------------------------------------------
    bool MovementControlState::isEnemyNear()
    {
        mCombatSelector.updateSelection();

        const GameObjectList& gov = mCombatSelector.getAllSelectedObjects();
        for (GameObjectList::const_iterator it = gov.begin(), end = gov.end(); it != end; ++it)
        {
            Creature* creature = dynamic_cast<Creature*>(*it);
            if (creature && creature->getAlignment() == Creature::ALIGNMENT_ENEMY
                && (creature->getLifeState() & Effect::LS_NO_COMBAT) == 0)
            {
                return true;
            }
        }

        return false;
    }

    //------------------------------------------------------------------------
    void MovementControlState::updateSelection()
    {
        if (isMouseUsedByCegui())
            return;

        InputManager* im = InputManager::getSingletonPtr();

        GameObject* oldGo = mSelector.getFirstSelectedObject();

        mSelector.updateSelection();

        GameObject* newGo = mSelector.getFirstSelectedObject();

        if (oldGo != nullptr && oldGo != newGo)
        {
            oldGo->setHighlighted(false);
        }

        if (newGo != nullptr && newGo != oldGo)
        {
            newGo->setHighlighted(true);
        }
        /*
                // Optionen anzeigen
                if (im->isMouseButtonDown(OIS::MB_Right) && newGo != nullptr)
                {
                    WindowFactory::getSingleton().showActionChoice(newGo);
                }
                else if (im->isMouseButtonDown(OIS::MB_Left) && newGo != nullptr)
                {
                    newGo->doDefaultAction(mCharacter, nullptr);
                }
        */
    }

    void MovementControlState::setViewMode(ViewMode mode)
    {
        mViewMode = mode;

        MeshObject* charMesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        AxisAlignedBox aabb;
        try
        {
            aabb = charMesh->getPoseSize(mCharacter->getAnimation("stehen").first);
        }
        catch (...)
        {
            aabb = charMesh->getDefaultSize();
        }
        if (mode == VM_FIRST_PERSON)
        {
            mLookAtOffset = Vector3(0, (aabb.getMaximum().y - aabb.getMinimum().y) * 0.90f,
                (aabb.getMaximum().z - aabb.getMinimum().z) * (-0.3f));
            mDistanceRange.first = 0.0;
            mDistanceRange.second = 0.0;
            mDesiredDistance = 0.0;
            mPitchRange.first = Degree(-85);
            mPitchRange.second = Degree(85);
            mPitch = 0;
            LOG_MESSAGE(Logger::UI, "Switch to 1st person view");
            resetCamera();
        }
        else if (mode == VM_THIRD_PERSON)
        {
            mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.90f, 0);
            mDistanceRange.first = 0.60;
            mDistanceRange.second = 7.00;
            mDesiredDistance = 2.0;
            mPitchRange.first = Degree(-75);
            mPitchRange.second = Degree(85);
            mPitch = Degree(30);
            LOG_MESSAGE(Logger::UI, "Switch to 3rd person view");
            resetCamera();
        }
        else if (mode == VM_FREE_CAMERA)
        {
            mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.80f, 0);
            mDistanceRange.first = 0.60;
            mDistanceRange.second = 7.00;
            mDesiredDistance = 2.0;
            mPitchRange.first = Degree(-75);
            mPitchRange.second = Degree(85);
            mPitch = Degree(30);
            mCamYaw = mCharacter->getActor()->getWorldOrientation().getYaw();
            LOG_MESSAGE(Logger::UI, "Switch to free camera view");
            resetCamera();
        }
        else // mode == VM_PNYX_MODE
        {
            mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.80f, 0);
            mDistanceRange.first = 0.60;
            mDistanceRange.second = 7.00;
            mDesiredDistance = 2.5;
            mPitchRange.first = Degree(-75);
            mPitchRange.second = Degree(85);
            mPitch = Degree(30);
            mCamYaw = mCharacter->getActor()->getWorldOrientation().getYaw();
            LOG_MESSAGE(Logger::UI, "Switch to pnyx mode movementcontroller");
            resetCamera();
        }
    }

    //------------------------------------------------------------------------
    /*
        // not used at the moment!
        void MovementControlState::interpolateAnimationLookAtOffset(std::string actAnim, std::string newAnim, Ogre::Real
       factor)
        {
            AxisAlignedBox aab;
            Vector3 size[2];
            Vector3 interpolatedSize;


            // Die Gr�e der beiden Animationen abfragen
            MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
            aab = mesh->getPoseSize(actAnim);
            size[0] = aab.getMaximum() - aab.getMinimum();

            aab = mesh->getPoseSize(newAnim);
            size[1] = aab.getMaximum() - aab.getMinimum();

            // interpolierte Gr�e (linear) berechnen
            interpolatedSize = size[0] + factor*(size[1] - size[0]);

            // LookAtOffset berechnen!
            switch(mViewMode)
            {
            case VM_FIRST_PERSON:
                mLookAtOffset = Vector3(0, interpolatedSize.y * 0.90f, interpolatedSize.z * (-0.3f) );
                break;
            case VM_THIRD_PERSON:
                mLookAtOffset = Vector3(0, interpolatedSize.y * 0.90f, 0);
                break;
            case VM_FREE_CAMERA:
            default:
                mLookAtOffset = Vector3(0, interpolatedSize.y * 0.80f, 0);
            }
        }
    */

    //------------------------------------------------------------------------
    MovementControlState::ViewMode MovementControlState::getViewMode()
    {
        return mViewMode;
    }

    //------------------------------------------------------------------------
    void MovementControlState::toggleViewMode()
    {
        if (getViewMode() == VM_THIRD_PERSON)
            setViewMode(VM_FIRST_PERSON);
        else if (getViewMode() == VM_FIRST_PERSON)
            setViewMode(VM_FREE_CAMERA);
        else if (getViewMode() == VM_FREE_CAMERA)
            setViewMode(VM_PNYX_MODE);
        else
            setViewMode(VM_THIRD_PERSON);
    }

    //------------------------------------------------------------------------
    void MovementControlState::resetCamera()
    {
        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);
        mCamBody->setPositionOrientation(calculateOptimalCameraPosition(false, 0.0f), camOri);
        mCamVirtualYaw = Degree(0);
        mNewCamVirtualYaw = Degree(0);
        mCamRearViewYaw = Degree(0);
        mLastCameraCollision = 0;
        if (mViewMode == VM_FIRST_PERSON)
            mCharacterActor->setVisible(false);
        else
            mCharacterActor->setVisible(true);

        LOG_MESSAGE(Logger::UI, "Camera resetted.");
    }

    //------------------------------------------------------------------------
    bool MovementControlState::keyPressed(const OIS::KeyEvent& evt, bool handled)
    {
        bool retval = false;
        if (!handled)
        {
            int code = CommandMapper::encodeKey(evt.key, InputManager::getSingleton().getModifierCode());
            // First see, if a control state action is defined
            CeGuiString command = mCommandMapper->getControlStateAction(code, mType);
            if (command == "")
            {
                // No. So try global actions.
                command = mCommandMapper->getGlobalAction(code);
            }
            else if (command == "freeflight_mode")
            {
                InputManager::getSingleton().pushControlState(CST_FREEFLIGHT);
                retval = true;
            }
            else if (command == "reset_camera")
            {
                resetCamera();
                retval = true;
            }
            else if (command == "toggle_view_mode")
            {
                toggleViewMode();
                retval = true;
            }
            else if (startAction(command, mCharacter))
                retval = true;

            if (!retval)
            {
                int movement = mCommandMapper->getMovement(evt.key);

                if (movement & MOVE_RUN_LOCK) // dieses einrasten lassen
                {
                    mCharacterState.mCurrentMovementState ^= MOVE_RUN_LOCK;
                    movement &= ~MOVE_RUN_LOCK;
                    retval = true;
                }

                if (movement != MOVE_NONE)
                {
                    mCharacterState.mCurrentMovementState |= movement;
                    retval = true;
                }
            }
        }

        if (ControlState::keyPressed(evt, handled || retval))
            retval = true;
        return retval;
    }

    //------------------------------------------------------------------------
    bool MovementControlState::keyReleased(const OIS::KeyEvent& evt, bool handled)
    {
        bool retval = false;
        int movement = mCommandMapper->getMovement(evt.key);
        if (movement != MOVE_NONE)
        {
            mCharacterState.mCurrentMovementState &= (~movement | MOVE_RUN_LOCK);
            retval = true;
        }

        if (ControlState::keyReleased(evt, retval))
            retval = true;
        return retval;
    }

    //------------------------------------------------------------------------
    bool MovementControlState::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled)
    {
        handled = handled || ControlState::mouseReleased(evt, id, handled);

        /*
                if( !handled )
                {
                    InputManager* im = InputManager::getSingletonPtr();
                    int mouseButtonMask = CommandMapper::encodeKey(id, im->getModifierCode());
                    return startAction(mCommandMapper->getControlStateAction(mouseButtonMask,
                        CST_MOVEMENT), mCharacter);
                }
        */
        return false;
    }

    //------------------------------------------------------------------------
    bool MovementControlState::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled)
    {
        handled = handled || ControlState::mouseReleased(evt, id, handled);

        // default action und action-selektor, falls object selected
        GameObject* newGo = mSelector.getFirstSelectedObject();
        if (newGo != nullptr && !isMouseUsedByCegui())
        {
            if (id == OIS::MB_Left)
            {
                if (newGo->getDefaultAction(mCharacter) != nullptr)
                {
                    newGo->doDefaultAction(mCharacter, nullptr);
                    handled = true;
                }
            }
            else if (id == OIS::MB_Right)
            {
                WindowFactory::getSingleton().showActionChoice(newGo);
                handled = true;
            }
        }

        if (!handled)
        {
            InputManager* im = InputManager::getSingletonPtr();
            int mouseButtonMask = CommandMapper::encodeKey(id, im->getModifierCode());
            return startAction(mCommandMapper->getControlStateAction(mouseButtonMask, CST_MOVEMENT), mCharacter);
        }
        return false;
    }

    //------------------------------------------------------------------------
    DebugVisualisableFlag MovementControlState::getFlag() const
    {
        return DVF_CONTROL;
    }

    //------------------------------------------------------------------------
    void MovementControlState::updatePrimitive()
    {
        if (mSceneNode->getParent() == nullptr)
        {
            mCharacterActor->_getSceneNode()->addChild(mSceneNode);
        }

        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
        lineSet->clear();
        lineSet->addLine(mLookAtOffset, mLookAtOffset + Vector3(0, 1.2, 0), ColourValue::Red);
    }

    //------------------------------------------------------------------------
    void MovementControlState::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }

    bool MovementControlState::updateAfterGameObjectLoading()
    {
        resume(); // saving/loading only possible in movement state
        //// We want to check for visibility from char's POV.
        // mSelector.setCheckVisibility(true, GameObjectManager::getSingleton().getGameObject(mCharacterId));
        // mSelector.track(mCharacter);
        // mSelector.setRadius(3.0);

        //// Same for combat selector
        // mCombatSelector.setCheckVisibility(true, GameObjectManager::getSingleton().getGameObject(mCharacterId));
        // mCombatSelector.track(mCharacter);
        // mCombatSelector.setRadius(10.0);

        return false;
    }

    bool MovementControlState::beforeLoadingSaveGame() // unhighlight selected go
    {
        if (mSelector.getFirstSelectedObject())
        {
            mSelector.getFirstSelectedObject()->setHighlighted(false);
        }

        pause(); // saving/loading only possible in movement state

        return false;
    }
}
