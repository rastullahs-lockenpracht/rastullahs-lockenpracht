#include <boost/bind.hpp>

#include "CharacterSheetWindow.h"
#include "Creature.h"
#include "InputManager.h"

namespace rl {

using namespace CEGUI;

Point CharacterSheetWindow::TAB_POSITION = Point(10, 30);
Size CharacterSheetWindow::TAB_SIZE = Size(650, 500);
Size CharacterSheetWindow::BUTTON_SIZE = Size(64, 64);

CharacterSheetWindow::CharacterSheetWindow()
{
	mCharacterSheetWindow = WindowManager::getSingleton().loadWindowLayout((utf8*)"charactersheet.xml");
	
	WindowManager::getSingleton().getWindow((utf8*)"CharacterSheet/CharacterSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/CharacterSheet"));
	
	WindowManager::getSingleton().getWindow((utf8*)"CharacterSheet/TalentSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/TalentSheet"));
	
	WindowManager::getSingleton().getWindow((utf8*)"CharacterSheet/MagicSheetButton")->subscribeEvent(
		StaticImage::MouseClickEvent, 
		boost::bind(&CharacterSheetWindow::showTab, this, (utf8*)"/MagicSheet"));	
	
	showTab((utf8*)"/CharacterSheet");

	Window* rootWindow = WindowManager::getSingleton().getWindow((utf8*)"root_wnd");
	rootWindow->addChildWindow(mCharacterSheetWindow);	
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
}

void CharacterSheetWindow::showTab(utf8* name)
{
	Window* tab = WindowManager::getSingleton().getWindow(
		mCharacterSheetWindow->getName() + name);
	tab->moveToFront();
}

void CharacterSheetWindow::show()
{
	InputManager::getSingleton().registerCeguiWindow();
	mCharacterSheetWindow->show();
}

void CharacterSheetWindow::hide()
{
	InputManager::getSingleton().unregisterCeguiWindow();
	mCharacterSheetWindow->hide();
}


}