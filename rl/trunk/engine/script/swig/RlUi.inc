/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

namespace rl {

%feature("director") CharacterController;
class CharacterController
{
private:
	CharacterController(rl::Actor* camera, rl::Actor* character);
public:
	enum ControllerType 
	{
		CTRL_NONE = 1,
		CTRL_FREEFLIGHT,
		CTRL_MOVEMENT,
		CTRL_DIALOG,
		CTRL_CUTSCENE,
		CTRL_COMBAT,
	};
	
	rl::CharacterController::ControllerType getType();

     /**
       *  @throw NullPointerException if camera or character is NULL.
       *  @throw InvalidArgumentException if character is not placed in the scene.
       */
    void toggleViewMode();
    void resetCamera();
        
};

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
	rl::Person* getActiveCharacter();
	void setActiveCharacter(rl::Person* chara);

	rl::CharacterController* getCharacterController() const;
	rl::CharacterController::ControllerType getCharacterControllerType() const;
	void setCharacterController(rl::CharacterController::ControllerType type);
};

class GameLoggerWindow
{
public:
	void logDialogEvent(const rl::CeGuiString& speaker, const rl::CeGuiString& text);
	void logFightEvent(const rl::CeGuiString& text);
	void logCharacterEvent(const rl::CeGuiString& text);
	void logMiscEvent(const rl::CeGuiString& text);
};

class WindowFactory
{
public:
	enum PopupType
	{
		ICON_ERROR = 1,
		ICON_QUEST
	};

	static rl::WindowFactory& getSingleton();

	void update();

	void showAboutWindow();
	void showActionChoice(rl::GameObject* obj);
	void showCharacterActionChoice();
	void showCharacterSheet();
	void showCharacterSheet(rl::Person* chara);
	void showContainerContent(rl::Container* container);
	void showDescriptionWindow(rl::GameObject* obj);
	void showDialog(rl::DialogCharacter* bot);
	void showExitConfirmation();
	void showJournalWindow();
	void showLogfiles();
	void showMainMenu();
	void showMessageWindow(const rl::CeGuiString& message);
    void showPlaylist();
	void showPopupMessage(int popupTypes);
    void showSoundConfig();

	void toggleConsole();
	void toggleDebugWindow();
	void toggleGameLogWindow();
	void toggleCharacterStateWindow();
	void toggleInGameGlobalMenu();
	void toggleInventoryWindow();	

	void checkForErrors();
	rl::GameLoggerWindow* getGameLogger();
	
    void showNextDebugWindowPage();
};

class WindowManager
{
public:
	static rl::WindowManager& getSingleton();
	void closeTopWindow();
};

}
