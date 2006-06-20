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
#include "Exception.h"

#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "CoreSubsystem.h"
#include "DialogWindow.h"
#include "InputManager.h"
#include "MeshObject.h"
#include "PhysicalThing.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"
#include "WindowManager.h"
#include "World.h"


#include <OgreSceneManager.h>
#include <OgreNewt_Body.h>

using namespace Ogre;

namespace rl {

	DialogCharacterController::DialogCharacterController(Actor* camera, Actor* character)
		: CharacterController(camera, character),
		mTargetCameraPosition(Vector3::ZERO),
		mTargetCameraOrientation(Quaternion::IDENTITY),
		mFadeTextTime(0),
		mCutHard(false),
		mDialogWindow(NULL),
		mSubtitleWindow(NULL),
		mSoundObject(NULL)
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
		if (mDialogWindow != NULL)
		{
			WindowManager::getSingleton().destroyWindow(mDialogWindow);
			mDialogWindow = NULL;
		}
		if (mSubtitleWindow != NULL)
		{
			WindowManager::getSingleton().destroyWindow(mSubtitleWindow);
			mSubtitleWindow = NULL;
		}
		delete mSoundObject;
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
		mesh->startAnimation("idle");
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
		if (mFadeTextTime >= 0)
		{
			mFadeTextTime -= elapsedTime;
		}

		if (mTextShown && mFadeTextTime <= 0)
		{
			mFadeTextTime = 0;
			mTextShown = false;
			mSubtitleWindow->setVisible(false);
			mDialogWindow->textFinished();
		}

		Logger::getSingleton().log(
				Logger::UI, 
				Logger::LL_TRIVIAL, 
				StringConverter::toString(mSubtitleWindow->getWindow()->getEffectiveAlpha())
				+ ": "
				+ mSubtitleWindow->getText()
				+ CeGuiString(mSubtitleWindow->getWindow()->isVisible() ? "vis" : "unvis")
				+ ", Time: "
				+ StringConverter::toString(mFadeTextTime));
	}

	void DialogCharacterController::setDialogWindow(DialogWindow* dialog)
	{
		mDialogWindow = dialog;
	}

	void DialogCharacterController::setSubtitleWindow(SubtitleWindow* subtitles)
	{
		mSubtitleWindow = subtitles;
	}

	void DialogCharacterController::toggleViewMode()
	{
		/// @todo Krasser Gesichtszoom? Halbtotale... usw?
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

	float DialogCharacterController::getShowTextLength(const CeGuiString& text)
	{
		return 0.1f * text.length() // FIXME: Zeit fürs Textlesen festlegen
			 + 0.25f; // Fade in
	}

	void DialogCharacterController::response(
		Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
	{
		// Drehung zu ZielRichtung  @todo reparieren
		mTargetCameraOrientation = 
			Vector3::UNIT_Z.getRotationTo(
			mTargetCameraPosition - actor->getWorldPosition() );		

		if (soundFile.length() == 0)
		{
			mFadeTextTime = getShowTextLength(text);
		}
		else
		{
			delete mSoundObject;
			mSoundObject = 
				new SoundObject(
					SoundManager::getSingleton().getActiveDriver()->
						createSample(
							SoundManager::getSingleton().getByName(soundFile)),
					soundFile);

			//TODO: An Sprecher hängen?
			mSoundObject->set3d(false);
			mSoundObject->play();

			mFadeTextTime = mSoundObject->getLength();
		}

		Logger::getSingleton().log(
			Logger::UI, 
			Logger::LL_MESSAGE, 
			"Response: "
				+ actor->getName()
				+ " File: '"
				+ soundFile 
				+ "', Text: '" + text + "', Time: "
				+ StringConverter::toString(mFadeTextTime));


		mTextShown = true;

		if (mSubtitleWindow != NULL && !text.empty())
		{
			mSubtitleWindow->show(text);
		}
	}
}
