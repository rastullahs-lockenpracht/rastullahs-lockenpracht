/*
* This source file is part of Rastullahs Lockenpracht.
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

#include "DialogCharacterController.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "Actor.h"
#include "PhysicalThing.h"
#include "World.h"
#include "CameraObject.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "MeshObject.h"

#include <OgreSceneManager.h>
#include <OgreNewt_Body.h>

using namespace Ogre;

namespace rl {

	DialogCharacterController::DialogCharacterController(Actor* camera, Actor* character)
		: CharacterController(camera, character),
		mTargetCameraPosition(Vector3::ZERO),
		mTargetCameraOrientation(Quaternion::IDENTITY),
		mText(""),
		mFadeTextTime(0),
		mCutHard(false)
	{
		mCamera->getPhysicalThing()->freeze();
		mCharacterActor->getPhysicalThing()->freeze();		
		Camera* ogreCam = static_cast<Camera*>(mCamera->_getMovableObject());
		ogreCam->setFixedYawAxis(true);
		ogreCam->setPosition(Vector3::ZERO);	
		ogreCam->setOrientation(Quaternion::IDENTITY);
		mCamera->_getSceneNode()->setFixedYawAxis(true);
	}

	DialogCharacterController::~DialogCharacterController()
	{
		mCamera->getPhysicalThing()->unfreeze();
		mCharacterActor->getPhysicalThing()->unfreeze();
	}

	CharacterController::ControllerType DialogCharacterController::getType() const
	{
		return CTRL_DIALOG;
	}

	void DialogCharacterController::setDialogPartner(Actor* partner)
	{
		mDialogPartner = partner;

		resetCamera();

		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
		mesh->stopAllAnimations();
		mesh->startAnimation("Idle");
	}

	void DialogCharacterController::run(Real elapsedTime)
	{
		if( elapsedTime <= 0.0 )
			return;

		// Aktuellen Abstand zur gewünschten Position berechnen
		Vector3 posDistance = (mTargetCameraPosition - mCamera->_getSceneNode()->getPosition());
		// Ist eine Anpassung nötig?
		if( posDistance != Vector3::ZERO )
		{
			// Die Kameraanpassgeschwindigkeit
			Real speed = 1/elapsedTime;
			
			mCamera->setPosition(mTargetCameraPosition);		
			mCamera->setOrientation(mTargetCameraOrientation);
		}

		// Textanzeigedauer
		if (mFadeTextTime > 0)
		{
			mFadeTextTime -= elapsedTime;
			if (mFadeTextTime <= 0)
			{
				mFadeTextTime = 0;
				//mDialogWindow->timeOver();
			}
		}
	}

	void DialogCharacterController::toggleViewMode()
	{
		/// @todo Krasser Gesichts zoom? Halbtotale... usw?
	}

	void DialogCharacterController::resetCamera()
	{
		// Position camera at position between char and dialog partner
		Vector3 globalCameraPosition = (mCharacterActor->getWorldPosition() + mDialogPartner->getWorldPosition()) / 2;
		globalCameraPosition.y += 4;
		
		// Weltkoordinaten in lokale umwandeln
		mTargetCameraPosition = 
			-1*(mCamera->_getSceneNode()->getParentSceneNode()->getWorldOrientation().Inverse()*
			(mCamera->_getSceneNode()->getParentSceneNode()->getWorldPosition() - globalCameraPosition));

		// Drehung zu ZielRichtung  @todo reparieren
		mTargetCameraOrientation = 
			Vector3::UNIT_Z.getRotationTo(
			mTargetCameraPosition - mDialogPartner->getWorldPosition() );	
	}

	void DialogCharacterController::response(
		Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
	{
		// Drehung zu ZielRichtung  @todo reparieren
		mTargetCameraOrientation = 
			Vector3::UNIT_Z.getRotationTo(
			mTargetCameraPosition - mDialogPartner->getWorldPosition() );		
		mText = text;
		mFadeTextTime = text.length() * 0.1;
	}
}
