/*
* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "DialogControlState.h"
#include "Exception.h"

#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "DialogCharacter.h"
#include "DialogOption.h"
#include "DialogResponse.h"
#include "DialogSubsystem.h"
#include "DialogWindow.h"
#include "GameLoggerWindow.h"
#include "InputManager.h"
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "Person.h"
#include "PhysicalThing.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

#include <OgreSceneManager.h>
#include <OgreNewt_Body.h>

using namespace Ogre;

namespace rl {

	const CeGuiString DialogControlState::DIALOG_START = "1";
	const CeGuiString DialogControlState::DIALOG_END = "DIALOG BEENDET";
	const CeGuiString DialogControlState::DIALOG_EXIT = "EXIT";

    DialogControlState::DialogControlState(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : ControlState(cmdMapper, camera, character, CST_DIALOG),
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
        mSubtitleSpeed(1.0f),
		mCurrentResponse(NULL),
		mCurrentResponseText(""),
		mBot(NULL),
		mGameLogger(NULL)
    {
        mSubtitleSpeed = ConfigurationManager::getSingleton().getRealSetting(
            "General", "Subtitle Speed");
    }

    DialogControlState::~DialogControlState()
    {
        delete mSoundObject;
    }

    void DialogControlState::pause()
    {
        if (mDialogPartner)
        {
            MeshObject* mesh = dynamic_cast<MeshObject*>(mDialogPartner->getControlledObject());
            if (mesh != NULL)
            {
                mesh->stopAllAnimations();
                if (mesh->hasAnimation("idle"))
                {
                    mTalkAnimation = mesh->startAnimation("idle");
                }
            }
        }
        mCameraActor->getPhysicalThing()->unfreeze();
        mCharacterActor->getPhysicalThing()->unfreeze();
        mDialogWindow->setVisible(false, false);
        mSubtitleWindow->setVisible(false, false);
    }

    void DialogControlState::resume()
    {
        mCharacterActor->setVisible(true);

        mCameraActor->getPhysicalThing()->freeze();
        mCharacterActor->getPhysicalThing()->freeze();
        Camera* ogreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
        ogreCam->setFixedYawAxis(true);
        ogreCam->setPosition(Vector3::ZERO);
        ogreCam->setOrientation(Quaternion::IDENTITY);
        mCameraActor->_getSceneNode()->setFixedYawAxis(true);

        mDialogWindow = new DialogWindow(this);
        mSubtitleWindow = new SubtitleWindow();

		mGameLogger = WindowFactory::getSingleton().getGameLogger();

		DialogCharacter* bot = DialogSubsystem::getSingleton().getCurrentDialogCharacter();
		initialize(bot);
    }

	void DialogControlState::initialize(DialogCharacter* bot)
	{
		mBot = bot;
        mBot->setDialogCharacter(mCharacter);
		mDialogPartner = mBot->getDialogPartner()->getActor();

        recalculateCamera( mDialogPartner, mCharacterActor );

		Ogre::String voiceFile = mBot->getVoiceFile().c_str();
		if (voiceFile != "")
		{
			if (voiceFile.find(".zip") != Ogre::String::npos)
			{
				ResourceGroupManager::getSingleton().addResourceLocation(
					voiceFile, "Zip", voiceFile);
			}
			else
			{
				ResourceGroupManager::getSingleton().addResourceLocation(
           			voiceFile, "FileSystem", voiceFile);
			}
		}

		start();
	}

    void DialogControlState::run(Real elapsedTime)
    {
        if ( elapsedTime <= 0.0 )
            return;

        // Aktuellen Abstand zur gewünschten Position berechnen
        Vector3 posDistance = (mTargetCameraPosition - mCameraActor->_getSceneNode()->getPosition());
        // Ist eine Anpassung nötig?
        if ( posDistance != Vector3::ZERO )
        {
            // Die Kameraanpassgeschwindigkeit
            Real speed = 1/elapsedTime;
            mCameraActor->setPosition(mTargetCameraPosition);
        }

        mCameraActor->setOrientation( Quaternion::IDENTITY );
        mCameraActor->_getSceneNode()->setDirection( mTargetCameraDirection );
        mCameraActor->_getSceneNode()->_update( true, false );

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
            mSubtitleWindow->setVisible(false, false);
            textFinished();
        }

        LOG_DEBUG(Logger::UI,
                StringConverter::toString(mSubtitleWindow->getWindow()->getEffectiveAlpha())
                + ": "
                + mSubtitleWindow->getText()
                + CeGuiString(mSubtitleWindow->getWindow()->isVisible() ? "vis" : "unvis")
                + ", Time: "
                + StringConverter::toString(mCurrFadeTextTime));
    }

    void DialogControlState::recalculateCamera( Actor* speaker, Actor* listener )
    {
        // Position camera at position between char and dialog partner
        Vector3 charEyes = speaker->getWorldPosition();
        // Modify by MeshBounds
        if ( speaker->getControlledObject()->isMeshObject() )
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
        if ( listener->getControlledObject()->isMeshObject() )
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
            -1*(mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldOrientation().Inverse()*
            (mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldPosition() - globalCameraPosition));


        mTargetCameraDirection = ( partEyes - mTargetCameraPosition ).normalisedCopy();
    }

    float DialogControlState::getShowTextLength(const CeGuiString& text) const
    {
        return 0.019f * text.length() + // Zeit fürs Text lesen
               0.25f;                   // Fade in
    }

    void DialogControlState::response(
        Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
    {
        if ( actor == mDialogPartner )
            recalculateCamera( mCharacterActor, mDialogPartner );
        else
            recalculateCamera( mDialogPartner, mCharacterActor );

        // Ungefähre Lesedauer bestimmen
        float fadeTime = getShowTextLength(text);
        if (soundFile.length() == 0)
        {
            float speed = mSubtitleSpeed;

            if ( mSubtitleSpeed == 0.0 )
                speed = 1.0;

            mCurrFadeTextTime = fadeTime*speed;
            mTotalFadeTextTime = fadeTime*speed;
        }
        else
        {
            if ( mSoundObject != NULL )
                mSoundObject->getMovableObject()->getParentSceneNode()->detachObject(
                    mSoundObject->getMovableObject() );

            delete mSoundObject;
            mSoundObject = new SoundObject(SoundManager::getSingleton().getActiveDriver()->
                    createSound(soundFile, ST_SAMPLE), soundFile);

            // An Sprecher hängen
            actor->_getSceneNode()->attachObject( mSoundObject->getMovableObject() );
            actor->_getSceneNode()->_update( true, false );
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

        LOG_DEBUG(Logger::UI,
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

    bool DialogControlState::mouseReleased(const OIS::MouseEvent& evt,
        OIS::MouseButtonID id)
    {

        if (mTextShown && (mCurrFadeTextTime + 0.25) < mTotalFadeTextTime)
        {
            mCurrFadeTextTime = -1;

            return true;
        }
        else
        {
            return false;
        }
    }

	void DialogControlState::start()
	{
		if (!mBot || !mDialogWindow)
		{
			Throw(IllegalStateException, "DialogControlState not properly initialized.");
		}

		mDialogWindow->setName(mBot->getName());
		getResponse(DIALOG_START);
	}

	void DialogControlState::getResponse(const CeGuiString& msg)
	{
		delete mCurrentResponse;
		mCurrentResponse = mBot->createResponse(msg);

		if (mBot->hasExitRequest())
		{
			mDialogWindow->setDialogEnd();
			mState = CLOSING_DIALOG;
			handleDialogClose();
			return;
		}
		if (mCurrentResponse == NULL)
		{
			mDialogWindow->setDialogEnd();
			mState = CLOSING_DIALOG;
		//	Übergangslösung, wenn gerade kein sprecher aktiv ist wird
		//	nicht nicht textFinished aufgerufen
		//  hier müsste wahrscheinlich requestClose auftauchen
			handleDialogClose();
			return;
		}

		DialogResponse::Responses responses = mCurrentResponse->getResponses();
		CeGuiString responseSound = "null.ogg";
		CeGuiString responseText;
		if (!responses.empty())
		{
			responseSound = responses.begin()->first;
			responseText = responses.begin()->second;
			responseText.c_str();
			responseSound.c_str();
		}

		response(mBot->getDialogPartner()->getActor(),
				responseText, responseSound.c_str());

		if (!responseText.empty())
		{
			mDialogWindow->setResponse(responseText);

			mGameLogger->logDialogEvent(mBot->getName(), responseText);
			LOG_MESSAGE(Logger::DIALOG, mBot->getName() + " says: " + responseText);
		}

		mDialogWindow->setVisible(false);
		mState = TALKING_PARTNER_CHARACTER;
		mCurrentResponseText = msg;
	}

	void DialogControlState::textFinished()
	{
		if (mState == TALKING_PARTNER_CHARACTER)
		{
			getOptions(mCurrentResponseText);
			mDialogWindow->setVisible(true);
			mState = CHOOSING_OPTION;
		}

		if (mState == TALKING_PLAYER_CHARACTER)
		{
			getResponse(mCurrentResponseText);
		}

		LOG_DEBUG(Logger::UI,
					StringConverter::toString(mState)
					+ " bei textFinished" );
	}

	void DialogControlState::getOptions(const CeGuiString& question)
	{
		if (mCurrentResponse == NULL)
		{
			Throw(rl::IllegalStateException, "mCurrentResponse must not be NULL.");
			return;
		}

		DialogResponse::DialogOptions options = mCurrentResponse->getDialogOptions();

		if (options.empty())
		{
			mDialogWindow->setDialogEnd();
			handleDialogClose();
			return;
		}

		//mDialogOptions->clearAllSelections();

		CeGuiStringVector optionTexts;
		for(DialogResponse::DialogOptions::const_iterator itr = options.begin();
			itr != options.end(); ++itr)
		{
			CeGuiString currentOption = (*itr)->getText();
			LOG_DEBUG(Logger::UI, "Player option: " + currentOption);

			optionTexts.push_back(currentOption);
			///@todo mark attribute/talent checks
		}

		mDialogWindow->setAvailableOptions(optionTexts);
	}

	bool DialogControlState::handleDialogSelectOption()
	{
		int idx = mDialogWindow->getSelectedOptionIndex();
		if (idx != -1)
		{
			DialogOption* option = mCurrentResponse->getDialogOptions()[idx];
			option->processSelection();
			mCurrentResponseText = option->getPattern();
			CeGuiString selectedOption = option->getText();
			if (mCurrentResponseText != "0" && mCurrentResponseText != "666")
			{
				if (!selectedOption.empty())
				{
					mState = TALKING_PLAYER_CHARACTER;
					mGameLogger->logDialogEvent("Held", selectedOption);
					LOG_MESSAGE(Logger::DIALOG,
						"Player says: " + selectedOption);
					mDialogWindow->setChosenOption(selectedOption);
					response(
						mBot->getDialogCharacter()->getActor(),
						selectedOption,
						option->getId().c_str());
					mDialogWindow->setVisible(false);
				}
			}
		}

		return true;
	}

	bool DialogControlState::handleDialogClose()
	{
		InputManager::getSingleton().popControlState();
		mDialogWindow->setVisible(false, true);
		mSubtitleWindow->setVisible(false, true);
		return true;
	}

	bool DialogControlState::requestDialogClose()
	{
	//	handleClose is called automatically
		getResponse(DIALOG_EXIT);
		return true;
	}


}
