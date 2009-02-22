/*
* This source file is part of Rastullahs Lockenpracht.
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
#include "stdinc.h" //precompiled header

#include "DialogControlState.h"
#include "Exception.h"

#include "Actor.h"
#include "AiSubsystem.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
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
#include "PhysicalThing.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

using namespace Ogre;
using namespace std;

namespace rl {

    DialogControlState::DialogControlState(CommandMapper* cmdMapper,
        Actor* camera, Creature* character)
        : DialogController(cmdMapper, camera, character, CST_DIALOG),
        mTargetCameraPosition(Vector3::ZERO),
        mTargetCameraDirection(Vector3::UNIT_Z),
        mDialogMode(DM_FRONT)
    {
    }

    DialogControlState::~DialogControlState()
    {
    }

    void DialogControlState::pause()
    {
        DialogController::pause();
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
            LOG_ERROR(Logger::UI, "DialogControlState not properly initialized.");
            handleDialogEnd();
            return;
		}
        mDialog->setProperty(Dialog::PROP_EXIT_REQUESTED, false);
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

    void DialogControlState::recalculateDialogCamera(Creature* speaker, std::list<Creature*> listeners)
    {
        // Position camera at position between char and dialog partner
        Vector3 speakerEyes = getParticipantPosition(speaker);
        Vector3 listenerEyes = getParticipantPosition(listeners.front()); ///@FIXME, this is just using the 1st listener
        Vector3 globalCameraPosition = (speakerEyes + listenerEyes) / 2.0f;

        // Weltkoordinaten in lokale umwandeln
        mTargetCameraPosition =
            -1*(mCameraActor->_getSceneNode()->getParentSceneNode()->_getDerivedOrientation().Inverse()*
            (mCameraActor->_getSceneNode()->getParentSceneNode()->_getDerivedPosition() - globalCameraPosition));

        mTargetCameraDirection = (listenerEyes - mTargetCameraPosition).normalisedCopy();
    }


	void DialogControlState::handleDialogEnd()
	{
		InputManager::getSingleton().popControlState();
		mDialogWindow->setVisible(false, true);
		mSubtitleWindow->setVisible(false, true);
        mDialogWindow = NULL;
        mSubtitleWindow = NULL;
        if (mDialog != NULL)
        {
            const list<Creature*> list = mDialog->getParticipants();
            for (std::list<Creature*>::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                Actor* actor = (*it)->getActor();
                if (actor != NULL)
                {
                    MeshObject* mesh = dynamic_cast<MeshObject*>(actor->getControlledObject());
                    if (mesh != NULL && mesh->hasAnimation("reden"))
                    {
                        mesh->stopAnimation("reden");
                    }
                }
            }
        }
        else
        {
            LOG_ERROR(Logger::UI, "There is no Dialog to be closed!");
        }
	}

	bool DialogControlState::requestDialogClose()
	{
	//	handleClose is called automatically
        handleDialogEnd();
		//@todo
		return true;
	}


	bool DialogControlState::textFinished()
	{

        if (!DialogController::textFinished())
        {
            if (mDialogState == TALKING_PARTNER_CHARACTER)
            {

                DialogResponse::Options options = mCurrentResponse->getAvailableOptions(mDialog);
                // if there is just one option and it is an auto selected one,
                // display it.
                if (options.size() == 1 &&  options.front()->isAutoSelected())
                {
                    handleDialogSelectOption(options.front());
                    return true;
                }
                mDialogWindow->setAvailableOptions(options);
                mDialogWindow->setVisible(true);
                mDialogState = CHOOSING_OPTION;
            }
            else if (mDialogState == TALKING_PLAYER_CHARACTER)
            {
                showResponse(mCurrentOption->getResponse());
            }
        }

        LOG_DEBUG(Logger::UI,
                    StringConverter::toString(mDialogState)
                    + " bei textFinished");

        return true;
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

        mDialogState = TALKING_PLAYER_CHARACTER;
        mCurrentParagraphs = mCurrentOption->getParagraphs(mDialog);
        doTalk(mCurrentParagraphs.front());

        return true;
    }

}
