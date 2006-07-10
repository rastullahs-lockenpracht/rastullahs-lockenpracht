/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#ifndef _TargetSelection_H_
#define _TargetSelection_H_

#include "UiPrerequisites.h"

#include "OgreNewt_Body.h"
#include "OgreNewt_ContactCallback.h"
#include "OgreNewt_MaterialID.h"
#include "OgreNewt_MaterialPair.h"

namespace rl {

	class Actor;

	class _RlUiExport TargetSelection : public OgreNewt::ContactCallback
	{
	public:
		TargetSelection(Actor* character, Actor* camera);
		~TargetSelection();

		std::list<Actor*> getSelectedObjects();

		void TargetSelection::update();

		virtual int userProcess();

	private:
		std::map<Actor*, RL_LONGLONG> mSelectedObjects;
		OgreNewt::Body* mSelectionCollision;
		OgreNewt::MaterialID* mSelectionID;
		OgreNewt::MaterialPair* mDefaultSelectionPair;
		OgreNewt::MaterialPair* mCharSelectionPair;
		Actor* mCharacter;
		Actor* mCamera;
		Ogre::Vector3 mOffset;
		Ogre::Quaternion mOrientationBias;

		void cleanSelection();
	};

} // namespace rl

#endif // _TargetSelection_H_
