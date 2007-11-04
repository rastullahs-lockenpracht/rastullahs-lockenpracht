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
#include "AiSubsystem.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Dialog.h"
#include "DialogElement.h"
#include "DialogParagraph.h"
#include "DialogOption.h"
#include "DialogResponse.h"
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
		mCurrentResponseText(""),
		mGameLogger(NULL),
        mCurrentSpeaker(NULL),
        mCurrentListener(NULL)
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
        /*
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
        */
        //mCameraActor->getPhysicalThing()->unfreeze();
        //mCharacterActor->getPhysicalThing()->unfreeze();
        mDialogWindow->setVisible(false, false);
        mSubtitleWindow->setVisible(false, false);
    }

    void DialogControlState::resume()
    {
        mCharacterActor->setVisible(true);

        //mCameraActor->getPhysicalThing()->freeze();
        //mCharacterActor->getPhysicalThing()->freeze();
        Camera* ogreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
        ogreCam->setFixedYawAxis(true);
        ogreCam->setPosition(Vector3::ZERO);
        ogreCam->setOrientation(Quaternion::IDENTITY);
        mCameraActor->_getSceneNode()->setFixedYawAxis(true);

        if (!mDialogWindow)
        {
            mDialogWindow = new DialogWindow(this);
        }
        if (!mSubtitleWindow)
        {
            mSubtitleWindow = new SubtitleWindow();
        }
        mDialogWindow->setVisible(true);
        mSubtitleWindow->setVisible(true);

		mGameLogger = WindowFactory::getSingleton().getGameLogger();
    }

	void DialogControlState::start(Dialog* dialog)
	{
		mDialog = dialog;

		//Ogre::String voiceFile = mBot->getVoiceFile().c_str();
		//if (voiceFile != "")
		//{
		//	if (voiceFile.find(".zip") != Ogre::String::npos)
		//	{
		//		ResourceGroupManager::getSingleton().addResourceLocation(
		//			voiceFile, "Zip", voiceFile);
		//	}
		//	else
		//	{
		//		ResourceGroupManager::getSingleton().addResourceLocation(
  //         			voiceFile, "FileSystem", voiceFile);
		//	}
		//}

		if (!mDialog || !mDialogWindow)
		{
			Throw(IllegalStateException, "DialogControlState not properly initialized.");
		}

        mCurrentSpeaker = mDialog->getNpc(0);
        mCurrentListener = mDialog->getPc(0);
        showResponse(mDialog->getDialogStart());
	}

    void DialogControlState::run(Real elapsedTime)
    {
        if (elapsedTime <= 0.0)
            return;

        // Aktuellen Abstand zur gewünschten Position berechnen
        Vector3 posDistance = (mTargetCameraPosition - mCameraActor->_getSceneNode()->getPosition());
        // Ist eine Anpassung nötig?
        if (posDistance != Vector3::ZERO)
        {
            // Die Kameraanpassgeschwindigkeit
            Real speed = 1/elapsedTime;
            mCameraActor->setPosition(mTargetCameraPosition);
        }

        mCameraActor->setOrientation(Quaternion::IDENTITY);
        mCameraActor->_getSceneNode()->setDirection(mTargetCameraDirection);
        mCameraActor->_getSceneNode()->_update(true, false);

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

    void DialogControlState::recalculateCamera(GameObject* speaker, GameObject* listener)
    {
        // Position camera at position between char and dialog partner
        Vector3 speakerEyes = getParticipantPosition(speaker);
        Vector3 listenerEyes = getParticipantPosition(listener);
        Vector3 globalCameraPosition = (speakerEyes + listenerEyes) / 2.0f;

        // Weltkoordinaten in lokale umwandeln
        mTargetCameraPosition =
            -1*(mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldOrientation().Inverse()*
            (mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldPosition() - globalCameraPosition));

        mTargetCameraDirection = (listenerEyes - mTargetCameraPosition).normalisedCopy();
    }

    Vector3 DialogControlState::getParticipantPosition(GameObject* participant)
    {
        Vector3 eyesPosition = participant->getPosition();

        // Modify by MeshBounds
        if (participant->getActor() && participant->getActor()->getControlledObject()->isMeshObject())
        {
            MeshObject* mo = static_cast<MeshObject*>(participant->getActor()->getControlledObject());
            Ogre::AxisAlignedBox aab = mo->getDefaultSize();
            Vector3 offset(
                aab.getCenter().x,
                aab.getMaximum().y * 0.933,
                aab.getCenter().z);
            eyesPosition += participant->getOrientation() * offset;
        }

        return eyesPosition;
    }

    float DialogControlState::getShowTextLength(const CeGuiString& text) const
    {
        return 0.019f * text.length() + // Zeit fürs Text lesen
               0.25f;                   // Fade in
    }

    void DialogControlState::doTalk(DialogParagraph* paragraph)
    {
        mDialogWindow->setVisible(false);
        Ogre::String soundFile = paragraph->getVoiceFile();
        CeGuiString text = paragraph->getText();

        recalculateCamera(mCurrentSpeaker, mCurrentListener);

        Actor* actor = mCurrentSpeaker->getActor();

        // Ungefähre Lesedauer bestimmen
        float fadeTime = getShowTextLength(text);
        if (soundFile.length() == 0)
        {
            float speed = mSubtitleSpeed;

            if (mSubtitleSpeed == 0.0)
                speed = 1.0;

            mCurrFadeTextTime = fadeTime*speed;
            mTotalFadeTextTime = fadeTime*speed;
        }
        else
        {
            if (mSoundObject != NULL)
            {
                mSoundObject->getMovableObject()->getParentSceneNode()->detachObject(
                    mSoundObject->getMovableObject());
                delete mSoundObject;
            }

            mSoundObject = new SoundObject(SoundManager::getSingleton().getActiveDriver()->
                    createSound(soundFile, ST_SAMPLE), soundFile);

            // An Sprecher hängen
            actor->_getSceneNode()->attachObject(mSoundObject->getMovableObject());
            actor->_getSceneNode()->_update(true, false);
            mSoundObject->_setActor(actor);
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
        OIS::MouseButtonID id, bool handled)
    {
        bool retval = false;
        if (ControlState::mouseReleased(evt, id, handled))
            retval = true;

        if (!handled && !retval)
        {
            if (mTextShown && 
                (mCurrFadeTextTime + 0.25 < mTotalFadeTextTime))
            {
                mCurrFadeTextTime = -1;
                retval = true;
            }
        }

        return retval;
    }

    void DialogControlState::showResponse(DialogResponse* response)
	{
        if (!response)
		{
			mDialogWindow->setDialogEnd();
			mState = CLOSING_DIALOG;
			handleDialogClose();
			return;
		}

		if (response->isSelection())
		{
			DialogResponseSelection* sel = static_cast<DialogResponseSelection*>(response);
			mCurrentResponse = sel->getSelectedElement(mDialog);
		}
		else
		{
			mCurrentResponse = response;
		}
	
        mState = TALKING_PARTNER_CHARACTER;
        mCurrentOption = NULL;

		mDialogWindow->setVisible(false);

        mCurrentListener = mCurrentSpeaker;
        mCurrentSpeaker = mCurrentResponse->getNpc(mDialog);
        mCurrentParagraphs = mCurrentResponse->getParagraphs(mDialog);
        DialogParagraph* firstParagraph = mCurrentParagraphs.front();
        mCurrentResponse->applyImplications(mDialog);
        if (!firstParagraph->getResponse())
        {
            doTalk(firstParagraph);
        }
        else
        {
            showResponse(firstParagraph->getResponse());
        }
	}

	void DialogControlState::textFinished()
	{
		if (mDialog->isExitRequested())
		{
			handleDialogClose();
			return;
		}

        mCurrentParagraphs.pop_front();
        if (!mCurrentParagraphs.empty())
        {
            DialogParagraph* curParagraph = mCurrentParagraphs.front();
            if (curParagraph->getResponse() == NULL)
            {
                doTalk(curParagraph);
            }
            else
            {
                showResponse(curParagraph->getResponse());
            }
        }
        else
        {
		    if (mState == TALKING_PARTNER_CHARACTER)
		    {
                mCurrentListener = mCurrentSpeaker;
                mCurrentSpeaker = mCharacter;

                DialogResponse::Options options = mCurrentResponse->getAvailableOptions(mDialog);
                mDialogWindow->setAvailableOptions(options);
		        mDialogWindow->setVisible(true);
		        mState = CHOOSING_OPTION;
		    }
            else if (mState == TALKING_PLAYER_CHARACTER)
            {
                showResponse(mCurrentOption->getResponse());
            }
        }

		LOG_DEBUG(Logger::UI,
					StringConverter::toString(mState)
					+ " bei textFinished");
	}

	bool DialogControlState::handleDialogSelectOption(DialogOption* option)
	{
		if (option->isSelection())
		{
			DialogOptionSelection* sel = static_cast<DialogOptionSelection*>(option);
			mCurrentOption = sel->getSelectedElement(mDialog);
		}
		else
		{
			mCurrentOption = option;
		}
        mState = TALKING_PLAYER_CHARACTER;
        mCurrentListener = mCurrentSpeaker;
        mCurrentSpeaker = mDialog->getPc(0); ///@todo allow char switch 
        mCurrentParagraphs = mCurrentOption->getParagraphs(mDialog);
        doTalk(mCurrentParagraphs.front());

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
		//@todo
		return true;
	}


}
