/* This source file is part of Rastullahs Lockenpracht.
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

#ifndef __DialogCharacterController_H__
#define __DialogCharacterController_H__

#include "UiPrerequisites.h"
#include "ControlState.h"


namespace rl {

	class Actor;
	class Dialog;
    class DialogElement;
	class DialogOption;
	class DialogParagraph;
	class DialogResponse;
	class DialogWindow;
	class GameLoggerWindow;
    class GameObject;
	class MeshAnimation;
	class SoundObject;
	class SubtitleWindow;

	/** Diese Klasse 
	  *  
	  */
	class _RlUiExport DialogControlState : public ControlState
	{
	public:
        enum DialogMode
		{ 
            // Frontperspektive auf Augenhöhe, ausgehend von der Mitte zwischen den Redenden
			DM_FRONT = 1,
		};


		/**
		*  @throw NullPointerException if camera or character is NULL.
		*  @throw InvalidArgumentException if character is not placed in the scene.
		*/
		DialogControlState(CommandMapper* cmdMapper, Actor* camera, Person* character);
		/// Dtor 
		virtual ~DialogControlState();

        virtual void pause();
        virtual void resume();

		/// @override
		virtual void run(Ogre::Real elapsedTime);

		void start(Dialog* dialog);

		/// Antwort eines der Dialogführenden
		void response(Actor* actor, const CeGuiString& text, const Ogre::String& soundFile = "");

        virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled);

		bool handleDialogSelectOption(DialogOption* option);	
		bool handleDialogClose();
		bool requestDialogClose();

	private:
		/// Die Zielkameraposition in lokalen Koordinaten
		Ogre::Vector3 mTargetCameraPosition;
		/// Die benötigte lokale Drehung der Kamera
		Ogre::Vector3 mTargetCameraDirection;

		/// Die aktuelle Zeit für die Textanzeige
		Ogre::Real mCurrFadeTextTime;
        /// Die Zeit bis der Text ausgeblendet wird
        Ogre::Real mTotalFadeTextTime;

        Ogre::Real mSubtitleSpeed;

		/// Der Untertitel-Text
		CeGuiString mText;
		/// Es wird gerade Text angezeigt
		bool mTextShown;

		GameObject* mCurrentSpeaker;
		GameObject* mCurrentListener;
        DialogResponse* mCurrentResponse;
        DialogOption* mCurrentOption;
        std::list<DialogParagraph*> mCurrentParagraphs;

        /// Die Art der Kamerapositinierung
        DialogMode mDialogMode;

		MeshAnimation* mTalkAnimation;

		DialogWindow* mDialogWindow;
		SubtitleWindow* mSubtitleWindow;
		SoundObject* mSoundObject;
	
		float getShowTextLength(const CeGuiString& text) const;
        void recalculateCamera(GameObject* speaker, GameObject* listener);
        void processTextVariables(CeGuiString& text);

		enum DialogState
		{
			CHOOSING_OPTION = 1,
			TALKING_PARTNER_CHARACTER,
			TALKING_PLAYER_CHARACTER,
			CLOSING_DIALOG
		};

		Dialog* mDialog;
		GameLoggerWindow* mGameLogger;
		DialogState mState;
		CeGuiString mCurrentResponseText;
				
        void showResponse(DialogResponse* response);
        void showOptions(const std::list<DialogOption*>& options);
        void doTalk(DialogParagraph* paragraph);
        Ogre::Vector3 getParticipantPosition(GameObject* participant);

		void textFinished();
	};
}
#endif
