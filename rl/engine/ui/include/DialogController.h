/*
 * DialogController.h
 *
 *  Created on: 15.02.2009
 *      Author: blakharaz
 */

#ifndef DIALOGCONTROLLER_H_
#define DIALOGCONTROLLER_H_

#include "AiPrerequisites.h"
#include "ControlState.h"
#include "Creature.h"

namespace rl
{
    class Actor;
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
        enum DialogState
        {
            DS_UNKNOWN = 1,
            DS_CHOOSING_OPTION,
            DS_SHOWING_OPTION,
            DS_SHOWING_RESPONSE,
            DS_CLOSING_DIALOG
        };

        DialogController(CommandMapper* commandMapper, Actor* camera, Creature* character, ControlStateType type);
        virtual ~DialogController();

        void showResponse(DialogResponse* response, DialogWindow* window = NULL);
        void doTalk(DialogParagraph* paragraph, DialogWindow* window = NULL);
        Ogre::Vector3 getParticipantPosition(Creature* participant);
        virtual bool textFinished();
        virtual void recalculateDialogCamera(Creature* speaker, const CreatureList& listeners) = 0;
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
        CreatureList mCurrentListeners;
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
        DialogState mDialogState;

        float getShowTextLength(const CeGuiString& text) const;
        void processTextVariables(CeGuiString& text);
    };
}

#endif /* DIALOGCONTROLLER_H_ */
