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
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "InputManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "MovementCharacterController.h"
#include "PhysicsManager.h"
#include "PhysicsMaterialRaycast.h"
#include "PhysicalThing.h"
#include "World.h"
#include "LineSetPrimitive.h"
#include "WindowFactory.h"

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
		mLastMovementState(MOVE_NONE)
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
		mRotationSpeed(Degree(180.0f)),
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
		mMovementSpeed = 
            (float)character->getWert(Creature::WERT_GS) / 
			(float)Date::ONE_KAMPFRUNDE 
			* 1000.0f;

		mGravitation = PhysicsManager::getSingleton().getGravity();
		mMouseSensitivity
			= ConfigurationManager::getSingleton().getIntSetting("mouse_sensitivity", 4);

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
		InputManager* im = InputManager::getSingletonPtr();

		// Fetch current movement state
		mCharacterState.mDesiredVel = Vector3::ZERO;
		int movement = mCharacterState.mCurrentMovementState;

		// Determine character's control state based on user input
		if (movement & MOVE_FORWARD)  mCharacterState.mDesiredVel.z = -1;
		if (movement & MOVE_BACKWARD) mCharacterState.mDesiredVel.z =  1;
		if (movement & MOVE_RIGHT)    mCharacterState.mDesiredVel.x =  1;
		if (movement & MOVE_LEFT)     mCharacterState.mDesiredVel.x = -1;
		
		mCharacterState.mDesiredVel.normalise();
		mCharacterState.mDesiredVel *= mMovementSpeed;
		
		if (movement & MOVE_JUMP)  mCharacterState.mStartJump = true;

		if (movement & TURN_LEFT)  mYaw += mRotationSpeed*elapsedTime;
		if (movement & TURN_RIGHT) mYaw -= mRotationSpeed*elapsedTime;

		if (isRunMovement(movement))
        {
            if( movement & MOVE_FORWARD ) 
                mCharacterState.mDesiredVel *= 3.0;
            else // für Rückwärts und Seite
                mCharacterState.mDesiredVel *= 1.5;
        }


		mDesiredDistance -= im->getMouseRelativeZ() * 0.002;
		if (mDesiredDistance < mDistanceRange.first)
		{
			mDesiredDistance = mDistanceRange.first;
		}
		if (mDesiredDistance > mDistanceRange.second)
		{
			mDesiredDistance = mDistanceRange.second;
		}

		mPitch += 0.5 * mMouseSensitivity * Degree(im->getMouseRelativeY() / 10);
		if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
		if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;


        if(mViewMode == VM_FREE_CAMERA)
            mCamYaw -= 2 * mMouseSensitivity / 4.0 * mRotationSpeed * Degree(im->getMouseRelativeX() / 15);
        else
        {
            if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
		        mYaw -= mMouseSensitivity / 4.0 * mRotationSpeed * Degree(im->getMouseRelativeX() / 15);
        }

		SceneNode* cameraNode = mCameraActor->_getSceneNode();

		Vector3 charPos;
		Quaternion charOri;
		mCharBody->getPositionOrientation(charPos, charOri);

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

		while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
		while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);
		while (mCamYaw.valueDegrees() > 360.0f) mCamYaw -= Degree(360.0f);
		while (mCamYaw.valueDegrees() < -360.0f) mCamYaw += Degree(360.0f);


        updateAnimationState();

        if (!im->isCeguiActive())
        {
		    updateSelection();
            if (im->isMouseButtonDown(OIS::MB_Right) && mSelector.getSelectedObject() != NULL)
            {
                WindowFactory::getSingleton().showActionChoice(mSelector.getSelectedObject());
            }
            else if (im->isMouseButtonDown(OIS::MB_Left) && mSelector.getSelectedObject() != NULL)
            {
                mSelector.getSelectedObject()->doDefaultAction(mCharacter, NULL);
            }
        }
		mCharBody->getPositionOrientation(charPos, charOri);

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

		updateAnimationState();
    }

	bool MovementCharacterController::isRunMovement(int movement)
	{
		return 
			((movement & MOVE_RUN) && !(movement & MOVE_RUN_LOCK)) 
			|| (!(movement & MOVE_RUN) && (movement & MOVE_RUN_LOCK));
	}

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
			setContactElasticity(0.0f); // was 0.3f 
			setContactSoftness(0.01f);

			setContactFrictionState(1, 0);
			setContactFrictionState(1, 1);
		}

		// return one to tell Newton we want to accept this contact
		return 1;
	}

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

            if( (mCharacterState.mHasFloorContact && mCharacterState.mJumpTimer > 0.25f) || 
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
			// Might be set to false in the collision callback
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

	void MovementCharacterController::calculateCamera(const Ogre::Real& timestep)
	{
		Vector3 charPos;
		Quaternion charOri;
		mCharBody->getPositionOrientation(charPos, charOri);

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

    Ogre::Vector3 MovementCharacterController::calculateOptimalCameraPosition(bool SlowlyMoveBackward, const Real &timestep)
    {
        Vector3 targetCamPos;


        if( mViewMode == VM_THIRD_PERSON || mViewMode == VM_FREE_CAMERA )
        {
            if( !mCharBody->getCollision().isNull() )
            {
                // man könnte mLookAtOffset dynamisch durch creature bestimmen lassen (offset der augen)
		        // so the cam does look at the characters head instead of the feet.
                AxisAlignedBox aabb = mCharBody->getCollision()->getAABB();
                if(mViewMode == VM_THIRD_PERSON)
                {
                    mLookAtOffset = Vector3(
                        0, 
                        (aabb.getMaximum().y - aabb.getMinimum().y) * 0.45f, 
                        0 //(aabb.getMaximum().z - aabb.getMinimum().z) * (-0.3f)
                        );
                }
                else
                {
                    mLookAtOffset = Vector3(
                        0, 
                        (aabb.getMaximum().y - aabb.getMinimum().y) * 0.25f, 
                        0 //(aabb.getMaximum().z - aabb.getMinimum().z) * (-0.3f)
                        );
                }
            }

            Vector3 charPos;
            Quaternion charOri;
            mCharBody->getPositionOrientation(charPos, charOri);
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
            if( !mCharBody->getCollision().isNull() )
            {
		        // so the cam does look at the characters head instead of the feet.
                AxisAlignedBox aabb = mCharBody->getCollision()->getAABB();
                mLookAtOffset = Vector3(
                    0, 
                    (aabb.getMaximum().y - aabb.getMinimum().y) * 0.45f,
                    (aabb.getMaximum().z - aabb.getMinimum().z) * (-0.3f) 
                    );
            }
/*
std::ostringstream ss;
            // nach head bone suchen!
            // funktioniert noch nicht ganz
ss << "suche position der head-bone: ";
            ActorControlledObject *aco = mCharacterActor->getControlledObject();
            if( aco->isMeshObject() )
            {
ss << "Mesh-Objekt ok; ";
                MeshObject* meshObj = dynamic_cast<MeshObject*>(aco);
                Entity* ent = meshObj->getEntity();
                if( ent->hasSkeleton() )
                {
ss << "Skeleton found; ";
                    Ogre::SkeletonInstance *skel = ent->getSkeleton();
                    try
                    {
                        Ogre::Bone *head = skel->getBone(Ogre::String("kopf"));
                        if( head != NULL )
                        {
ss << "'head'-bone found; ";
                            mLookAtOffset = head->getPosition();
ss << "Position: ( " << mLookAtOffset.x << " | " << mLookAtOffset.y << " | " << mLookAtOffset.z << " )";
                        }
                    }
                    catch(...)
                    {
ss << "'head'-bone NOT found; ";
                    }
                }
            }
//LOG_DEBUG(Logger::UI, ss.str());
*/
            Vector3 charPos;
            Quaternion charOri;
            mCharBody->getPositionOrientation(charPos, charOri);
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

	void MovementCharacterController::updateSelection()
	{
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
	}

	void MovementCharacterController::updateAnimationState()
	{
		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
		PhysicalThing* pt = mCharacterActor->getPhysicalThing();
        static bool lastAirBorne(false);

		if (mCharacterState.mCurrentMovementState != mCharacterState.mLastMovementState
            || mCharacterState.mIsAirBorne != lastAirBorne)
		{
            // Do we need to update coliision proxy?
            if (mCharacterState.mCurrentMovementState & MOVE_SNEAK
                && mCharacterState.mPose != CharacterState::Crouch)
            {
                pt->fitToPose("hocke_idle");
                mCharacterState.mPose = CharacterState::Crouch;
            }
            else if (!(mCharacterState.mCurrentMovementState & MOVE_SNEAK)
                && mCharacterState.mPose == CharacterState::Crouch)
            {
                pt->fitToPose("idle");
                mCharacterState.mPose = CharacterState::Stand;
            }

			// Update animation state
            mesh->stopAllAnimations();
            if (mCharacterState.mIsAirBorne)
            {
            }
			else if (mCharacterState.mCurrentMovementState & MOVE_SNEAK)
			{
				if (mCharacterState.mCurrentMovementState & MOVE_FORWARD)
				{
					mesh->startAnimation("hocke_gehen");
				}
				else
				{
					mesh->startAnimation("hocke_idle");
				}
			}
			else if (isRunMovement(mCharacterState.mCurrentMovementState) 
				&& (mCharacterState.mCurrentMovementState != MOVE_RUN) 
				&& (mCharacterState.mCurrentMovementState != MOVE_RUN_LOCK))
			{
				mesh->startAnimation("rennen");
			}
			else if (mCharacterState.mCurrentMovementState == MOVE_NONE ||
				mCharacterState.mCurrentMovementState == MOVE_RUN)
			{
				mesh->startAnimation("idle");
			}
			else
			{
				// standard walk in any direction
				if (mCharacterState.mCurrentMovementState & MOVE_FORWARD)
				{
					mesh->startAnimation("gehen");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_BACKWARD)
				{
					mesh->startAnimation("gehen_rueckwaerts");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_LEFT)
				{
					mesh->startAnimation("seitwaerts_links");
				}
				else if (mCharacterState.mCurrentMovementState & MOVE_RIGHT)
				{
					mesh->startAnimation("seitwaerts_rechts");
				}
				else if (mCharacterState.mCurrentMovementState & TURN_LEFT)
				{
					mesh->startAnimation("drehen_links");
				}
				else if (mCharacterState.mCurrentMovementState & TURN_RIGHT)
				{
					mesh->startAnimation("drehen_rechts");
				}
			}
			mCharacterState.mLastMovementState = mCharacterState.mCurrentMovementState;
            lastAirBorne = mCharacterState.mIsAirBorne;
		}
	}

	void MovementCharacterController::setViewMode(ViewMode mode)
	{
		mViewMode = mode;

		MeshObject* charMesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
		AxisAlignedBox aabb = charMesh->getDefaultSize();
		if (mode == VM_FIRST_PERSON)
		{
            // wird in calculateOptimalCameraPosition gesetzt
			//mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);
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
            // wird in calculateOptimalCameraPosition gesetzt
			//mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);
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
            // wird in calculateOptimalCameraPosition gesetzt
			//mLookAtOffset = Vector3(0, (aabb.getMaximum() - aabb.getMinimum()).y * 0.45f, 0);
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

	MovementCharacterController::ViewMode MovementCharacterController::getViewMode()
	{
		return mViewMode;
	}

	void MovementCharacterController::toggleViewMode()
	{
		if (getViewMode() == VM_THIRD_PERSON)
            setViewMode(VM_FIRST_PERSON);
        else if(getViewMode() == VM_FIRST_PERSON)
            setViewMode(VM_FREE_CAMERA);
        else
            setViewMode(VM_THIRD_PERSON);         
	}

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

	bool MovementCharacterController::injectKeyDown(int keycode)
	{
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
		int movement = mCommandMapper->getMovement(scancode);

		if (movement != MOVE_NONE)
		{
			mCharacterState.mCurrentMovementState |= movement;
			return true;
		}
		return false;
	}

	bool MovementCharacterController::injectKeyUp(int keycode)
	{
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
		int movement = mCommandMapper->getMovement(scancode);

		if (movement != MOVE_NONE)
		{
			mCharacterState.mCurrentMovementState &= ~movement;
			return true;
		}
        else
        {
            return startAction(mCommandMapper->getAction(keycode, CMDMAP_KEYMAP_OFF_COMBAT), mCharacter);
        }
			
		return false;
	}

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

    DebugVisualisableFlag MovementCharacterController::getFlag() const
    {
        return DVF_CONTROL;
    }

    void MovementCharacterController::updatePrimitive()
    {
        if (mSceneNode->getParent() == NULL)
        {
            mCharacterActor->_getSceneNode()->addChild(mSceneNode);
        }

        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
        lineSet->clear();
        lineSet->addLine(mLookAtOffset, mLookAtOffset + Vector3(0, 1.2, 0), ColourValue::Red);
        lineSet->addLine(Vector3::ZERO, mGravitation * 0.1, ColourValue::Green);
    }

    void MovementCharacterController::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }
}
