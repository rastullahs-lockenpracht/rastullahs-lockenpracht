#ifndef __CharacterSheetWindow_H__
#define __CharacterSheetWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include "Eigenschaft.h"

namespace rl {

	class Person;

	class _RlUiExport CharacterSheetWindow : public CeGuiWindow
	{
	public:
		CharacterSheetWindow();
		~CharacterSheetWindow();

		void setCharacter(Person* character);

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
		Person* mCharacter;

		CEGUI::Window* mCharacterSheet;
		CEGUI::Window* mTalentSheet;
		CEGUI::Window* mMagicSheet;
		CEGUI::MultiColumnList* mTalentTable;
		
		CEGUI::StaticText* mEigenschaft[EIGENSCHAFT_COUNT];
		CEGUI::StaticText* mLE;
		CEGUI::StaticText* mAE;
		CEGUI::StaticText* mAP;
		
		void updateTalents();
	};
}


#endif

