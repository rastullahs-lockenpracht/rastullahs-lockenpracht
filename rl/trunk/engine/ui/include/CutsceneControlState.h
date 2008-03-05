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

#ifndef __CutsceneCharacterController_H__
#define __CutsceneCharacterController_H__

#include "UiPrerequisites.h"
#include "ControlState.h"

namespace rl {

	class Actor;

	class _RlUiExport CutsceneControlState : public ControlState
	{
	public:
		/**
		*  @throw NullPointerException if camera is NULL.
		*/
		CutsceneControlState(CommandMapper* cmdMapper, Actor* camera);
		virtual ~CutsceneControlState();

        virtual void pause();
        virtual void resume();

		virtual void run(Ogre::Real elapsedTime);

        void setCameraPosition(const Ogre::Vector3& pos);
        void setCameraOrientation(const Ogre::Quaternion& orient);
        void lookAt(const Ogre::Vector3& point);
	};
}
#endif
