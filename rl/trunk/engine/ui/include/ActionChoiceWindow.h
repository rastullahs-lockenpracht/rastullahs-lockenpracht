#ifndef __ActionChoiceWindow_H__
#define __ActionChoiceWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include <vector>

namespace rl {

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
