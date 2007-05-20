/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include <utility>

#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "InputManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "MeshAnimation.h"
#include "MovementCharacterController.h"
#include "Person.h"
#include "PhysicsManager.h"
#include "PhysicsMaterialRaycast.h"
#include "PhysicalThing.h"
#include "World.h"
#include "LineSetPrimitive.h"
#include "WindowFactory.h"
#include "AnimationManager.h"
#include "UiSubsystem.h"

#include <numeric>

using namespace Ogre;

namespace rl {

    String MovementCharacterController::msDebugWindowPageName = "MovementCharacterController";

    MovementCharacterController::CharacterState::CharacterState()
        :
        mCurrentMovementState(MOVE_NONE),
        mLastMovementState(MOVE_NONE)
    {
    }

    MovementCharacterController::MovementCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : CharacterController(cmdMapper, camera, character),
        mMovingCreature(NULL),
        mCharacterState(),
        mDesiredDistance(2.00),
        mDistanceRange(0.60, 7.00),
        mCamYaw(0),
        mCamVirtualYaw(0),
        mPitch(20),
        mRoll(0),
        mPitchRange(Degree(-75), Degree(85)),
        mLinearSpringK(400.0f),
        mLinearDampingK(Math::NEG_INFINITY),
        mCamMoveAwayVelocity(4.0f),
        mCamMoveAwayStartTime(0.25f),
        mCamMoveAwayRange(8.0f),
        mLookAtOffset(),
        mRotationSpeed(Degree(120.0f)),
        mMouseSensitivity(4.0f),
        mViewMode(VM_THIRD_PERSON),
        mObstractedFrameCount(0),
        mObstractedTime(0.0f),
        mCameraJammedFrameCount(0),
        mCameraJammedTime(0.0f),
        mRaycast(new PhysicsMaterialRaycast()),
        mSelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager()),
        mCombatSelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(),
            QUERYFLAG_CREATURE)
    {
        DebugWindow::getSingleton().registerPage(msDebugWindowPageName);


        mMouseSensitivity = ConfigurationManager::getSingleton().getIntSetting("Input", "Mouse Sensitivity");
        mInvertedMouse = ConfigurationManager::getSingleton().getBoolSetting("Input", "Mouse Invert");

        // The relationCoefficient determines the relation between spring accel in target direction
        // and damping in velocity direction. 1.0 means equilibrium is reached in optimal time
        // smaller 1.0 means spring accel is stronger and thus cam shoots over the target, resulting
        // in a damped ozillation before reaching equilibrium.
        // Values greater than 1.0 mean damping is stronger and thus camera takes a detour.
        Real relationCoefficient = 1.0f;
        mLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mLinearSpringK);

        // Offset for the look at point,
        // so the cam does look at the characters head instead of the feet.
        MeshObject* charMesh = dynamic_cast<MeshObject*>(
            mCharacterActor->getControlledObject());
        AxisAlignedBox aabb = charMesh->getDefaultSize();

        // this will be recalculated in calculateOptimalCameraPosition
        mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);

        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mCombatSelector.setFilter(filter);

        mSelector.setFilter(new InSceneSelectionFilter());
    }

    //------------------------------------------------------------------------
    MovementCharacterController::~MovementCharacterController()
    {
        delete mCombatSelector.getFilter();
        delete mRaycast;

        if (DebugWindow::getSingletonPtr())
        {
            DebugWindow::getSingletonPtr()->unregisterPage(msDebugWindowPageName);
        }

        // Remove debug scene node from character node, if debugview was used.
        if (mSceneNode != NULL && mSceneNode->getParent() != NULL)
        {
            mCharacterActor->_getSceneNode()->removeChild(mSceneNode);
        }
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::pause()
    {
        if( mMovingCreature != NULL )
        {
            delete mMovingCreature;
            mMovingCreature = NULL;
        }

        // actors aren't controlled anymore
        //mCharacterActor->getPhysicalThing()->setPhysicsController(NULL);
        mCameraActor->getPhysicalThing()->setPhysicsController(NULL);
        // cam<->Level collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"));
        // cam<->Default collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"));
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"));


        // Unhighlight selected object, if any.
        GameObject* go = mSelector.getFirstSelectedObject();
        if (go != NULL && go->isHighlighted())
        {
            go->setHighlighted(false);
        }
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::resume()
    {
        if( mMovingCreature == NULL )
            mMovingCreature = new MovingCreature(mCharacter);

        // We want to check for visibility from char's POV.
        mSelector.setCheckVisibility(true, mCharacter);
        mSelector.track(mCharacter);
        mSelector.setRadius(3.0);

        // Same for combat selector
        mCombatSelector.setCheckVisibility(true, mCharacter);
        mCombatSelector.track(mCharacter);
        mCombatSelector.setRadius(10.0);

        // The actor should be controlled manually,
        // so let the PM prepare it accordingly
        //mCharacterActor->getPhysicalThing()->setMaterialID(
        //    PhysicsManager::getSingleton().getMaterialID("character"));
        //mCharacterActor->getPhysicalThing()->setPhysicsController(this);
        mCameraActor->getPhysicalThing()->setMaterialID(
            PhysicsManager::getSingleton().createMaterialID("camera"));
        mCameraActor->getPhysicalThing()->setPhysicsController(this);

        // We also handle cam<->level, cam<->default cam<->char collision from now on
        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"))->setContactCallback(this);
        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"))->setContactCallback(this);
        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"))->setContactCallback(this);

        // Fit Collision proxy to idle anim
        mCharacterActor->getPhysicalThing()->fitToPose("idle");

        //MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        //mesh->stopAllAnimations();
        //mesh->startAnimation("idle");

        setViewMode(VM_THIRD_PERSON);
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::run(Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();

        updateCharacter(elapsedTime);
        updateCameraLookAt(elapsedTime);
        updateSelection();



        // camera pitch
        if (mInvertedMouse)
            mPitch -= 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
        else
            mPitch += 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
        if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
        if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;




        // Do we need to reset the Camera?
        Vector3 charPos = mCharacterActor->getWorldPosition();
        Quaternion charOri = mCharacterActor->getWorldOrientation();
        //mCharBody->getPositionOrientation(charPos, charOri);

        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);

        float maxdistance;
        if (mViewMode == VM_FIRST_PERSON)
            maxdistance = 0.25;
        else
            maxdistance = 1.3f * mDesiredDistance + 1.4f;

        // if we have more than 250ms and at least five frames with camera distance higher
        // than desired distance, reset camera
        if ((camPos - (charPos + charOri*mLookAtOffset)).length() > maxdistance)
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

        mCharacterState.mLastMovementState = mCharacterState.mCurrentMovementState;

        if (isEnemyNear())
        {
            InputManager::getSingleton().pushControlState(CST_COMBAT);
        }
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::updateCharacter(Ogre::Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();
        if( mMovingCreature != NULL )
        {
            int movement = mCharacterState.mCurrentMovementState;
            Degree rotation(0);

            AbstractMovement *drehen = mMovingCreature->getMovementFromId(MovingCreature::MT_DREHEN);
            Real baseVelocity = 0;
            if( drehen->calculateBaseVelocity(baseVelocity) )
            {
                Degree baseVel(baseVelocity*360);
                if (movement & TURN_LEFT)
                    rotation = elapsedTime * baseVel;
                if (movement & TURN_RIGHT)
                    rotation = -elapsedTime * baseVel;

                // mouse
                if( !im->isCeguiActive() && mViewMode == VM_FIRST_PERSON || mViewMode == VM_THIRD_PERSON )
                {
                    if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
                    {
                        rotation = -mMouseSensitivity/3.0f * Degree(im->getMouseRelativeX())/200.0 * baseVel;
                    }
                }


                // virtual yaw
                Degree newVirtualYaw(0);
                if( ((movement & MOVE_FORWARD) && (movement & MOVE_RIGHT) && !(movement & MOVE_LEFT)) ||
                    ((movement & MOVE_BACKWARD) && (movement & MOVE_LEFT) && !(movement & MOVE_RIGHT)) )
                {
                    newVirtualYaw = Degree(45);
                }
                if( ((movement & MOVE_FORWARD) && (movement & MOVE_LEFT) && !(movement & MOVE_RIGHT)) ||
                    ((movement & MOVE_BACKWARD) && (movement & MOVE_RIGHT) && !(movement & MOVE_LEFT)) )
                {
                    newVirtualYaw = Degree(-45);
                }
                if( mCamVirtualYaw != newVirtualYaw )
                {
                    rotation += mCamVirtualYaw - newVirtualYaw;
                    mCamVirtualYaw = newVirtualYaw;
                }
            }



            if( movement & MOVE_SNEAK )
            {
                Vector3 direction(Vector3::ZERO);
                if (movement & MOVE_FORWARD)
                    direction.z = -1;
                else if( movement & MOVE_BACKWARD)
                    direction.z = 1;
                mMovingCreature->setMovement(
                    MovingCreature::MT_SCHLEICHEN,
                    direction,
                    Vector3(0, rotation.valueRadians(), 0) );
            }
            else if( movement & MOVE_JUMP )
            {
                MovingCreature::MovementType type = MovingCreature::MT_HOCHSPRUNG;
                Vector3 direction = Vector3::UNIT_Y;
                if( movement & MOVE_FORWARD )
                {
                    type = MovingCreature::MT_WEITSPRUNG;
                    direction += Vector3::NEGATIVE_UNIT_Z;
                }
                mMovingCreature->setMovement(
                    type,
                    direction,
                    Vector3(0, rotation.valueRadians(), 0) );
            }
            else if( movement & MOVE_FORWARD )
            {
                MovingCreature::MovementType type = MovingCreature::MT_GEHEN;
                if( movement & MOVE_RUN_LOCK )
                {
                    if( movement & MOVE_RUN )
                        type = MovingCreature::MT_RENNEN;
                    else
                        type = MovingCreature::MT_LAUFEN;
                }
                else
                {
                    if( movement & MOVE_RUN )
                        type = MovingCreature::MT_GEHEN;
                    else
                        type = MovingCreature::MT_JOGGEN;
                }
                mMovingCreature->setMovement(
                    type,
                    Vector3(0,0,-1), 
                    Vector3(0, rotation.valueRadians(), 0) );
            }
            else if (movement & MOVE_BACKWARD )
            {
                MovingCreature::MovementType type = MovingCreature::MT_RUECKWAERTS_GEHEN;
                if( movement & MOVE_RUN )
                    type = MovingCreature::MT_RUECKWAERTS_JOGGEN;
                mMovingCreature->setMovement(
                    type,
                    Vector3(0,0,1), 
                    Vector3(0, rotation.valueRadians(), 0) );
            }
            else if (movement & MOVE_LEFT || movement & MOVE_RIGHT)
            {
                Vector3 direction = Vector3::UNIT_X;
                if( movement & MOVE_LEFT )
                    direction = Vector3::NEGATIVE_UNIT_X;
                mMovingCreature->setMovement(
                    MovingCreature::MT_SEITWAERTS_GEHEN,
                    direction, 
                    Vector3(0, rotation.valueRadians(), 0) );
            }
            else
            {
                mMovingCreature->setMovement(
                    MovingCreature::MT_STEHEN, 
                    Vector3(0,0,0),
                    Vector3(0, rotation.valueRadians(), 0) );
            }
        }
    }

    // ------------------------------------------------------------------------
    void MovementCharacterController::updateCameraLookAt(Ogre::Real elapsedTime)
    {
        InputManager* im = InputManager::getSingletonPtr();

        // camera position (distance)
        mDesiredDistance -= im->getMouseRelativeZ() * 0.002;
        if (mDesiredDistance < mDistanceRange.first)
        {
            mDesiredDistance = mDistanceRange.first;
        }
        if (mDesiredDistance > mDistanceRange.second)
        {
            mDesiredDistance = mDistanceRange.second;
        }

        if( !im->isCeguiActive() && mViewMode == VM_FREE_CAMERA )
        {
            mCamYaw -= 2 * mMouseSensitivity / 4.0 * mRotationSpeed * Degree(im->getMouseRelativeX() / 15);

            while (mCamYaw.valueDegrees() > 360.0f) mCamYaw -= Degree(360.0f);
            while (mCamYaw.valueDegrees() < -360.0f) mCamYaw += Degree(360.0f);
        }

        SceneNode* cameraNode = mCameraActor->_getSceneNode();

        Vector3 charPos;
        charPos = mCharacter->getActor()->getWorldPosition();
        Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw, Vector3::UNIT_Y);


        if( mViewMode == VM_FIRST_PERSON)
        {
            Quaternion camOri;
            camOri.FromAngleAxis(mPitch, Vector3::NEGATIVE_UNIT_X);
            cameraNode->lookAt(
                charPos
                + charOri * virtualCamOri * mLookAtOffset
                + charOri * camOri * virtualCamOri * (-Vector3::UNIT_Z),
                Node::TS_WORLD);
        }
        else if( mViewMode == VM_THIRD_PERSON )
        {
            // Kamera-Gr�e beziehen
            CameraObject* ogreCam = static_cast<CameraObject*>(
                mCameraActor->getControlledObject());
            AxisAlignedBox aabb = ogreCam->getDefaultSize();
            // Radius berechnen
            Real radius = (aabb.getMaximum()-aabb.getMinimum()).length() / 2.0f;

            cameraNode->lookAt(
                charPos
                + charOri * virtualCamOri *  mLookAtOffset
                + charOri * virtualCamOri * (-Vector3::UNIT_Z*radius),
                Node::TS_WORLD);

        }
        else if( mViewMode == VM_FREE_CAMERA )
        {
            cameraNode->lookAt(
                charPos + charOri * virtualCamOri * mLookAtOffset,
                Node::TS_WORLD);
        }


        // Character ausblenden, wenn Kamera zu nah.
        if( mViewMode != VM_FIRST_PERSON )
        {
            // here the real charOri of the object is needed
            Vector3 charPos;
            Quaternion charOri;
            mCharBody->getPositionOrientation(charPos, charOri);
            Vector3 camPos;
            Quaternion camOri;
            mCamBody->getPositionOrientation(camPos, camOri);
            Vector3 camPoint, charPoint, normal;
            int collisionPoints =
                OgreNewt::CollisionTools::CollisionClosestPoint(
                    PhysicsManager::getSingleton()._getNewtonWorld(),
                    mCamBody->getCollision(), camOri, camPos,
                    mCharBody->getCollision(), charOri, charPos,
                    camPoint, charPoint, normal
                    );
            if( collisionPoints == 0 )
                mCharacterActor->setVisible(false);
            else
            {
                // eigentlich muss hier transparent gemacht werden!
                mCharacterActor->setVisible(true);
            }
        }
    }


    // -------------------------------------------------------------
    // character collision moved to MovingCreature(Manager)
    int MovementCharacterController::userProcess()
    {
        // only camera collision
    
        if( mViewMode == VM_FIRST_PERSON )
            return 0;

        setContactSoftness(1.0f);
        setContactElasticity(0.0f);

        return 1;
    }

    //------------------------------------------------------------------------
    // character callback moved to MovingCreature
    void MovementCharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
    {
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        Real timestep = world->getTimeStep();

        calculateCamera(timestep);




        ///@todo move to MovingCreature?
        SceneNode* node = mCharacterActor->_getSceneNode();
        std::ostringstream ss;
        Vector3 bodpos, playpos = node->getPosition();
        Quaternion egal;
        mCamBody->getPositionOrientation(bodpos,egal);
        static Real maxHeight(0);
        if( playpos.y > maxHeight )
            maxHeight = playpos.y;
        ss
            << "scene node : " << playpos << std::endl
            << "player max Height : " << maxHeight << std::endl
            << "camera posder : " << static_cast<Camera*>(
                mCameraActor->_getMovableObject())->getDerivedPosition() << std::endl
                << "camera pos : " << static_cast<Camera*>(
                    mCameraActor->_getMovableObject())->getPosition() << std::endl
            << "camera actor orientation : " << mCameraActor->getWorldOrientation() << std::endl
            << "camera actor : " << mCameraActor->getWorldPosition() << std::endl
            << "camera body pos : " << bodpos << std::endl
            << "camera distance : " << mDesiredDistance << std::endl
            << "is airborne: " << (mMovingCreature->getAbstractLocation() == MovingCreature::AL_AIRBORNE ? "true" : "false") << std::endl;

        LOG_DEBUG(Logger::UI, ss.str());
        DebugWindow::getSingleton().setPageText(msDebugWindowPageName, ss.str());
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::calculateCamera(const Ogre::Real& timestep)
    {
        Vector3 charPos = mCharacter->getActor()->getWorldPosition();
        Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw, Vector3::UNIT_Y);


        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);
        SceneNode* cameraNode = mCameraActor->_getSceneNode();

/*
        // Ringbuffer mit Positionen der Kamera
        static std::vector<Ogre::Vector3> camPositionsBuffer(20);
        static size_t camPositionsBufferIdx = -1;
        camPositionsBufferIdx = (camPositionsBufferIdx + 1) % camPositionsBuffer.size();
        camPositionsBuffer[camPositionsBufferIdx] = camPos;

        // Ringbuffer mit optimalen Positionen der Kamera
        static std::vector<Ogre::Vector3> camOptPositionsBuffer(20);
        static size_t camOptPositionsBufferIdx = -1;
        camOptPositionsBufferIdx = (camOptPositionsBufferIdx + 1) % camOptPositionsBuffer.size();
*/
        Vector3 optimalCamPos = calculateOptimalCameraPosition(true, timestep);
        charPos = charPos + charOri * virtualCamOri * mLookAtOffset;
//        camOptPositionsBuffer[camOptPositionsBufferIdx] = optimalCamPos;


        // Ringbuffer mit Positionen des Characters
        static std::vector<Ogre::Vector3> charPositionsBuffer(20);
        static size_t charPositionsBufferIdx = -1;
        charPositionsBufferIdx = (charPositionsBufferIdx + 1) % charPositionsBuffer.size();
        charPositionsBuffer[charPositionsBufferIdx] = charPos;


        static Real characterOccludedTime = 0;
        static unsigned int characterOccludedFrameCount = 0;



        // Kamera-Gr�e beziehen
        CameraObject* ogreCam = static_cast<CameraObject*>(
            mCameraActor->getControlledObject());
        AxisAlignedBox camAabb = ogreCam->getDefaultSize();
        // Radius berechnen
        Real camRadius = (camAabb.getMaximum().z - camAabb.getMinimum().z) / 2.0f;





        if (mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA )
        {

            // wir machen ein paar Raycasts um herauszufinden, ob wir von der jetzigen Position
            // so zur optimalen kommen
            const OgreNewt::MaterialID *charMaterialId = mCharBody->getMaterialGroupID();
            const OgreNewt::MaterialID *camMaterialId = mCamBody->getMaterialGroupID();
            PhysicsMaterialRaycast::MaterialVector materialVector;
            materialVector.push_back(charMaterialId);
            materialVector.push_back(camMaterialId);
//                PhysicsManager::getSingleton()._getLevelMaterialID();
            OgreNewt::World *world = PhysicsManager::getSingleton()._getNewtonWorld();

            Vector3 normToOptCamPos = (optimalCamPos - charPos);
            normToOptCamPos.normalise();



            RaycastInfo infoCastOptPos = mRaycast->execute(
                world,
                &materialVector,
                camPos + camRadius * normToOptCamPos, // Gr�e der Kamera einbeziehen
                optimalCamPos + camRadius * normToOptCamPos,
                true); // Gr�e der Kamera einbeziehen

            RaycastInfo infoCastChar = mRaycast->execute(
                world,
                &materialVector,
                camPos,
                charPos,
                true);

            Real maxdistance = Math::Pow(1.5f * mDesiredDistance + 1.4f, 2);
            if( infoCastChar.mBody || (camPos - charPos).squaredLength() > maxdistance)
            {
                characterOccludedTime += timestep;
                characterOccludedFrameCount++;

                // falls zu lange, Kamera resetten:
                if( characterOccludedTime > 0.500f && characterOccludedFrameCount > 10 )
                {
                    resetCamera();
                    return;
                }

            }
            else
                characterOccludedTime = 0;

            if( infoCastOptPos.mBody )
            {
                if( !infoCastChar.mBody ) // Character noch im Blickfeld
                {
                    // andere Position ermitteln, die ziwschen optimaler und Character liegt
                    // und erreichbar ist
                    Real lenToOptCamPos = (optimalCamPos - charPos).length();

                    RaycastInfo infoCastNewPos;
                    Real delta = lenToOptCamPos/2.0f;
                    Vector3 temp = charPos + delta * normToOptCamPos;
                    // Ann�erung in Schritten, an den Punkt, der von der aktuellen Position aus erreicht werden kann!
                    while( delta > 0.05 ) // genauigkeit des gefundenen Punktes
                    {
                        infoCastNewPos = mRaycast->execute(
                            world,
                            &materialVector,
                            camPos + camRadius * normToOptCamPos, // Gr�e der Kamera!
                            temp,
                            true);
                        delta = delta/2.0f;
                        if( infoCastNewPos.mBody ) // Hindernis gefunden, n�er an Char ran
                        {
                            temp = temp - delta * normToOptCamPos;
                        }
                        else // kein Hindernis gefunden, weiter von Char weg
                        {
                            temp = temp + delta * normToOptCamPos;
                        }
                    }

                    // Jetzt k�nen wir sicher sein, dass diese Stelle erreichbar ist:
                    temp = temp - 0.05 * normToOptCamPos;
                    // Gr�e der Kamera einbeziehen
                    optimalCamPos = temp - camRadius * normToOptCamPos;
                    // so ab hier kann ganz normal weiter gerechnet werden!
                }
            }


            static bool isPathfinding (false);
            static unsigned int lastReachableBufPos;
            // gibt an, ob schon gebufferte Daten fr den
            // neuen Weg existieren und dort weitergemacht werden kann,
            // oder ob neu nach einem Weg gesucht werden muss!
            if( infoCastChar.mBody && infoCastOptPos.mBody ) // neue Position und Character nicht erreichbar
            {
                // anderen Weg finden
                // hier werden erstmal nur alte Player-Positionen betrachtet
                // es wird davon ausgegangen, dass diese "nah" genug aneinanderliegen
                // und durch "Geraden" miteinander verbunden werden k�nen
                // durch das spring-Acc-Damping System sollten die Bewegungen trotzdem flssig
                // und weich (keine scharfen Kurven) erscheinen

                size_t buffSize = charPositionsBuffer.size();

                if( !isPathfinding )
                {
                    LOG_DEBUG(Logger::UI, " Pathfinding der Kamera sollte jetzt anfangen!");


                    // letzte Character - Position suchen, die erreichbar ist...
                    // Ist vermutlicherweise ja die letzte, davor war ja noch alles ok!
                    unsigned int delta = 1;
                    while ( delta < buffSize )
                    {
                        RaycastInfo info = mRaycast->execute(
                            world,
                            &materialVector,
                            camPos,
                            charPositionsBuffer[ (charPositionsBufferIdx - delta) % buffSize ],
                            true);

                        if( !info.mBody )
                            break;
                        delta++;
                    }
                    if( delta >= buffSize )
                    {
                        // is wohl irgendwas schiefgegangen!
                        LOG_MESSAGE(Logger::UI, " Der Ringbuffer mit den Player-Positionen scheint zu klein zu sein; Pathfinding der Kamera fehlgeschlagen! ");
                        isPathfinding = false;
                        resetCamera();
                        return;
                    }
                    lastReachableBufPos = delta;

                    // auf zu der ermittelten Position!
                    optimalCamPos = charPositionsBuffer[ (charPositionsBufferIdx - lastReachableBufPos) % buffSize ];
                }
                else
                {
                    LOG_DEBUG(Logger::UI, " Pathfinding der Kamera sollte weitergefhrt werden!");


                    // suche von lastReachableBufPos aus der letzten Frame nach neuen erreichbaren Buffer-Positionen
                    unsigned int delta = lastReachableBufPos; // das ist die von der letzten Frame!
                    while ( delta > 0 ) // delta = 0 braucht nicht berprft zu werden, wurde oben schon ausgeschlossen!
                    {
                        RaycastInfo info = mRaycast->execute(
                            world,
                            &materialVector,
                            camPos,
                            charPositionsBuffer[ (charPositionsBufferIdx - delta) % buffSize ],
                            true);

                        if( info.mBody )
                            break;
                        delta--;
                    }
                    lastReachableBufPos = delta + 1;

                    // auf zu der ermittelten Position!
                    optimalCamPos = charPositionsBuffer[ (charPositionsBufferIdx - lastReachableBufPos) % buffSize ];
                }
                isPathfinding = true; // so zum Testen noch keine Optimierung (doppelte Prfung gleicher sachen)
            }
            else
            {
                isPathfinding = false;
            }

            Vector3 diff = camPos - optimalCamPos;

            Vector3 cameraVelocity;
            cameraVelocity = mCamBody->getVelocity();
            // spring velocity
            Vector3 springAcc = -mLinearSpringK*diff - mLinearDampingK * cameraVelocity;

            // get the camera mass
            Real mass;
            Vector3 inertia;
            mCamBody->getMassMatrix(mass, inertia);

            //mCamBody->setPositionOrientation(newCamPos, camOri);
            mCamBody->setForce(springAcc * mass);
        }
        else if( mViewMode == VM_FIRST_PERSON )
        {
            mCamBody->setPositionOrientation(optimalCamPos, camOri);
        }
    }

    //------------------------------------------------------------------------
    Ogre::Vector3 MovementCharacterController::calculateOptimalCameraPosition(bool SlowlyMoveBackward, const Real &timestep)
    {
        Vector3 targetCamPos;

        Vector3 charPos = mCharacter->getActor()->getWorldPosition();
        Quaternion charOri = mCharacter->getActor()->getWorldOrientation();
        Quaternion virtualCamOri;
        virtualCamOri.FromAngleAxis(mCamVirtualYaw, Vector3::UNIT_Y);



        if( mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA )
        {
            charPos = charPos + charOri * mLookAtOffset;
            if(mViewMode == VM_THIRD_PERSON)
            {
                targetCamPos =
                    charPos
                    + charOri * virtualCamOri * Vector3(
                                    0,
                                    Math::Sin(mPitch) * mDesiredDistance,
                                    Math::Cos(mPitch) * mDesiredDistance);
            }
            else
            {
                Quaternion camOri;
                camOri.FromAngleAxis(mCamYaw, Vector3::UNIT_Y);
                targetCamPos =
                    charPos
                    + charOri * camOri * virtualCamOri * Vector3(
                                                0,
                                                Math::Sin(mPitch) * mDesiredDistance,
                                                Math::Cos(mPitch) * mDesiredDistance);
            }


            // Kamera-Gr�e beziehen
            CameraObject* ogreCam = static_cast<CameraObject*>(
                mCameraActor->getControlledObject());
            AxisAlignedBox aabb = ogreCam->getDefaultSize();
            // Radius berechnen
            Real radius = (aabb.getMaximum().z - aabb.getMinimum().z) / 2.0f;
            radius *= 1.1f; // bissle was dazu tun schadet nich, da ja nur wenige raycasts gemacht werden
            // unds eigentlich ne kugel ist!



            Vector3 startRay[6], endRay[6];

            Real sinPitchRad = Math::Sin(mPitch) * radius;
            Real cosPitchRad = Math::Cos(mPitch) * radius;
            Vector3 radiusOffset = charOri * Vector3(0, sinPitchRad, cosPitchRad);

            startRay[0] = charPos;
            endRay[0] = targetCamPos; // hier ist nun leider was doppelt,
                                      // dadurch kann aber sichergestellt
                                      // werden, dass kein Objekt direkt
                                      // hinter dem Helden bersehen wird
            startRay[1] = charPos + radiusOffset;
            endRay[1] = targetCamPos + radiusOffset;
            radiusOffset = charOri * Vector3(radius, sinPitchRad, cosPitchRad);
            startRay[2] = charPos + radiusOffset;
            endRay[2] = targetCamPos + radiusOffset;
            startRay[3] = charPos - radiusOffset;
            endRay[3] = targetCamPos - radiusOffset;
            radiusOffset = charOri * Vector3(0, radius-cosPitchRad, -sinPitchRad);
            startRay[4] = charPos + radiusOffset;
            endRay[4] = targetCamPos + radiusOffset;
            startRay[5] = charPos - radiusOffset;
            endRay[5] = targetCamPos - radiusOffset;

            const OgreNewt::MaterialID* materialId =
                mCharBody->getMaterialGroupID();
//                PhysicsManager::getSingleton()._getLevelMaterialID();
            OgreNewt::World *world = PhysicsManager::getSingleton()._getNewtonWorld();

            Vector3 diff = targetCamPos - charPos;
            bool CollisionFound = false;
            for( int i = 0; i < 6; i++ )
            {
                RaycastInfo info = mRaycast->execute(
                    world,
                    materialId,
                    startRay[i],
                    endRay[i],
                    true);

                if( info.mBody && info.mBody != mCamBody )
                {
                    CollisionFound = true;
                    Vector3 newdiff = (info.mDistance) * (endRay[i] - startRay[i]);
                    if( newdiff.squaredLength() < diff.squaredLength() )
                        diff = newdiff;
                    if( i == 0 ) // beim ersten schon nahes hindernis gefunden?
                    {
                        if( diff.squaredLength() < radius*radius )
                        {
                            diff -= charOri * Vector3(0, sinPitchRad, cosPitchRad);
                            break;
                        }
                    }
                }
            }

            // Langsames Entfernen vom Char:
            static Real lastDistance(0.0f);
            static Real TimeOfLastCollision(0.0f);
            if( CollisionFound )
                TimeOfLastCollision = 0.0f;
            else
                TimeOfLastCollision += timestep;


            Real desiredDistance = diff.length();
            Vector3 camPos;
            Quaternion camOri;
            mCamBody->getPositionOrientation(camPos, camOri);

            if( SlowlyMoveBackward &&
                desiredDistance > lastDistance )
            {

                diff.normalise();
                Real newDistance;
                Vector3 actDiff = camPos - charPos;
                actDiff.normalise();

                if( TimeOfLastCollision > mCamMoveAwayStartTime ||
                    diff.directionEquals(actDiff, mCamMoveAwayRange*timestep) )
                    newDistance = lastDistance + mCamMoveAwayVelocity*timestep;
                else
                    newDistance = lastDistance;

                if( newDistance > desiredDistance )
                    newDistance = desiredDistance;

                diff = diff*newDistance;

                lastDistance = newDistance;
            }
            else
                lastDistance = desiredDistance;


            targetCamPos = charPos + diff;
        }
        else  // FIRST_PERSON
        {
            // determine the optimal target position of the camera
            targetCamPos =
                charPos
                + charOri * virtualCamOri * mLookAtOffset
                + charOri * virtualCamOri * Vector3(
                                0,
                                Math::Sin(mPitch) * mDesiredDistance,
                                Math::Cos(mPitch) * mDesiredDistance);
        }


        return targetCamPos;
    }

    //------------------------------------------------------------------------
    bool MovementCharacterController::isEnemyNear()
    {
        mCombatSelector.updateSelection();

        const Selector::GameObjectVector& gov = mCombatSelector.getAllSelectedObjects();
        for (size_t i = 0, end = gov.size(); i < end; ++i)
        {
            Creature* creature = dynamic_cast<Creature*>(gov.at(i));
            if (creature && creature->getAlignment() == Creature::ALIGNMENT_ENEMY)
            {
                return true;
            }
        }

        return false;
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::updateSelection()
    {
        InputManager* im = InputManager::getSingletonPtr();
        if( im->isCeguiActive() )
            return;

        GameObject* oldGo = mSelector.getFirstSelectedObject();

        mSelector.updateSelection();

        GameObject* newGo = mSelector.getFirstSelectedObject();

        if (oldGo != NULL && oldGo != newGo)
        {
            oldGo->setHighlighted(false);
        }

        if (newGo != NULL && newGo != oldGo)
        {
            newGo->setHighlighted(true);
        }

        // Optionen anzeigen
        if (im->isMouseButtonDown(OIS::MB_Right) && newGo != NULL)
        {
            WindowFactory::getSingleton().showActionChoice(newGo);
        }
        else if (im->isMouseButtonDown(OIS::MB_Left) && newGo != NULL)
        {
            newGo->doDefaultAction(mCharacter, NULL);
        }
    }


    void MovementCharacterController::setViewMode(ViewMode mode)
    {
        mViewMode = mode;

        MeshObject* charMesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        AxisAlignedBox aabb = charMesh->getDefaultSize();
        if (mode == VM_FIRST_PERSON)
        {
            mLookAtOffset = Vector3(
                    0,
                    (aabb.getMaximum().y - aabb.getMinimum().y) * 0.90f,
                    (aabb.getMaximum().z - aabb.getMinimum().z) * (-0.3f) );
            mDistanceRange.first = 0.0;
            mDistanceRange.second = 0.0;
            mDesiredDistance = 0.0;
            mPitchRange.first = Degree(-85);
            mPitchRange.second = Degree(85);
            mPitch = 0;
            LOG_MESSAGE(Logger::UI, "Switch to 1st person view");
            resetCamera();
        }
        else if(mode == VM_THIRD_PERSON)
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
        else // mode == VM_FREE_CAMERA
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
    }

    //------------------------------------------------------------------------
/*
    // not used at the moment!
    void MovementCharacterController::interpolateAnimationLookAtOffset(std::string actAnim, std::string newAnim, Ogre::Real factor)
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
    MovementCharacterController::ViewMode MovementCharacterController::getViewMode()
    {
        return mViewMode;
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::toggleViewMode()
    {
        if (getViewMode() == VM_THIRD_PERSON)
            setViewMode(VM_FIRST_PERSON);
        else if(getViewMode() == VM_FIRST_PERSON)
            setViewMode(VM_FREE_CAMERA);
        else
            setViewMode(VM_THIRD_PERSON);
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::resetCamera()
    {
        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);
        mCamBody->setPositionOrientation(calculateOptimalCameraPosition(false, 0.0f), camOri);
        if(mViewMode == VM_FIRST_PERSON)
            mCharacterActor->setVisible(false);
        else
            mCharacterActor->setVisible(true);
        
        LOG_MESSAGE(Logger::UI, "Camera resetted.");
        ///@todo remove this
        mCharacter->modifyAu(100);
    }

    //------------------------------------------------------------------------
    bool MovementCharacterController::injectKeyDown(int keycode)
    {
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
        int movement = mCommandMapper->getMovement(scancode);

        if (movement & MOVE_RUN_LOCK) // dieses einrasten lassen
        {
            mCharacterState.mCurrentMovementState ^= MOVE_RUN_LOCK;
            movement &= ~MOVE_RUN_LOCK;
        }
        if (movement != MOVE_NONE)
        {
            mCharacterState.mCurrentMovementState |= movement;
            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------
    bool MovementCharacterController::injectKeyUp(int keycode)
    {
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
        int movement = mCommandMapper->getMovement(scancode);

        if (movement != MOVE_NONE)
        {
            mCharacterState.mCurrentMovementState &= (~movement | MOVE_RUN_LOCK);
            return true;
        }
        else
        {
            CeGuiString command = mCommandMapper->getControlStateAction(keycode, CST_MOVEMENT);
            if (command == "freeflight_mode")
            {
                InputManager::getSingleton().pushControlState(CST_FREEFLIGHT);
                return true;
            }
            else if (command == "reset_camera")
            {
                resetCamera();
                return true;
            }
            else if (command == "toggle_view_mode")
            {
                toggleViewMode();
                return true;
            }
            else
            {
                return startAction(command);
            }
        }

        return false;
    }

    //------------------------------------------------------------------------
    bool MovementCharacterController::injectMouseDown(int mouseButtonMask)
    {
        return false;
    }

    //------------------------------------------------------------------------
    bool MovementCharacterController::injectMouseUp(int mouseButtonMask)
    {
        if (!InputManager::getSingleton().isCeguiActive())
        {
            return startAction(mCommandMapper->getControlStateAction(mouseButtonMask,
                CST_MOVEMENT), mCharacter);
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------
    DebugVisualisableFlag MovementCharacterController::getFlag() const
    {
        return DVF_CONTROL;
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::updatePrimitive()
    {
        if (mSceneNode->getParent() == NULL)
        {
            mCharacterActor->_getSceneNode()->addChild(mSceneNode);
        }

        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
        lineSet->clear();
        lineSet->addLine(mLookAtOffset, mLookAtOffset + Vector3(0, 1.2, 0), ColourValue::Red);
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }
}
