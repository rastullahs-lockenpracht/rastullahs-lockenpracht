#ifndef __ActionChoiceWindow_H__
#define __ActionChoiceWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {

	class GameObject;

	class _RlUiExport ActionChoiceWindow : public CeGuiWindow
	{
	public:
		ActionChoiceWindow();
		~ActionChoiceWindow();
		
		void setActionObject(GameObject* object);
		void handleActionChosen(std::string& action);

		void handleShowHint(const CEGUI::EventArgs& evt);
		void handleRemoveHint();

	private:
        GameObject* mObject;
		std::vector<CEGUI::PushButton*> mButtons;
	};

}

#endif
