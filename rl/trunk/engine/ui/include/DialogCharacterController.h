/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __DialogCharacterController_H__
#define __DialogCharacterController_H__

#include "UiPrerequisites.h"
#include "CharacterController.h"

#include <OgreCamera.h>

namespace rl {

	class Actor;
	class DialogWindow;
	class MeshAnimation;
	class SoundObject;
	class SubtitleWindow;

	/** Diese Klasse 
	  *  
	  */
	class _RlUiExport DialogCharacterController : public CharacterController
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
		DialogCharacterController(CommandMapper* cmdMapper, Actor* camera, Person* character);
		/// Dtor 
		virtual ~DialogCharacterController();

        virtual void pause();
        virtual void resume();

		/// @override
		virtual void run(Ogre::Real elapsedTime);

		/// Setzt den Dialogpartner (Diealogführenden Spieler-Actor)
		void setDialogPartner(Actor* partner);

		/// Antwort eines der Dialogführenden
		void response(
			Actor* actor, const CeGuiString& text, const Ogre::String& soundFile = "");

		void setDialogWindow(DialogWindow* dialog);
		void setSubtitleWindow(SubtitleWindow* subtitles);
	
		bool injectMouseUp(int mouseButtonMask);
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

		/// Der Untertitel Text
		CeGuiString mText;
		/// Es wird gerade Text angezeigt
		bool mTextShown;

		/// Der Spieler der redet
		Actor* mDialogPartner;
		/// Der Besitzer des Dialoges (Der Bauer mit dem der Spieler redet)
		Actor* mCurrentActor;
        /// Die Art der Kamerapositinierung
        DialogMode mDialogMode;

		MeshAnimation* mTalkAnimation;

		DialogWindow* mDialogWindow;
		SubtitleWindow* mSubtitleWindow;
		SoundObject* mSoundObject;
	
		float getShowTextLength(const CeGuiString& text) const;
        void recalculateCamera( Actor* speaker, Actor* listener );
	};
}
#endif
