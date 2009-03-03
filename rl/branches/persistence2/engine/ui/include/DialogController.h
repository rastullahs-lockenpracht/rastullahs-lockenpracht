/*
 * DialogController.h
 *
 *  Created on: 15.02.2009
 *      Author: blakharaz
 */

#ifndef DIALOGCONTROLLER_H_
#define DIALOGCONTROLLER_H_

#include "AiPrerequisites.h"

#include <list>

#include "ControlState.h"

namespace rl
{
    class Actor;
    class Creature;
    class Dialog;
    class DialogOption;
    class DialogParagraph;
    class DialogResponse;
    class DialogWindow;
    class GameLoggerWindow;
    class MeshAnimation;
    class SoundObject;
    class SubtitleWindow;

    class DialogController : public ControlState
    {
    public:
        virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled);

    protected:
        DialogController(CommandMapper* commandMapper, Actor* camera, Creature* character,
                ControlStateType type);
        virtual ~DialogController();

        void showResponse(DialogResponse* response, DialogWindow* window = NULL);
        void doTalk(DialogParagraph* paragraph, DialogWindow* window = NULL);
        Ogre::Vector3 getParticipantPosition(Creature* participant);
        virtual bool textFinished();
        virtual void recalculateDialogCamera(Creature* speaker, std::list<Creature*> listeners) = 0;
        virtual void handleDialogEnd() = 0;
        void setDialogWindow(DialogWindow* window);
        void setSubtitleWindow(SubtitleWindow* window);
        virtual void pause();

         SubtitleWindow* mSubtitleWindow;
        DialogWindow* mDialogWindow;
        SoundObject* mSoundObject;
        MeshAnimation* mTalkAnimation;
        /// Die aktuelle Zeit f�r die Textanzeige
        Ogre::Real mCurrFadeTextTime;
        /// Die Zeit bis der Text ausgeblendet wird
        Ogre::Real mTotalFadeTextTime;
        Ogre::Real mSubtitleSpeed;
        Creature* mCurrentSpeaker;
        std::list<Creature*> mCurrentListeners;
        /// Der Untertitel-Text
        CeGuiString mText;
        /// Es wird gerade Text angezeigt
        bool mTextShown;

        DialogResponse* mCurrentResponse;
        DialogOption* mCurrentOption;
        std::list<DialogParagraph*> mCurrentParagraphs;

        Dialog* mDialog;
        GameLoggerWindow* mGameLogger;
        CeGuiString mCurrentResponseText;

        float getShowTextLength(const CeGuiString& text) const;
        void processTextVariables(CeGuiString& text);
    };
}


#endif /* DIALOGCONTROLLER_H_ */
