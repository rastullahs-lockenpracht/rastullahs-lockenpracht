#include <boost/bind.hpp>

#include "CharacterSheetWindow.h"
#include "DsaManager.h"
#include "Creature.h"
#include "InputManager.h"
#include "Talent.h"

namespace rl {

using namespace CEGUI;

CharacterSheetWindow::CharacterSheetWindow()
	: CeGuiWindow("charactersheet.xml", true)
{
	mTalentTable = getMultiColumnList("CharacterSheet/TalentSheet/Table");
	mTalentTable->addColumn((utf8*)"Talent", 0, 0.5);
	mTalentTable->addColumn((utf8*)"Probe", 1, 0.3);
	mTalentTable->addColumn((utf8*)"eBE", 2, 0.1);
	mTalentTable->addColumn((utf8*)"TW", 3, 0.1);
	mTalentTable->setUserSortControlEnabled(false);
	addToRoot(mWindow);	
}

CharacterSheetWindow::~CharacterSheetWindow()
{
}

void CharacterSheetWindow::setCharacter(Creature* creature)
{
	mCreature = creature;
	update();
}

void CharacterSheetWindow::update()
{
	    //TODO: Daten updaten
	updateTalents();
	/*if (mCreature->isMagic())
		updateMagic();*/
	updateValues();
}

void CharacterSheetWindow::updateValues()
{
	for (unsigned int row = 0; row < mTalentTable->getRowCount(); row++)
	{
		int tw = mCreature->getTalent(mTalentTable->getItemAtGridReference(MCLGridRef(row, 0))->getID());

		mTalentTable->getItemAtGridReference(MCLGridRef(row, 3))->setText(StringConverter::toString(tw));
	}
}

void CharacterSheetWindow::updateTalents()
{
	const Creature::TalentMap talents = mCreature->getAllTalents();

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
		string probe = "";
		probe += DsaManager::getSingleton().getEigenschaft(eigensch.first)->getNameAbbreviation();
		probe += "/";
		probe += DsaManager::getSingleton().getEigenschaft(eigensch.second)->getNameAbbreviation();
		probe += "/";
		probe += DsaManager::getSingleton().getEigenschaft(eigensch.third)->getNameAbbreviation();

		mTalentTable->setItem(new ListboxTextItem(probe), 1, talentNum);
		CEGUI::String eBe;
		if (talent->getEbe() == EBE_KEINE_BE)
			eBe = (utf8*)"-";
		else if (talent->getEbe() == EBE_BEx2)
			eBe = (utf8*)"BEx2";
		else if (talent->getEbe() == 0)
			eBe = (utf8*)"BE";
		else if (talent->getEbe() > 0)
			eBe = (utf8*)"BE+" + CEGUI::String(StringConverter::toString(talent->getEbe()));
		else
			eBe = (utf8*)"BE" + CEGUI::String(StringConverter::toString(talent->getEbe()));

		mTalentTable->setItem(new ListboxTextItem(eBe), 2, talentNum);
		mTalentTable->setItem(new ListboxTextItem((utf8*)""), 3, talentNum);

		talentNum++;
	}

}


}
