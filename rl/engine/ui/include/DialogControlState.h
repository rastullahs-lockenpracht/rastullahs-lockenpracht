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
#include "DialogController.h"
#include "Creature.h"

namespace rl {

	class Actor;
	class Dialog;
    class DialogElement;
	class DialogWindow;
	class GameLoggerWindow;
	class MeshAnimation;
	class SoundObject;
	class SubtitleWindow;

	/** Diese Klasse
	  *
	  */
	class _RlUiExport DialogControlState : public DialogController
	{
	public:
        enum DialogMode
		{
            // Frontperspektive auf Augenh�he, ausgehend von der Mitte zwischen den Redenden
			DM_FRONT = 1,
		};


		/**
		*  @throw NullPointerException if camera or character is NULL.
		*  @throw InvalidArgumentException if character is not placed in the scene.
		*/
		DialogControlState(CommandMapper* cmdMapper, Actor* camera, Creature* character);
		/// Dtor
		virtual ~DialogControlState();

        virtual void pause();
        virtual void resume();

		/// @override
		virtual void run(Ogre::Real elapsedTime);

		void start(Dialog* dialog);

		/// Antwort eines der Dialogf�hrenden
		void response(Actor* actor, const CeGuiString& text, const Ogre::String& soundFile = "");

		bool handleDialogSelectOption(DialogOption* option);
		virtual void handleDialogEnd();
		bool requestDialogClose();
	    virtual bool textFinished();

	private:
		/// Die Zielkameraposition in lokalen Koordinaten
		Ogre::Vector3 mTargetCameraPosition;
		/// Die ben�tigte lokale Drehung der Kamera
		Ogre::Vector3 mTargetCameraDirection;


        /// Die Art der Kamerapositionierung
        DialogMode mDialogMode;



		DialogWindow* mDialogWindow;

        virtual void recalculateDialogCamera(Creature* speaker, const CreatureList& listeners);

        void showOptions(const std::list<DialogOption*>& options);
	};
}
#endif
