#ifndef __CharacterSheetWindow_H__
#define __CharacterSheetWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"

namespace rl {

	class Creature;

	class _RlUiExport CharacterSheetWindow : public CeGuiWindow
	{
	public:
		CharacterSheetWindow();
		~CharacterSheetWindow();

		void setCharacter(Creature* creature);

		/**
		 * Aktualisiert die Tabellenelemente in Talent- und Magietabellen
		 * und auf dem Charakterblatt
		 */
		void update();

		/**
		 * Aktualisiert nur die Werte in den Talentlisten
		 * und auf dem Charakterblatt
		 */
		void updateValues();


	private:
		Creature* mCreature;

		CEGUI::Window* mCharacterSheet;
		CEGUI::Window* mTalentSheet;
		CEGUI::Window* mMagicSheet;
		CEGUI::MultiColumnList* mTalentTable;

		void updateTalents();
	};
}


#endif

