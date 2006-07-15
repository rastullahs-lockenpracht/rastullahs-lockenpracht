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
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "PhysicalThing.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"
#include "WindowManager.h"
#include "World.h"
#include "ConfigurationManager.h"

#include <OgreSceneManager.h>
#include <OgreNewt_Body.h>

using namespace Ogre;

namespace rl {

	DialogCharacterController::DialogCharacterController(Actor* camera, Actor* character)
		: CharacterController(camera, character),
		mTargetCameraPosition(Vector3::ZERO),
        mTargetCameraDirection(Vector3::UNIT_Z),
		mCurrFadeTextTime(0),
        mTotalFadeTextTime(0),
        mText(),
        mDialogMode(DM_FRONT),
        mTextShown(false),
		mDialogWindow(NULL),
		mSubtitleWindow(NULL),
		mSoundObject(NULL),
		mTalkAnimation(NULL),
        mSubtitleSpeed(1.0f)
	{
		mCamera->getPhysicalThing()->freeze();
		mCharacterActor->getPhysicalThing()->freeze();		
		Camera* ogreCam = static_cast<Camera*>(mCamera->_getMovableObject());
		ogreCam->setFixedYawAxis(true);
		ogreCam->setPosition(Vector3::ZERO);	
		ogreCam->setOrientation(Quaternion::IDENTITY);
		mCamera->_getSceneNode()->setFixedYawAxis(true);

        // Aktuelle Objektanwahl entfernen
        InputManager::getSingleton().setObjectPickingActive(false);

        mSubtitleSpeed = ConfigurationManager::getSingleton().getRealSetting( "subtitle_speed", 1.0 );
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
		}

        mCamera->setOrientation( Quaternion::IDENTITY );
        mCamera->_getSceneNode()->setDirection( mTargetCameraDirection );

		// Textanzeigedauer
		if (mCurrFadeTextTime >= 0)
		{
			mCurrFadeTextTime -= elapsedTime;
		}

		if (mTextShown && mCurrFadeTextTime <= 0)
		{
			mCurrFadeTextTime = 0;
            mTotalFadeTextTime = 0;
			mTextShown = false;
   			if (mSoundObject)
            {
                mSoundObject->stop();
            }
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
				+ StringConverter::toString(mCurrFadeTextTime));
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
		recalculateCamera( mDialogPartner, mCharacterActor );
	}

    void DialogCharacterController::recalculateCamera( Actor* speaker, Actor* listener )
    {
        // Position camera at position between char and dialog partner
        Vector3 charEyes = speaker->getWorldPosition();
        // Modify by MeshBounds
        if( speaker->getControlledObject()->isMeshObject() )
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(speaker->getControlledObject());
            Ogre::AxisAlignedBox aab = mo->getDefaultSize();
            Vector3 offset( 
                aab.getCenter().x, 
                aab.getMaximum().y*0.933, 
                aab.getCenter().z );
            charEyes += speaker->getWorldOrientation()*offset;
        }        

        Vector3 partEyes = listener->getWorldPosition();        
        // Modify by MeshBounds
        if( listener->getControlledObject()->isMeshObject() )
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(listener->getControlledObject());
            Ogre::AxisAlignedBox aab = mo->getDefaultSize();
            Vector3 offset( 
                aab.getCenter().x, 
                aab.getMaximum().y*0.933, 
                aab.getCenter().z );
            partEyes += listener->getWorldOrientation()*offset;
        }

		Vector3 globalCameraPosition = ( charEyes + partEyes ) / 2.0f;

		// Weltkoordinaten in lokale umwandeln
		mTargetCameraPosition = 
			-1*(mCamera->_getSceneNode()->getParentSceneNode()->getWorldOrientation().Inverse()*
			(mCamera->_getSceneNode()->getParentSceneNode()->getWorldPosition() - globalCameraPosition));


        mTargetCameraDirection = ( partEyes - mTargetCameraPosition ).normalisedCopy();
    }

	float DialogCharacterController::getShowTextLength(const CeGuiString& text) const
	{
		return 0.019f * text.length() + // Zeit fürs Text lesen
			   0.25f;                   // Fade in
	}

	void DialogCharacterController::response(
		Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
	{
        if( actor == mDialogPartner )
		    recalculateCamera( mCharacterActor, mDialogPartner );
        else
            recalculateCamera( mDialogPartner, mCharacterActor );

        // Ungefähre Lesedauer bestimmen
        float fadeTime = getShowTextLength(text);
		if (soundFile.length() == 0)
		{
            float speed = mSubtitleSpeed;

            if( mSubtitleSpeed == 0.0 )
                speed = 1.0;

            mCurrFadeTextTime = fadeTime*speed;
            mTotalFadeTextTime = fadeTime*speed;
		}
		else
		{
            if( mSoundObject != NULL )
                mSoundObject->getMovableObject()->getParentSceneNode()->detachObject(
                    mSoundObject->getMovableObject() );

			delete mSoundObject;
			mSoundObject = 
				new SoundObject(
					SoundManager::getSingleton().getActiveDriver()->
						createSample(
							SoundManager::getSingleton().getByName(soundFile)),
					soundFile);

			// An Sprecher hängen
            actor->_getSceneNode()->attachObject( mSoundObject->getMovableObject() );
            mSoundObject->_setActor( actor );
			mSoundObject->set3d(true);
			mSoundObject->play();
            mSoundObject->_update();

            mCurrFadeTextTime = std::max(fadeTime*mSubtitleSpeed,mSoundObject->getLength());
            mTotalFadeTextTime = mCurrFadeTextTime;
		}

		MeshObject* mesh = dynamic_cast<MeshObject*>(actor->getControlledObject());
		if (mesh != NULL)
		{
			if (mesh->hasAnimation("reden"))
			{
				mesh->stopAllAnimations();
				mTalkAnimation = mesh->startAnimation("reden");
			}
		}

		Logger::getSingleton().log(
			Logger::UI, 
			Logger::LL_MESSAGE, 
			"Response: "
				+ actor->getName()
				+ " File: '"
				+ soundFile 
				+ "', Text: '" + text + "', Time: "
				+ StringConverter::toString(mCurrFadeTextTime));


		mTextShown = true;

		if (mSubtitleWindow != NULL && !text.empty())
		{
			mSubtitleWindow->show(text);
		}
	}

    bool DialogCharacterController::injectMouseUp(int mouseButtonMask)
    {
        
        if( mTextShown && (mCurrFadeTextTime+0.25)<mTotalFadeTextTime )
        {
            mCurrFadeTextTime = -1;

            return true;
        }
        else
        {
            return false;
        }

    }
}
