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
#include "PhysicsManager.h"
#include "PhysicsMaterialRaycast.h"
#include "PhysicalThing.h"
#include "World.h"
#include "LineSetPrimitive.h"
#include "WindowFactory.h"
#include "AnimationManager.h"

#include <numeric>

using namespace Ogre;

namespace rl {

    String MovementCharacterController::msDebugWindowPageName = "MovementCharacterController";

	MovementCharacterController::CharacterState::CharacterState()
		: mIsAirBorne(false),
        mHasFloorContact(false),
		mStartJump(false),
		mJumpTimer(0.0),
		mDesiredVel(Vector3::ZERO),
		mCurrentMovementState(MOVE_NONE),
		mLastMovementState(MOVE_NONE),
        mPose(Stand)
	{

	}

	MovementCharacterController::MovementCharacterController(Actor* camera, Creature* character)
		: CharacterController(camera, character->getActor()),
		mCharacterState(),
		mCharacter(character),
		mDesiredDistance(2.00),
		mDistanceRange(0.60, 7.00),
		mYaw(0),
        mCamYaw(0),
		mPitch(20),
		mPitchRange(Degree(-75), Degree(85)),
        mLinearSpringK(400.0f),
        mLinearDampingK(Math::NEG_INFINITY),
        mCamMoveAwayVelocity(4.0f),
        mCamMoveAwayStartTime(0.25f),
        mCamMoveAwayRange(8.0f),
        mRotLinearSpringK(600.0f),
        mRotLinearDampingK(Math::NEG_INFINITY),
		mLookAtOffset(),
		mRotationSpeed(Degree(120.0f)),
        mMouseSensitivity(4.0f),
		mSpeedModifier(1.0f),
		mViewMode(VM_THIRD_PERSON),
//		mMaxDelay(1.0/20.0),
		mObstractedFrameCount(0),
		mObstractedTime(0.0f),
		mCameraJammedFrameCount(0),
		mCameraJammedTime(0.0f),
		mRaycast(new PhysicsMaterialRaycast()),
		mGravitation(),
        mSelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager())
	{
        DebugWindow::getSingleton().registerPage(msDebugWindowPageName);


		mGravitation = PhysicsManager::getSingleton().getGravity();
		mMouseSensitivity
			= ConfigurationManager::getSingleton().getIntSetting("mouse_sensitivity", 4);
        int temp = 
            ConfigurationManager::getSingleton().getIntSetting("mouse_invert", 0);
        mInvertedMouse = temp != 0;

        // The relationCoefficient determines the relation between spring accel in target direction
        // and damping in velocity direction. 1.0 means equilibrium is reached in optimal time
        // smaller 1.0 means spring accel is stronger and thus cam shoots over the target, resulting
        // in a damped ozillation before reaching equilibrium.
        // Values greater than 1.0 mean damping is stronger and thus camera takes a detour.
        Real relationCoefficient = 1.0f;
        mLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mLinearSpringK);
        mRotLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mRotLinearSpringK);

		// Offset for the look at point,
		// so the cam does look at the characters head instead of the feet.
		MeshObject* charMesh = dynamic_cast<MeshObject*>(
			mCharacterActor->getControlledObject());
        AxisAlignedBox aabb = charMesh->getDefaultSize();

        // wird später neu berechnet in calculateOptimalCameraPosition
        mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);

		// The actor should be controlled manually,
		// so let the PM prepare it accordingly
		PhysicsManager::getSingleton().setPhysicsController(
			mCharacterActor->getPhysicalThing(), this);
		PhysicsManager::getSingleton().setPhysicsController(
			mCameraActor->getPhysicalThing(), this);
        // We also handle char<->level, char<->default collision from now on
        PhysicsManager::getSingleton().setCharLevelContactCallback(this);
        PhysicsManager::getSingleton().setCharDefaultContactCallback(this);
		// Fit Collision proxy to idle anim
        mCharacterActor->getPhysicalThing()->fitToPose("idle");

        MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
		mesh->stopAllAnimations();
		mesh->startAnimation("idle");

		setViewMode(VM_THIRD_PERSON);

        // We want to check for visibility from char's POV.
        mSelector.setCheckVisibility(true, mCharacterActor);
	}

	//------------------------------------------------------------------------
	MovementCharacterController::~MovementCharacterController()
	{
        delete mRaycast;
		// actors aren't controlled anymore
		PhysicsManager::getSingleton().setPhysicsController(
			mCharacterActor->getPhysicalThing(), NULL);
		PhysicsManager::getSingleton().setPhysicsController(
			mCameraActor->getPhysicalThing(), NULL);
        // Char<->Level collision back to default
        PhysicsManager::getSingleton().setCharLevelContactCallback(NULL);
        // Char<->Default collision back to default
        PhysicsManager::getSingleton().setCharDefaultContactCallback(NULL);

        if (DebugWindow::getSingletonPtr())
        {
            DebugWindow::getSingletonPtr()->unregisterPage(msDebugWindowPageName);
        }

        // Unhighlight selected object, if any.
        GameObject* go = mSelector.getSelectedObject();
        if (go != NULL && go->isHighlighted())
        {
            go->setHighlighted(false);
        }

        // Remove debug scene node from character node, if debugview was used.
        if (mSceneNode != NULL && mSceneNode->getParent() != NULL)
        {
            mCharacterActor->_getSceneNode()->removeChild(mSceneNode);
        }
	}

	//------------------------------------------------------------------------
	CharacterController::ControllerType MovementCharacterController::getType() const
	{
		return CTRL_MOVEMENT;
	}

	//------------------------------------------------------------------------
	void MovementCharacterController::run(Real elapsedTime)
	{
        int movement = mCharacterState.mCurrentMovementState;
        updateAnimationState(movement);
        updateCharacterState(movement, elapsedTime);
        updateCameraLookAt(elapsedTime);
        updateSelection();


        // Do we need to reset the character?
        static Real charAnimationOccTime = 0;
        charAnimationOccTime += elapsedTime;
        if( mCharacterState.mPose != CharacterState::Stand &&
            mCharacterState.mPose != CharacterState::Crouch )
        {
            if( charAnimationOccTime > 10.0f ) // mehr als ... sekunden!
                mCharacterState.mPose = CharacterState::Stand;
        }
        else
            charAnimationOccTime = 0;

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
    }

	//------------------------------------------------------------------------
	void MovementCharacterController::updateCharacterState(int movement, Ogre::Real elapsedTime)
	{
		InputManager* im = InputManager::getSingletonPtr();
        Real gs = mCharacter->getWert(Creature::WERT_GS);
        Real auPerSecond = 0.0; // Standard - Ausdauer-verbrauch/Regeneration pro Sekunde
        bool canRun = mCharacter->getAu() > 6.0f;
        Radian rotationSpeed = mRotationSpeed * gs / 6.0f; // <-- arme Zwerge? aber wenn gs sinkt sollte das auch sinken!

        if (mCharacterState.mPose == CharacterState::StartJump ||
            mCharacterState.mPose == CharacterState::Jumping ||
            mCharacterState.mPose == CharacterState::EndJump)
            ; // do nothing, old mDesiredVel!!
        else
            mCharacterState.mDesiredVel = Vector3::ZERO;

        if (mCharacterState.mPose == CharacterState::Stand)
        {
            // Sprünge werden von updateAnimationStat geregelt

            // Rückwärts gehen oder laufen
            if ( movement & MOVE_BACKWARD && !(movement & MOVE_FORWARD) )
            {
                if( movement & MOVE_RUN_LOCK ) // Rückwärts joggen
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,1) * 0.6 * gs / 2.0;
                }
                else
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,1) * 0.6 * gs / 3.6;
                }
            }
            else if ( movement & MOVE_FORWARD ) // Vorwärtsbewegung
            {
                if( movement & MOVE_RUN && movement & MOVE_RUN_LOCK ) // sprinten
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,-1) * gs;
                }
                else if( movement & MOVE_RUN && !(movement & MOVE_RUN_LOCK) ) // gehen
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,-1) * gs / 3.6f;
                }
                else if( movement & MOVE_RUN_LOCK ) // laufen
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,-1) * gs / 2.0f;
                }
                else // joggen
                {
                    mCharacterState.mDesiredVel = Vector3(0,0,-1) * gs / 2.5f;
                }
            }
            else if( movement & MOVE_LEFT )
            {
                mCharacterState.mDesiredVel = Vector3(-1,0,0) * gs / 3.6;
            }
            else if( movement & MOVE_RIGHT )
            {
                mCharacterState.mDesiredVel = Vector3(1,0,0) * gs / 3.6;
            }

            if (movement & TURN_LEFT)  mYaw += rotationSpeed*elapsedTime;
            if (movement & TURN_RIGHT) mYaw -= rotationSpeed*elapsedTime;


        }
        else if (mCharacterState.mPose == CharacterState::Crouch)
        {
            if (movement & MOVE_FORWARD)
                mCharacterState.mDesiredVel.z = -0.5 * gs / 3.6;
            rotationSpeed *= 0.6;

            if (movement & TURN_LEFT)  mYaw += rotationSpeed*elapsedTime;
            if (movement & TURN_RIGHT) mYaw -= rotationSpeed*elapsedTime;

        }

