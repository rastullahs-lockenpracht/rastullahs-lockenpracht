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
    void resetCamera();
};

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
	void showCharacterActionChoice();
	void showActionChoice(rl::GameObject* obj);
	void showMessageWindow(const rl::CeGuiString& message);
	rl::Person* getActiveCharacter();
	void setActiveCharacter(Person* person);
	void showMainMenu(GameObject* actionHolder);
	void toggleConsole();
	void toggleDebugWindow();
	rl::GameController* getGameController();
	rl::GameLoggerWindow* getGameLogger();
	
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
	void logDialogEvent(const rl::CeGuiString& text);
	void logFightEvent(const rl::CeGuiString& text);
	void logCharacterEvent(const rl::CeGuiString& text);
	void logMiscEvent(const rl::CeGuiString& text);
};

}
