#ifndef __ActionChoiceWindow_H__
#define __ActionChoiceWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include <vector>

namespace rl {

	class GameObject;
	class Creature;

	class _RlUiExport ActionChoiceWindow : public CeGuiWindow
	{
	public:
		ActionChoiceWindow();
		~ActionChoiceWindow();
		
		void showActionsOfObject(GameObject* object);
		void showTalentsOfPerson(Creature* creature);
		
		void handleActionChosen(const CeGuiString& action);

		void handleShowHint(const CEGUI::EventArgs& evt);
		void handleRemoveHint();

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
	};

}

#endif