/*
// soll dafür sorgen, dass er auf dem
// Boden bleibt und über kleine Hindernisse kommt

Quaternion orientation = mCharacterActor->getWorldOrientation();
Vector3 position = mCharacterActor->getWorldPosition();
Vector3 delta(0,0.05,0);
position += delta;
Real stepHeight = 0.5;
Real stepWidth = 0.3;
Vector3 dirVector = stepWidth*mCharacterState.mDesiredVel;
dirVector.y = stepHeight;
RaycastInfo info;
Vector3 relCollPos;
do
{
    // Raycast in Bewegungsrichtung
    info = mRaycast->execute(
        PhysicsManager::getSingleton()._getNewtonWorld(),
        mCharBody->getMaterialGroupID(),
        position, position + orientation * dirVector,
        true);
    if( info.mBody )
    {
        relCollPos = (info.mDistance * 1.1) * dirVector.normalisedCopy();
        dirVector.y += 0.1;
        break;
    }
    dirVector.y -= 0.2;
}
while( dirVector.y > -stepHeight );

// kann das Hindernis überwunden werden:
dirVector = relCollPos;
while( dirVector.y <= stepHeight )
{
    info = mRaycast->execute(
        PhysicsManager::getSingleton()._getNewtonWorld(),
        mCharBody->getMaterialGroupID(),
        position, position + orientation * dirVector,
        true);
    if( info.mBody )
    {
        break;
    }
    dirVector.y += 0.1;
}

// linie an der angegebenen Stelle anzeigen
LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
if(lineSet != NULL)
{
    lineSet->clear();
    lineSet->addLine(delta, delta+dirVector, ColourValue::Blue);
    lineSet->addLine(delta+dirVector, delta+2*dirVector, ColourValue::Red);
    lineSet->addLine(delta, delta-dirVector, ColourValue::Red);
    lineSet->addLine(delta, delta+relCollPos, ColourValue::White);
}

if( dirVector.y < stepHeight && dirVector.y > -stepHeight )
{
    mCharacterState.mDesiredVel.y = dirVector.y;
    // Bremsen wenn bergab:
    if (dirVector.y < 0)
    {
        mCharacterState.mDesiredVel *= (1 + dirVector.y/stepHeight * 0.5);
    }
}
*/








		mDesiredDistance -= im->getMouseRelativeZ() * 0.002;
		if (mDesiredDistance < mDistanceRange.first)
		{
			mDesiredDistance = mDistanceRange.first;
		}
		if (mDesiredDistance > mDistanceRange.second)
		{
			mDesiredDistance = mDistanceRange.second;
		}

        if (mInvertedMouse)
		    mPitch -= 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
        else
            mPitch += 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
		if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
		if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;


        if( !im->isCeguiActive() && mViewMode == VM_FIRST_PERSON || mViewMode == VM_THIRD_PERSON )
        {
            if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
            {
                mYaw -= mMouseSensitivity / 6.0 * rotationSpeed * Degree(im->getMouseRelativeX() / 15);

                while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
		        while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);
            }
        }
    }

	//------------------------------------------------------------------------
	void MovementCharacterController::updateCameraLookAt(Ogre::Real elapsedTime)
	{
        InputManager* im = InputManager::getSingletonPtr();
        

        if( !im->isCeguiActive() && mViewMode == VM_FREE_CAMERA )
        {
            mCamYaw -= 2 * mMouseSensitivity / 4.0 * mRotationSpeed * Degree(im->getMouseRelativeX() / 15);

		    while (mCamYaw.valueDegrees() > 360.0f) mCamYaw -= Degree(360.0f);
		    while (mCamYaw.valueDegrees() < -360.0f) mCamYaw += Degree(360.0f);
        }

		SceneNode* cameraNode = mCameraActor->_getSceneNode();

		Vector3 charPos;
		Quaternion charOri;
		//mCharBody->getPositionOrientation(charPos, charOri);
        charPos = mCharacterActor->getWorldPosition();
        charOri = mCharacterActor->getWorldOrientation();

		if( mViewMode == VM_FIRST_PERSON)
		{
			Quaternion camOri;
            camOri.FromAngleAxis(mPitch, Vector3::NEGATIVE_UNIT_X);
			cameraNode->lookAt(
				charPos
				+ charOri * mLookAtOffset
				+ charOri * camOri * (-Vector3::UNIT_Z), 
				Node::TS_WORLD);
		}
		else if( mViewMode == VM_THIRD_PERSON )
		{
            // Kamera-Größe beziehen
            CameraObject* ogreCam = static_cast<CameraObject*>(
                mCameraActor->getControlledObject());
            AxisAlignedBox aabb = ogreCam->getDefaultSize();
            // Radius berechnen
            Real radius = (aabb.getMaximum()-aabb.getMinimum()).length() / 2.0f;

            cameraNode->lookAt(
				charPos	+ mLookAtOffset
                + charOri * (-Vector3::UNIT_Z*radius), 
				Node::TS_WORLD);

		}
        else if( mViewMode == VM_FREE_CAMERA )
        {
            cameraNode->lookAt(
				charPos	+ mLookAtOffset,
                Node::TS_WORLD);
        }


        // Character ausblenden, wenn Kamera zu nah.
        if( mViewMode != VM_FIRST_PERSON )
        {
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

    //------------------------------------------------------------------------
    // adopted from the chararcter demo in the newton sdk
    // copyright 2000-2004
    // By Julio Jerez
	int MovementCharacterController::userProcess()
	{
        if ((m_body0 == mCamBody && m_body1 == mCharBody)
            || (m_body0 == mCharBody && m_body1 == mCamBody))
        {
            // No collision between char and camera
            return 0;
        }
		else if (m_body0 == mCamBody || m_body1 == mCamBody)
		{
			// this is camera collision

            if( mViewMode == VM_FIRST_PERSON )
                return 0;

            setContactSoftness(1.0f);  // "weiche" Collision
            setContactElasticity(0.0f);

            return 1;
		}
		else if (m_body0 == mCharBody || m_body1 == mCharBody)
		{
			// this is character collision
			Vector3 point;
			Vector3 normal;
			getContactPositionAndNormal(point, normal);

			// determine if this contact is with the floor.
			// Meaning the contact normal has an angle to UNIT_Y of 20° or less.
			Degree angle = Math::ACos(normal.dotProduct(Vector3::UNIT_Y));

            Vector3 charPos;
            Quaternion charOri;
            mCharBody->getPositionOrientation(charPos, charOri);
            bool isFloorCollision(false);

            //if( charPos.y > point.y && angle < Degree(50.0f)  )
            AxisAlignedBox CharAab = mCharBody->getCollision()->getAABB();
            Real CharHeight = CharAab.getMaximum().y - CharAab.getMinimum().y;
            Real stepHeight = point.y - charPos.y + CharHeight / 2.0f;
            if( stepHeight < 0.4f && mCharacterState.mHasFloorContact ||
                stepHeight < 0.1f )
                isFloorCollision = true;

            if ( isFloorCollision )
            {
				mCharacterState.mHasFloorContact = true;
            }
            else
            {

				Vector3 velocity = mCharBody->getVelocity();

				// calculate char velocity perpendicular to the contact normal
				Vector3 tangentVel = velocity - normal * (normal.dotProduct(velocity));

				// align the tangent at the contact point with the
				// tangent velocity vector of the char
				rotateTangentDirections(tangentVel);
			}


            // beinflusst die Sprunghöhe!
            // der erste Sprung bleibt normal, jeder weitere Sprung ist ungefähr um
            // die angegebene Zahl höher (eher 2*die angegebene Zahl oder so, 
            // vermutlich wieder framedauer abhängig!)
			setContactElasticity(0.3f); // was 0.3f 
			setContactSoftness(0.01f);

			setContactFrictionState(1, 0);
			setContactFrictionState(1, 1);
		}

		// return one to tell Newton we want to accept this contact
		return 1;
	}

    //------------------------------------------------------------------------
    // adopted from the chararcter demo in the newton sdk
    // copyright 2000-2004
    // By Julio Jerez
	void MovementCharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
	{
		OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
		OgreNewt::Body* body = thing->_getBody();

		// Get the current world timestep
		Real timestep = world->getTimeStep();

		if (body == mCamBody)
		{
			calculateCamera(timestep);
		}
		else
		{
		    mCharacterState.mJumpTimer += timestep;
            static Real timeSinceLastFloorContact(0.0f); // damit kleine Bodenunebenheiten nicht gleich zum "Sprung führen"
            if( !mCharacterState.mHasFloorContact )
                timeSinceLastFloorContact += timestep;
            else
                timeSinceLastFloorContact = 0.0f;
            
            
            
            
            // apply character force
			Vector3 position;
			Quaternion orientation;
			body->getPositionOrientation(position, orientation);

			// get the charater mass
			Real mass;
			Vector3 inertia;
			body->getMassMatrix(mass, inertia);

			// apply gravity
            Vector3 force = Vector3::ZERO;
			force = mass * mGravitation;

			// Get the velocity vector
			Vector3 currentVel = body->getVelocity();

			// Gravity is applied above, so not needed here
			// prevent adding a counter force against gravity
			//if (currentVel.y < 0.0f || mCharacterState.mJumpTimer < 2.0f) 
			currentVel.y = 0.0f;

            if( (mCharacterState.mHasFloorContact && mCharacterState.mJumpTimer > 0.1f) || 
                ( timeSinceLastFloorContact < 0.2f && !mCharacterState.mIsAirBorne ) )
                mCharacterState.mIsAirBorne = false;
            else
                mCharacterState.mIsAirBorne = true;


            if( !mCharacterState.mIsAirBorne )
            {
			    // apply the jump
			    if ( mCharacterState.mStartJump )
			    {
				    mCharacterState.mJumpTimer = 0.0f;
                    mCharacterState.mIsAirBorne = true;
                    Real height = 0.75f; // beim zweiten gehts seltsamerweise was höher /doppelsprung physik?)
                    Real gravity = 9.81f;
                    Real jumpForce = 0.5f*gravity*mass * (Math::Sqrt(1 + 8*height/(gravity * timestep * timestep)) - 1);
                    force += Vector3(0,
                        jumpForce,
                        0);
			    }

                Real delay = 2 * PhysicsManager::getSingleton().getMaxTimestep(); // so ist die Beschleunigung unabhängig von der framerate!
                force += mass*(orientation*mCharacterState.mDesiredVel - currentVel) / delay;
            }
            mCharacterState.mStartJump = false;

			body->setForce(force);
			// Assume we are air borne.
			// Might be set to true in the collision callback
			mCharacterState.mHasFloorContact = false;






		    // Calculate angular velocity
		    // We first need the yaw rotation from actual yaw to desired yaw
		    Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
		    src.y = 0;
		    Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::UNIT_Z;
		    dst.y = 0;
		    Radian yaw = src.getRotationTo(dst).getYaw();

            
            // was soll dieser komische Fehler?
            // wenn yaw 0 sein sollte, wird er manchmal zu -90 grad, find ich nich nett
            if( (yaw.valueDegrees() - (-90)) < 0.5 )
                if( src.directionEquals(-dst, Degree(45)) )
                    yaw = Degree(0);


		    // Calculate omega in order to go this rotation in mMaxDelay seconds.
            // Real newOmega = yaw.valueRadians() / mMaxDelay;
            Vector3 diff = Vector3(0, yaw.valueRadians(), 0);
            Vector3 omega = mCharBody->getOmega();
            omega.x = omega.z = 0;
            // sollte nicht direkt gesetzt werden!
            Vector3 springAcc = -mRotLinearSpringK*diff - mRotLinearDampingK * omega;
		    //body->setOmega(Vector3(0, newOmega, 0)); // omega sollte nicht direkt gesetzt werden
            body->setTorque( mass * springAcc );
        }



        SceneNode* node = mCharacterActor->_getSceneNode();
		std::ostringstream ss;
        Vector3 bodpos, playpos = node->getPosition();
        Quaternion egal;
        mCamBody->getPositionOrientation(bodpos,egal);
        static Real maxHeight(0);
        if( playpos.y > maxHeight )
            maxHeight = playpos.y;
        ss 
            << "scene node : " << playpos << endl
//            << "player max Height : " << maxHeight << endl
            << "camera posder : " << static_cast<Camera*>(
                mCameraActor->_getMovableObject())->getDerivedPosition() << endl
//                << "camera pos : " << static_cast<Camera*>(
//                    mCameraActor->_getMovableObject())->getPosition() << endl
            << "camera actor orientation : " << mCameraActor->getWorldOrientation() << endl
            << "camera actor : " << mCameraActor->getWorldPosition() << endl
            << "camera body pos : " << bodpos << endl
            << "is airborne: " << (mCharacterState.mIsAirBorne ? "true" : "false") << endl
            << "start jump : " << (mCharacterState.mStartJump ? "true" : "false")  << endl
            << "jump timer : " << mCharacterState.mJumpTimer << endl;

        LOG_DEBUG(Logger::UI, ss.str());
        DebugWindow::getSingleton().setPageText(msDebugWindowPageName, ss.str());
    }

    //------------------------------------------------------------------------
	void MovementCharacterController::calculateCamera(const Ogre::Real& timestep)
	{
		Vector3 charPos;
		Quaternion charOri;
		//mCharBody->getPositionOrientation(charPos, charOri);
        charPos = mCharacterActor->getWorldPosition();
        charOri = mCharacterActor->getWorldOrientation();

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
        charPos = charPos + charOri * mLookAtOffset;
//        camOptPositionsBuffer[camOptPositionsBufferIdx] = optimalCamPos;
        

        // Ringbuffer mit Positionen des Characters
        static std::vector<Ogre::Vector3> charPositionsBuffer(20);
        static size_t charPositionsBufferIdx = -1;
        charPositionsBufferIdx = (charPositionsBufferIdx + 1) % charPositionsBuffer.size();
        charPositionsBuffer[charPositionsBufferIdx] = charPos;


        static Real characterOccludedTime = 0;
        static unsigned int characterOccludedFrameCount = 0;



        // Kamera-Größe beziehen
        CameraObject* ogreCam = static_cast<CameraObject*>(
            mCameraActor->getControlledObject());
        AxisAlignedBox camAabb = ogreCam->getDefaultSize();
        // Radius berechnen
        Real camRadius = (camAabb.getMaximum().z - camAabb.getMinimum().z) / 2.0f;





		if (mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA )
		{

            // wir machen ein paar Raycasts um herauszufinden, ob wir von der jetzigen Position
            // so zur optimalen kommen
            const OgreNewt::MaterialID* materialId =
                mCharBody->getMaterialGroupID();
//			    PhysicsManager::getSingleton()._getLevelMaterialID();
            OgreNewt::World *world = PhysicsManager::getSingleton()._getNewtonWorld();

            Vector3 normToOptCamPos = (optimalCamPos - charPos);
            normToOptCamPos.normalise();



            RaycastInfo infoCastOptPos = mRaycast->execute(
                world, 
                materialId, 
                camPos + camRadius * normToOptCamPos, // Größe der Kamera einbeziehen
                optimalCamPos + camRadius * normToOptCamPos,
                true); // Größe der Kamera einbeziehen

            RaycastInfo infoCastChar = mRaycast->execute(
                world, 
                materialId, 
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
                    // Annäherung in Schritten, an den Punkt, der von der aktuellen Position aus erreicht werden kann!
                    while( delta > 0.05 ) // genauigkeit des gefundenen Punktes
                    {
                        infoCastNewPos = mRaycast->execute(
                            world, 
                            materialId,
                            camPos + camRadius * normToOptCamPos, // Größe der Kamera!
                            temp,
                            true);
                        delta = delta/2.0f;
                        if( infoCastNewPos.mBody ) // Hindernis gefunden, näher an Char ran
                        {
                            temp = temp - delta * normToOptCamPos;
                        }
                        else // kein Hindernis gefunden, weiter von Char weg
                        {
                            temp = temp + delta * normToOptCamPos;
                        }
                    }

                    // Jetzt können wir sicher sein, dass diese Stelle erreichbar ist:
                    temp = temp - 0.05 * normToOptCamPos;
                    // Größe der Kamera einbeziehen
                    optimalCamPos = temp - camRadius * normToOptCamPos;
                    // so ab hier kann ganz normal weiter gerechnet werden!
                }
            }


            static bool isPathfinding (false); 
            static unsigned int lastReachableBufPos;
            // gibt an, ob schon gebufferte Daten für den
            // neuen Weg existieren und dort weitergemacht werden kann,
            // oder ob neu nach einem Weg gesucht werden muss!
            if( infoCastChar.mBody && infoCastOptPos.mBody ) // neue Position und Character nicht erreichbar
            {
                // anderen Weg finden
                // hier werden erstmal nur alte Player-Positionen betrachtet
                // es wird davon ausgegangen, dass diese "nah" genug aneinanderliegen
                // und durch "Geraden" miteinander verbunden werden können
                // durch das spring-Acc-Damping System sollten die Bewegungen trotzdem flüssig
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
                            materialId,
                            camPos,
                            charPositionsBuffer[ (charPositionsBufferIdx - delta) % buffSize ] ,
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
                    LOG_DEBUG(Logger::UI, " Pathfinding der Kamera sollte weitergeführt werden!");


                    // suche von lastReachableBufPos aus der letzten Frame nach neuen erreichbaren Buffer-Positionen
                    unsigned int delta = lastReachableBufPos; // das ist die von der letzten Frame!
                    while ( delta > 0 ) // delta = 0 braucht nicht überprüft zu werden, wurde oben schon ausgeschlossen!
                    {
                        RaycastInfo info = mRaycast->execute(
                            world, 
                            materialId,
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
                isPathfinding = true; // so zum Testen noch keine Optimierung (doppelte Prüfung gleicher sachen)
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


        if( mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA )
        {
            Vector3 charPos;
            Quaternion charOri;
            charPos = mCharacterActor->getWorldPosition();
            charOri = mCharacterActor->getWorldOrientation();
            //mCharBody->getPositionOrientation(charPos, charOri);
            charPos = charPos + charOri * mLookAtOffset;
            if(mViewMode == VM_THIRD_PERSON)
            {
	            targetCamPos = 
		            charPos 
		            + charOri * Vector3(
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
		            + charOri * camOri * Vector3(
						                        0, 
						                        Math::Sin(mPitch) * mDesiredDistance,
						                        Math::Cos(mPitch) * mDesiredDistance);
            }


            // Kamera-Größe beziehen
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
                                      // hinter dem Helden übersehen wird
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
//			    PhysicsManager::getSingleton()._getLevelMaterialID();
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
            
                if( info.mBody )
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
        else
        {
            Vector3 charPos;
            Quaternion charOri;
            //mCharBody->getPositionOrientation(charPos, charOri);
            charPos = mCharacterActor->getWorldPosition();
            charOri = mCharacterActor->getWorldOrientation();
            // determine the optimal target position of the camera            
	        targetCamPos = 
		        charPos 
		        + charOri * mLookAtOffset 
		        + charOri * Vector3(
						        0, 
						        Math::Sin(mPitch) * mDesiredDistance,
						        Math::Cos(mPitch) * mDesiredDistance);
        }


        return targetCamPos;
    }

    //------------------------------------------------------------------------
	void MovementCharacterController::updateSelection()
	{
        InputManager* im = InputManager::getSingletonPtr();
        if( im->isCeguiActive() )
            return;

        GameObject* oldGo = mSelector.getSelectedObject();

        mSelector.setPosition(mCharacterActor->getWorldPosition());
        mSelector.setOrientation(mCharacterActor->getWorldOrientation());
        mSelector.setRadius(3.0);
        mSelector.updateSelection();

        GameObject* newGo = mSelector.getSelectedObject();

        if (oldGo != NULL && oldGo != newGo)
        {
            oldGo->setHighlighted(false);
        }

        if (newGo != NULL && newGo != oldGo)
        {
            newGo->setHighlighted(true);            
        }

        // Optionen anzeigen
        if (im->isMouseButtonDown(OIS::MB_Right) && mSelector.getSelectedObject() != NULL)
        {
            WindowFactory::getSingleton().showActionChoice(mSelector.getSelectedObject());
        }
        else if (im->isMouseButtonDown(OIS::MB_Left) && mSelector.getSelectedObject() != NULL)
        {
            mSelector.getSelectedObject()->doDefaultAction(mCharacter, NULL);
        }
	}

    //------------------------------------------------------------------------
	void MovementCharacterController::updateAnimationState(int &movement)
	{
		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
		PhysicalThing* pt = mCharacterActor->getPhysicalThing();
        static std::string lastAnimation("");
        std::string newAnimation("");
        Real animSpeed = 1;
        static Real lastSpeed = 1;
        static bool jumpNextFrame(false);
        unsigned int animTimesToPlay = 0;
        Real gs = mCharacter->getWert(Creature::WERT_GS);
		static String lastCollisionPose = "";
		String collisionPose = "idle";

        // the different factors used to calculate the animation-speed from the character-speed
        const Real factor_hocke_gehen = 0.7;
        const Real factor_drehen_idle = 0.6;
        const Real factor_gehen = 0.5;
        const Real factor_gehen_rueckwaerts = 0.7;
        const Real factor_rennen = 0.25;
        const Real factor_rennen_absprung = 0.25;
        const Real factor_rennen_sprung_landung = 0.6;
        const Real factor_rennen_sprung = 0.25;
        const Real factor_seitwaerts = 0.9;


        Vector3 charVelocity, charOmega;
        charOmega = mCharBody->getOmega();
        charVelocity = mCharBody->getVelocity();
        Quaternion charOri = mCharacterActor->getWorldOrientation();
        if( charOri != Quaternion::ZERO )
        {
            charVelocity = charOri.Inverse() * charVelocity;
        }
        Real vel = charVelocity.length();

        if (jumpNextFrame)
            movement |= MOVE_JUMP;

        if (mCharacterState.mPose == CharacterState::Falling) // noch nicht implementiert!
            mCharacterState.mPose = CharacterState::Stand;


        if (mCharacterState.mPose == CharacterState::Stand && movement & MOVE_SNEAK)
        {
            newAnimation = "idle_zu_hocke";
            mCharacterState.mPose = CharacterState::StandToCrouch;
            animTimesToPlay = 1;
        }
        else if(mCharacterState.mPose == CharacterState::StandToCrouch)
        {
            MeshAnimation *meshAnim = mesh->getAnimation("idle_zu_hocke");
            if (meshAnim->getTimePlayed() >= meshAnim->getLength())
            {
                newAnimation = "hocke_idle";
				collisionPose = "hocke_idle";
                mCharacterState.mPose = CharacterState::Crouch;
            }
            // kamera-offset interpolieren grrr
            Real factor;
            factor = meshAnim->getTimePlayed() / meshAnim->getLength();
            interpolateAnimationLookAtOffset("idle", "hocke_idle", factor);
        }
        else if(mCharacterState.mPose == CharacterState::Crouch && movement & MOVE_SNEAK)
        {
            if (vel > 0.1 && movement & MOVE_FORWARD)
            {
                newAnimation = "hocke_gehen";
				collisionPose = "hocke_idle";
                animSpeed = charVelocity.length() * factor_hocke_gehen;
            }
            else
            {
                newAnimation = "hocke_idle";
				collisionPose = "hocke_idle";
            }
        }
        else if (mCharacterState.mPose == CharacterState::Crouch)
        {
            // Achtung, nur wenn nach oben hin genug Platz ist aufstehen!!
            newAnimation = "hocke_zu_stehen";
            mCharacterState.mPose = CharacterState::CrouchToStand;
            animTimesToPlay = 1;
        }
        else if(mCharacterState.mPose == CharacterState::CrouchToStand)
        {
            MeshAnimation *meshAnim = mesh->getAnimation("hocke_zu_stehen");
            if (meshAnim->getTimePlayed() >= meshAnim->getLength())
            {
                newAnimation = "idle";
                mCharacterState.mPose = CharacterState::Stand;
            }
            // kamera-offset interpolieren grrr
            Real factor;
            factor = meshAnim->getTimePlayed() / meshAnim->getLength();
            interpolateAnimationLookAtOffset("hocke_idle", "idle", factor);
        }
        else if(mCharacterState.mPose == CharacterState::StartJump)
        {
            if( lastAnimation == "rennen_absprung" )
            {
                MeshAnimation *meshAnim = mesh->getAnimation("rennen_absprung");
                if (meshAnim->getTimePlayed() >= meshAnim->getLength())
                {
					///\todo Animation "rennen_sprung" has length of 0.0f. Substitute for Idle, till this is resolved.
                    //newAnimation = "rennen_sprung";
                    newAnimation = "idle";

                    animSpeed = factor_rennen_sprung * vel;
                    animTimesToPlay = 1;
                    mCharacterState.mPose = CharacterState::Jumping;
                    mCharacterState.mStartJump = true;
                }
            }
            else // "idle_absprung"
            {
                MeshAnimation *meshAnim = mesh->getAnimation("idle_absprung");
                if (meshAnim->getTimePlayed() >= meshAnim->getLength())
                {
					///\todo Animation "idle_sprung" has length of 0.0f. Substitute for Idle, till this is resolved.
                    //newAnimation = "idle_sprung";
                    newAnimation = "idle";

                    animTimesToPlay = 1;
                    mCharacterState.mPose = CharacterState::Jumping;
                    mCharacterState.mStartJump = true;
                }
            }
        }
        else if(mCharacterState.mPose == CharacterState::Jumping)
        {
            if( lastAnimation == "rennen_sprung" )
            {
                if (!mCharacterState.mIsAirBorne)
                {
                    newAnimation = "rennen_sprung_landung";
                    animSpeed = factor_rennen_sprung_landung * vel;
                    animTimesToPlay = 1;
                    mCharacterState.mPose = CharacterState::EndJump;
                }
            }
            else // "idle_sprung"
            {
                if (!mCharacterState.mIsAirBorne)
                {
                    newAnimation = "idle_sprung_landung";
                    animTimesToPlay = 1;
                    mCharacterState.mPose = CharacterState::EndJump;
                }
            }
        }
        else if(mCharacterState.mPose == CharacterState::EndJump)
        {
            MeshAnimation *meshAnim = mesh->getAnimation(lastAnimation);

            if (meshAnim->getTimePlayed() >= meshAnim->getLength())
                mCharacterState.mPose = CharacterState::Stand;
        }




        // absichtlich kein else!
        if( mCharacterState.mPose == CharacterState::Stand && !(movement & MOVE_SNEAK) )
        {
            newAnimation = "idle";

            // Nur im Stehen Drehen:
            if( vel < 0.1)
            {
                if( charOmega.squaredLength() < 0.1)
                {
                    newAnimation = "idle";
                }
                else if( charOmega.y > 0 )
                {
                    newAnimation = "drehen_links";
                    animSpeed = factor_drehen_idle * charOmega.y * (1);
                }
                else if( charOmega.y < 0 )
                {
                    newAnimation = "drehen_rechts";
                    animSpeed = factor_drehen_idle * charOmega.y * (-1);
                }
            }
            else if( vel <= gs / 3.55f)
            {
                if( charVelocity.z > 0 && movement & MOVE_BACKWARD)
                {
                    newAnimation = "gehen_rueckwaerts";
                    animSpeed = factor_gehen_rueckwaerts * vel;
                }
                else if( movement & MOVE_FORWARD )
                {
                    newAnimation = "gehen";
                    animSpeed = factor_gehen * vel;
                }
                else if( movement & MOVE_LEFT )
                {
                    newAnimation = "seitwaerts_links";
                    animSpeed = factor_seitwaerts * vel;
                }
                else if( movement & MOVE_RIGHT )
                {
                    newAnimation = "seitwaerts_rechts";
                    animSpeed = factor_seitwaerts * vel;
                }
            }
            else if( movement & MOVE_FORWARD )
            {
                newAnimation = "rennen";
                animSpeed = factor_rennen * vel;
            }


            if( movement & MOVE_JUMP )
            {
                // Beginn eines Sprunges
                if( newAnimation == "rennen" )
                {
                    newAnimation = "rennen_absprung";
                    animTimesToPlay = 1;
                    animSpeed = factor_rennen_absprung * vel;
                    mCharacterState.mPose = CharacterState::StartJump;
                }
                else 
                {
                    if ( vel > 0.1 )
                    {
                        // erstmal anhalten!
                        movement = MOVE_NONE;
                        jumpNextFrame = true;
                    }
                    else
                    {
                        newAnimation = "idle_absprung";
                        animTimesToPlay = 1;
                        mCharacterState.mPose = CharacterState::StartJump;
                    }
                }
            }
        }

        if( mCharacterState.mPose == CharacterState::StartJump )
            jumpNextFrame = false;




        if (newAnimation != "")
        {
			// nur schneller nicht langsamer ausführen!
            if (animSpeed < 1) animSpeed = 1;
                
            if (lastAnimation != newAnimation)
            {
				if (collisionPose != lastCollisionPose)
				{
					pt->fitToPose(collisionPose);
					lastCollisionPose = collisionPose;
				}
                mesh->stopAllAnimations();
                mesh->startAnimation(newAnimation, animSpeed, animTimesToPlay);
                lastAnimation = newAnimation;
                lastSpeed = animSpeed;
            }
            else
            {
                if ( lastSpeed != animSpeed ) // Geschwindigkeitsänderung
                {
                    MeshAnimation *meshAnim = mesh->getAnimation(newAnimation);
                    meshAnim->setSpeed(animSpeed);
                    lastSpeed = animSpeed;
                }
            }
        }
    }

    //------------------------------------------------------------------------
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
            mCamYaw = mYaw;
			LOG_MESSAGE(Logger::UI, "Switch to free camera view");			
			resetCamera();
        }
	}

    //------------------------------------------------------------------------
    void MovementCharacterController::interpolateAnimationLookAtOffset(std::string actAnim, std::string newAnim, Ogre::Real factor)
    {
        AxisAlignedBox aab;
        Vector3 size[2];
        Vector3 interpolatedSize;


        // Die Größe der beiden Animationen abfragen
        MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        aab = mesh->getPoseSize(actAnim);
        size[0] = aab.getMaximum() - aab.getMinimum();

        aab = mesh->getPoseSize(newAnim);
        size[1] = aab.getMaximum() - aab.getMinimum();

        // interpolierte Größe (linear) berechnen
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
            return startAction(mCommandMapper->getAction(keycode, CMDMAP_KEYMAP_OFF_COMBAT), mCharacter);
        }
			
		return false;
	}

    //------------------------------------------------------------------------
	bool MovementCharacterController::injectMouseDown(int mouseButtonMask)
	{
      //  if (!im->isCeguiActive())
      //  {
		    //updateSelection();
      //      if (im->isMouseButtonDown(1) && mSelector.getSelectedObject() != NULL)
      //      {
      //          WindowFactory::getSingleton().showActionChoice(mSelector.getSelectedObject());
      //      }
      //      else if (im->isMouseButtonDown(0) && mSelector.getSelectedObject() != NULL)
      //      {
      //          mSelector.getSelectedObject()->doDefaultAction(mCharacter, NULL);
      //      }
      //  }
	  return false;
	}

    //------------------------------------------------------------------------
	bool MovementCharacterController::injectMouseUp(int mouseButtonMask)
	{
        if (!InputManager::getSingleton().isCeguiActive())
        {
		    return startAction(mCommandMapper->getAction(mouseButtonMask,
                CMDMAP_MOUSEMAP_OFF_COMBAT), mCharacter);		
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
        //lineSet->clear();
        //lineSet->addLine(mLookAtOffset, mLookAtOffset + Vector3(0, 1.2, 0), ColourValue::Red);
        //lineSet->addLine(Vector3::ZERO, mGravitation * 0.1, ColourValue::Green);
    }

    //------------------------------------------------------------------------
    void MovementCharacterController::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }
}
