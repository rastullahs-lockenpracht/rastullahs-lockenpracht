#ifndef __CharacterSheetWindow_H__
#define __CharacterSheetWindow_H__

#include <CEGUI.h>

#include "UiPrerequisites.h"

namespace rl {

	class Creature;

	class _RlUiExport CharacterSheetWindow
	{
	public:
		CharacterSheetWindow();
		~CharacterSheetWindow();

		void setCharacter(Creature* creature);
		void update();

		void show();
		void hide();

	private:
		static CEGUI::Point TAB_POSITION;
		static CEGUI::Size TAB_SIZE;
		static CEGUI::Size BUTTON_SIZE;

		Creature* mCreature;

		CEGUI::Window* mCharacterSheetWindow;
		CEGUI::Window* mCharacterSheet;
		CEGUI::Window* mTalentSheet;
		CEGUI::Window* mMagicSheet;

		void createCharaterSheet(CEGUI::Window* wndCharSheet, int posButton);
		void createTalentSheet(CEGUI::Window* wndCharSheet, int posButton);
		void createMagicSheet(CEGUI::Window* wndCharSheet, int posButton);

		void showTab(CEGUI::utf8* tabName);
		void showCharacterSheet(const CEGUI::EventArgs& e);
		void showTalentSheet(const CEGUI::EventArgs& e);
		void showMagicSheet(const CEGUI::EventArgs& e);
	};
}


#endif