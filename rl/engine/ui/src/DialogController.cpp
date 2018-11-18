/*
 * DialogController.cpp
 *
 *  Created on: 15.02.2009
 *      Author: blakharaz
 */

#include "stdinc.h"

#include "DialogController.h"

#include "Actor.h"
#include "ConfigurationManager.h"
#include "Creature.h"
#include "Dialog.h"
#include "DialogOption.h"
#include "DialogParagraph.h"
#include "DialogResponse.h"
#include "DialogWindow.h"
#include "GameObject.h"
#include "MeshObject.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"

using namespace Ogre;

namespace rl
{
    DialogController::DialogController(CommandMapper* commandMapper, Actor* camera, Creature* character,
            ControlStateType type)
        : ControlState(commandMapper, camera, character, type),
        mSubtitleWindow(NULL),
        mSoundObject(NULL),
        mTalkAnimation(NULL),
        mSubtitleSpeed(1.0f),
        mTextShown(false),
        mCurrFadeTextTime(0),
        mTotalFadeTextTime(0),
        mText(),
        mDialogWindow(NULL),
        mCurrentResponseText(""),
        mGameLogger(NULL),
        mCurrentSpeaker(NULL),
        mDialogState(DS_UNKNOWN)
    {
        mSubtitleSpeed = ConfigurationManager::getSingleton().getRealSetting(
            "General", "Subtitle Speed");
    }

    DialogController::~DialogController()
    {
        delete mSoundObject;
    }

    void DialogController::doTalk(DialogParagraph* paragraph, DialogWindow* window)
    {
        if (window)
        {
            window->setVisible(false);
        }
        Ogre::String soundFile = paragraph->getVoiceFile();
        CeGuiString text = paragraph->getText();
        // if there is no text in the paragraph, go directly to the next entry!
        if(text.empty())
        {
            textFinished();
            return;
        }
        processTextVariables(text);

        mCurrentSpeaker = paragraph->getSpeaker(mDialog);
        mCurrentListeners = paragraph->getListeners(mDialog);

        recalculateDialogCamera(mCurrentSpeaker, mCurrentListeners);
        Actor* actor = mCurrentSpeaker->getActor();

        // Ungefaehre Lesedauer bestimmen
        float fadeTime = getShowTextLength(text);
        if (soundFile.length() == 0)
        {
            float speed = mSubtitleSpeed;

            if (mSubtitleSpeed == 0.0)
            {
                speed = 1.0;
            }

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

            // An Sprecher haengen
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

    void DialogController::showResponse(DialogResponse* response, DialogWindow* window)
    {
        if (!response)
        {
            LOG_MESSAGE(Logger::UI, "No response found! Close Dialog!");
            if (window)
            {
                window->setDialogEnd();
            }
            handleDialogEnd();
            return;
        }
        
        mDialogState = DS_SHOWING_RESPONSE;

        if (response->isSelection())
        {
            DialogResponseSelection* sel = static_cast<DialogResponseSelection*>(response);
            mCurrentResponse = sel->getSelectedElement(mDialog);
        }
        else
        {
            mCurrentResponse = response;
        }


        if (window)
        {
            window->setVisible(false);
        }

        mCurrentParagraphs = mCurrentResponse->getParagraphs(mDialog);
        if(!mCurrentParagraphs.empty())
        {
            DialogParagraph* firstParagraph = mCurrentParagraphs.front();
            mCurrentResponse->applyImplications(mDialog);
            // does this paragraph contain a response?
            if (!firstParagraph->getResponse())
            {
                // no, so directly start with the first paragraph
                doTalk(firstParagraph, window);
            }
            else if(mCurrentParagraphs.size() > 1)
            {
                // yes, but there are other paragraphs in the list.
                // the response should be executed at last, so we put it back to the end
                mCurrentParagraphs.pop_front();
                mCurrentParagraphs.push_back(firstParagraph);
                firstParagraph = mCurrentParagraphs.front();
                // we don't allow more than one goto per response
                if(firstParagraph->getResponse())
                {
                    LOG_ERROR(Logger::UI, "To many gotoresponses in response with id: "
                        + mCurrentResponse->getId());
                    handleDialogEnd();
                }
                else
                {
                    doTalk(firstParagraph, window);
                }
            }
            else
            {
                // we only have a response as paragraph, execute it!
                showResponse(firstParagraph->getResponse());
            }
        }
        else
        {
            handleDialogEnd();
        }
    }

    float DialogController::getShowTextLength(const CeGuiString& text) const
    {
        return 0.019f * text.length() + // Zeit fuers Text lesen
               0.25f;                   // Fade in
    }

    void DialogController::processTextVariables(CeGuiString& text)
    {
        CeGuiString newText = text;
        CeGuiString::size_type pos = CeGuiString::npos;
        do
        {
            pos = newText.find("{$");
            if (pos != CeGuiString::npos)
            {
                CeGuiString::size_type endpos = newText.find("}", pos);
                if (endpos != CeGuiString::npos)
                {
                    CeGuiString varName = newText.substr(pos+2, endpos - pos - 2);
                    CeGuiString varValue = mDialog->getVariableValue(varName.c_str());
                    newText = newText.replace(pos, endpos - pos + 1, varValue);
                }
            }
        }
        while (pos != CeGuiString::npos);
        text.assign(newText);
    }

    void DialogController::pause()
    {
        if (mDialogWindow) {
            mDialogWindow->setVisible(false, false);
        }
        if (mSubtitleWindow) {
            mSubtitleWindow->setVisible(false, false);
        }
    }

    bool DialogController::mouseReleased(const OIS::MouseEvent& evt,
        OIS::MouseButtonID id, bool handled)
    {
        bool retval = ControlState::mouseReleased(evt, id, handled);

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

    void DialogController::setDialogWindow(DialogWindow* window)
    {
        mDialogWindow = window;
    }

    void DialogController::setSubtitleWindow(SubtitleWindow* window)
    {
        mSubtitleWindow = window;
    }

    Vector3 DialogController::getParticipantPosition(Creature* participant)
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

    bool DialogController::textFinished()
    {
        if (mDialog->isExitRequested())
        {
            handleDialogEnd();
            return true;
        }

        mCurrentParagraphs.pop_front();
        if (!mCurrentParagraphs.empty())
        {
            DialogParagraph* curParagraph = mCurrentParagraphs.front();
            if (curParagraph->getResponse() == NULL)
            {
                doTalk(curParagraph, mDialogWindow);
            }
            else
            {
                showResponse(curParagraph->getResponse());
            }

            return true;
        }

		return false;
    }
}
