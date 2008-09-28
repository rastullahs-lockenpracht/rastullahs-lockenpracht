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

#ifndef __CharacterSheetWindow_H__
#define __CharacterSheetWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"
#include "Eigenschaft.h"
#include "ObjectStateChangeListener.h"

namespace rl {

	class Creature;
	class ObjectStateChangeEvent;

	class _RlUiExport CharacterSheetWindow : public AbstractWindow, public ObjectStateChangeListener
	{
	public:
		CharacterSheetWindow();
		~CharacterSheetWindow();

		void setCharacter(Creature* character);

		/**
		 * Aktualisiert die Tabellenelemente in Talent- und Magietabellen
		 * und auf dem Charakterblatt
		 */
		void update();

		void objectStateChanged(ObjectStateChangeEvent* evt);

	private:
		Creature* mCharacter;

		CEGUI::Window* mCharacterSheet;
		CEGUI::Window* mTalentSheet;
		CEGUI::Window* mMagicSheet;
		CEGUI::MultiColumnList* mTalentTable;
		
		CEGUI::Window* mEigenschaft[EIGENSCHAFT_COUNT];
		CEGUI::Window* mLE;
		CEGUI::Window* mAE;
		CEGUI::Window* mAP;
		CEGUI::Window* mName;
		CEGUI::Window* mRasse;
        CEGUI::Window* mKultur;
		CEGUI::Window* mProfession;
        CEGUI::Window* mEffekte;
		
		void updateTalents();

		/**
		* Aktualisiert nur die Werte in den Talentlisten
		* und auf dem Charakterblatt
		*/
		void updateValues();

		bool handleClose();
	};
}


#endif

