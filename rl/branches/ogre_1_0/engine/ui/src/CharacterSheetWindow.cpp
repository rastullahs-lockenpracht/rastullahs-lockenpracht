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
#include "UiPrerequisites.h"

#include <boost/bind.hpp>

#include "DsaManager.h"
#include "Person.h"
#include "InputManager.h"
#include "Talent.h"

#include "CharacterSheetWindow.h"

namespace rl {

using namespace CEGUI;

CharacterSheetWindow::CharacterSheetWindow()
	: CeGuiWindow("charactersheet.xml", WND_MOUSE_INPUT)
{
	mTalentTable = getMultiColumnList("CharacterSheet/TalentSheet/Table");
	mTalentTable->addColumn((utf8*)"Talent", 0, 0.5);
	mTalentTable->addColumn((utf8*)"Probe", 1, 0.3);
	mTalentTable->addColumn((utf8*)"eBE", 2, 0.1);
	mTalentTable->addColumn((utf8*)"TW", 3, 0.1);
	mTalentTable->setUserSortControlEnabled(false);
	
	mLE = getStaticText("CharacterSheet/CharacterSheet/LE");
	mAE = getStaticText("CharacterSheet/CharacterSheet/AE");
	mAP = getStaticText("CharacterSheet/CharacterSheet/AP");
	mName = getStaticText("CharacterSheet/CharacterSheet/Name");
	mRasse = getStaticText("CharacterSheet/CharacterSheet/Rasse");
	mProfession = getStaticText("CharacterSheet/CharacterSheet/Profession");
	
	for (int i=0; i<EIGENSCHAFT_COUNT; i++)
		mEigenschaft[i] = 
			getStaticText(("CharacterSheet/CharacterSheet/Eigenschaften/"+
				DsaManager::getSingleton().getEigenschaft(i)->getNameAbbreviation()).c_str());
	addToRoot(mWindow);	
}

CharacterSheetWindow::~CharacterSheetWindow()
{
}

void CharacterSheetWindow::setCharacter(Person* person)
{
	mCharacter = person;
	update();
}

void CharacterSheetWindow::update()
{
	    //TODO: Daten updaten
	updateTalents();
	/*if (mCharacter->isMagic())
		updateMagic();*/
	updateValues();
}

void CharacterSheetWindow::updateValues()
{
	mName->setText("Name: "+mCharacter->getName());
	mRasse->setText(string("Rasse: "));
	mProfession->setText(string("Profession: "));

	mLE->setText("LeP: "+
		StringConverter::toString(mCharacter->getLe())+"/"+
		StringConverter::toString(mCharacter->getLeMax()));
	
	for (unsigned int eig = 0; eig < EIGENSCHAFT_COUNT; eig++)
	{
		CeGuiString name = DsaManager::getSingleton().getEigenschaft(eig)->getName();
		mEigenschaft[eig]->setText(
			name +": "+
			StringConverter::toString(mCharacter->getEigenschaft(eig)));
	}
	
	for (unsigned int row = 0; row < mTalentTable->getRowCount(); row++)
	{
		// TalentID ist bei der Namenszelle jeweils als user_id hinterlegt
		int tw = mCharacter->getTalent(
			mTalentTable->getItemAtGridReference(MCLGridRef(row, 0))->getID());

		mTalentTable->getItemAtGridReference(MCLGridRef(row, 3))->setText(
			StringConverter::toString(tw));
	}
}

void CharacterSheetWindow::updateTalents()
{
	const Creature::TalentMap talents = mCharacter->getAllTalents();

	while(mTalentTable->getRowCount() > talents.size())
		mTalentTable->removeRow(mTalentTable->getRowCount()-1);
	while(mTalentTable->getRowCount() < talents.size())
		mTalentTable->addRow();

	int talentNum = 0;

	for (Creature::TalentMap::const_iterator iter = talents.begin(); iter != talents.end(); iter++)
	{
		//Talente in die Talenttabelle
		Talent* talent = DsaManager::getSingleton().getTalent((*iter).first);
		mTalentTable->setItem(new ListboxTextItem(talent->getName(), talent->getId()), 0, talentNum);

		EigenschaftTripel eigensch = talent->getEigenschaften();
		CeGuiString probe;
		probe = DsaManager::getSingleton().getEigenschaft(eigensch.first)->getNameAbbreviation() + "/";
		probe += DsaManager::getSingleton().getEigenschaft(eigensch.second)->getNameAbbreviation() + "/";
		probe += DsaManager::getSingleton().getEigenschaft(eigensch.third)->getNameAbbreviation();

		mTalentTable->setItem(new ListboxTextItem(probe), 1, talentNum);
		CeGuiString eBe;
		if (talent->getEbe() == EBE_KEINE_BE)
			eBe = (utf8*)"-";
		else if (talent->getEbe() == EBE_BEx2)
			eBe = (utf8*)"BEx2";
		else if (talent->getEbe() == 0)
			eBe = (utf8*)"BE";
		else if (talent->getEbe() > 0)
			eBe = (utf8*)"BE+" + CeGuiString(StringConverter::toString(talent->getEbe()));
		else
			eBe = (utf8*)"BE" + CeGuiString(StringConverter::toString(talent->getEbe()));

		mTalentTable->setItem(new ListboxTextItem(eBe), 2, talentNum);
		mTalentTable->setItem(new ListboxTextItem((utf8*)""), 3, talentNum);

		talentNum++;
	}

}


}
