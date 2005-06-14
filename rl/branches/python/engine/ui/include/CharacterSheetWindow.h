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
		CEGUI::StaticText* mName;
		CEGUI::StaticText* mRasse;
		CEGUI::StaticText* mProfession;
		
		void updateTalents();
	};
}


#endif

