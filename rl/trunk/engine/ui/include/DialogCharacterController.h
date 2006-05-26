/* This source file is part of Rastullahs Lockenpracht.
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

#ifndef __DialogCharacterController_H__
#define __DialogCharacterController_H__

#include "UiPrerequisites.h"
#include "CharacterController.h"

#include <OgreCamera.h>

namespace rl {

	class Actor;
	class DialogWindow;
	class SoundObject;
	class SubtitleWindow;

	/** Diese Klasse 
	  *  
	  */
	class _RlUiExport DialogCharacterController : public CharacterController
	{
	public:
		/**
		*  @throw NullPointerException if camera or character is NULL.
		*  @throw InvalidArgumentException if character is not placed in the scene.
		*/
		DialogCharacterController(Actor* camera, Actor* character);
		/// Dtor 
		virtual ~DialogCharacterController();
		virtual ControllerType getType() const;

		/// @override
		virtual void run(Ogre::Real elapsedTime);

		/// @override
		virtual void toggleViewMode();
		/// @override 
		virtual void resetCamera();

		/// Setzt den Dialogpartner (Diealogführenden Spieler-Actor)
		void setDialogPartner(Actor* partner);

		/// Antwort eines der Dialogführenden
		void response(
			Actor* actor, const CeGuiString& text, const Ogre::String& soundFile = "");

		void setDialogWindow(DialogWindow* dialog);
		void setSubtitleWindow(SubtitleWindow* subtitles);
	
	private:
		/// Die Zielkameraposition in lokalen Koordinaten
		Ogre::Vector3 mTargetCameraPosition;
		/// Die benötigte lokale Drehung der Kamera
		Ogre::Quaternion mTargetCameraOrientation;
		/// Harter Schnitt zur nächsten Kameraposition
		bool mCutHard;


		/// Die Zeit bis der Text ausgeblendet wird
		Ogre::Real mFadeTextTime;
		/// Der Untertitel Text
		CeGuiString mText;

		/// Der Spieler der redet
		Actor* mDialogPartner;
		/// Der Besitzer des Dialoges (Der Bauer mit dem der Spieler redet)
		Actor* mCurrentActor;

		DialogWindow* mDialogWindow;
		SubtitleWindow* mSubtitleWindow;
		SoundObject* mSoundObject;
	
	};
}
#endif
