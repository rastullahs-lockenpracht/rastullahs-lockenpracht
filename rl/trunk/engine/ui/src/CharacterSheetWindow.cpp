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
	getWindow("CharacterSheet/CharacterSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/CharacterSheet"));
	
	getWindow("CharacterSheet/TalentSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/TalentSheet"));
	
	getWindow("CharacterSheet/MagicSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/MagicSheet"));	
	
	showTab((utf8*)"/CharacterSheet");

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
	for (int row = 0; row <= mTalentTable->getRowCount(); row++)
	{

	}
}

void CharacterSheetWindow::updateTalents()
{
	const Creature::TalentMap talents = mCreature->getAllTalents();

	if (talents.size() > mTalentTable->getRowCount());
		//TODO: Zeilen anpassen

	int talentNum = 0;

	for (Creature::TalentMap::const_iterator iter = talents.begin(); iter != talents.end(); iter++)
	{
		//Talente in die Talenttabelle
		Talent* talent = DsaManager::getSingleton().getTalent((*iter).first);
		mTalentTable->addRow();
		mTalentTable->setItem(new ListboxTextItem(talent->getName()), 0, talentNum);
		mTalentTable->setItem(new ListboxTextItem((utf8*)"MU/MU/MU"), 1, talentNum);
		mTalentTable->setItem(new ListboxTextItem((utf8*)""), 2, talentNum);

		talentNum++;
	}

}

void CharacterSheetWindow::showTab(utf8* name)
{
	Window* tab = WindowManager::getSingleton().getWindow(
		mWindow->getName() + name);
	tab->moveToFront();
}

}