/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

class CharacterController
{
private:
	CharacterController(rl::Actor* camera, rl::Actor* character);
	
public:
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
	enum ControllerType 
	{
		CTRL_FREEFLIGHT = 1,
		CTRL_MOVEMENT,
		CTRL_DIALOG,
		CTRL_COMBAT
	};

    static UiSubsystem& getSingleton(void);
    
	rl::Person* getActiveCharacter();
	void setActiveCharacter(rl::Person* person);

	rl::CharacterController* getCharacterController() const;
	rl::UiSubsystem::ControllerType getCharacterControllerType() const;
	void setCharacterController(rl::UiSubsystem::ControllerType type);
	
	void useDefaultAction(rl::GameObject* obj, rl::Creature* actor);
	void usePickedObjectDefaultActions();
	void toggleObjectPicking();
	
	void setCombatMode(bool inCombat);
	bool isInCombatMode();
	void startCombat(rl::Combat* combat);
};

class VideoWindow
{
public:
    static rl::VideoWindow& getSingleton(void);
    void show(CEGUI::Texture *texture, rl::CeGuiString name);
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
	void showPickedObjectActions();
    void showPlaylist();
    void showSoundConfig();
	void showTargetWindow();

	void toggleConsole();
	void toggleDebugWindow();
	void toggleGameLogWindow();
	void toggleCharacterStateWindow();
	void toggleInGameGlobalMenu();

	void checkForErrors();
	rl::GameLoggerWindow* getGameLogger();
};

class WindowManager
{
public:
	static rl::WindowManager& getSingleton();
	void closeTopWindow();
};

}
