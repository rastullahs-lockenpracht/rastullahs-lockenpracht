/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __ActionChoiceWindow_H__
#define __ActionChoiceWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include <vector>

namespace rl {

	class Action;
	class GameObject;
	class Person;

	class _RlUiExport ActionChoiceWindow : public CeGuiWindow
	{
	public:
		ActionChoiceWindow(Person* actor);
		~ActionChoiceWindow();
		
		void showActionsOfObject(GameObject* object);
		void showTalentsOfActor();
		
		bool handleActionChosen(Action* action);
		bool handleShowHint(const CeGuiString& evt);

	private:
	
		static CEGUI::Point 
			getPositionOnRadialMenu(
				CEGUI::Point center, 
				float minAngle, 
				float maxAngle,
				float radius,
				int elemNum,
				int elemCount);

		void activateAction(
			Action* action, 
			GameObject* object, 
			Person* actor, 
			GameObject* target);
				
        GameObject* mObject;
		std::vector<CEGUI::PushButton*> mButtons;
		CEGUI::StaticText* mHint;

		enum ACWState
		{
			IDLE = 0,
			CHOOSE_OBJECT_ACTION,
			CHOOSE_TARGET
		};

		ACWState mState;
		Person* mActor;
	};

}

#endif
