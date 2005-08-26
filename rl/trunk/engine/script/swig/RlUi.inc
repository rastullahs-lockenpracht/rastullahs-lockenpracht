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

class GameController
{
public:
    typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON} ViewMode;
    rl::Actor* getControlledActor();
    void setControlledActor(rl::Actor* actor);
    rl::Actor* getCamera();
    void setViewMode(ViewMode mode);
    ViewMode getViewMode();
    void toggleViewMode();
    void resetCamera();
};

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
	void showCharacterActionChoice();
	void showPickedObjectActions();
	void showActionChoice(rl::GameObject* obj);
	void showMessageWindow(const rl::CeGuiString& message);
	void showCharacterSheet();
	void showCharacterSheet(Person* chara);
    void showPlaylist();
	void showTargetWindow();
	void showContainerContent(rl::Container* container);

	rl::Person* getActiveCharacter();
	void setActiveCharacter(rl::Person* person);
	void showMainMenu(GameObject* actionHolder);
	void toggleConsole();
	void toggleDebugWindow();
	void toggleGameLogWindow();
	void toggleDialogWindow();
	void toggleObjectPicking();
	void toggleCharacterStateWindow();
	void toggleInGameGlobalMenu();
	void usePickedObjectDefaultActions();
	
	rl::GameController* getGameController();
	rl::GameLoggerWindow* getGameLogger();
	
	void update();
	
	void requestExit();
};

class CommandMapper
{
public:
	static CommandMapper& getSingleton();
	//void loadCommandMap(const char* mapfile);

};

class GameLoggerWindow
{
public:
	void logDialogEvent(const rl::CeGuiString& speaker, const rl::CeGuiString& text);
	void logFightEvent(const rl::CeGuiString& text);
	void logCharacterEvent(const rl::CeGuiString& text);
	void logMiscEvent(const rl::CeGuiString& text);
};

}
