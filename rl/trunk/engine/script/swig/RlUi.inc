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

namespace rl {

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
	void showCharacterActionChoice();
	void showActionChoice(rl::GameObject* obj);
	void showMessageWindow(const CeGuiString& message);
	Person* getActiveCharacter();
	void setActiveCharacter(Person* person);
	void consoleToggle();
	
	void requestExit();
};

class CommandMapper
{
public:
	static CommandMapper& getSingleton();
	//void loadCommandMap(const char* mapfile);

};

}