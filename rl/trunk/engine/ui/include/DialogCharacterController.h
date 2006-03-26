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

#include <OgreNewt_Body.h>
#include <OgreCamera.h>

namespace rl {

	class Actor;
	class DialogCharacter;

	/**
	* This class provides a no-clip free fly controller.
	*/
	class _RlUiExport DialogCharacterController : public CharacterController
	{
	public:
		/**
		*  @throw NullPointerException if camera or character is NULL.
		*  @throw InvalidArgumentException if character is not placed in the scene.
		*/
		DialogCharacterController(Actor* camera, Actor* character);
		virtual ~DialogCharacterController();

		void run(Ogre::Real elapsedTime);

		void toggleViewMode();
		void resetCamera();

		void setDialogPartner(Actor* partner);

		void response(
			Actor* actor, const CeGuiString& text, const Ogre::String& soundFile);

	private:
		Ogre::Real mTime;
		CeGuiString mText;

		Ogre::Camera* mOgreCam;

		Actor* mDialogPartner;
		Actor* mCurrentActor;
	};
}
#endif
