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

#ifndef __DialogWindow_H__
#define __DialogWindow_H__

#include "UiPrerequisites.h"

#include <vector>
#include <map>

#include "CeGuiWindow.h"

#include "NaturalLanguageProcessor.h"

namespace rl {

	class DialogCharacter;
	class DialogCharacterController;
	class GameLoggerWindow;

	class _RlUiExport DialogWindow : public CeGuiWindow
	{
	public:
		DialogWindow(DialogCharacter* bot, GameLoggerWindow* gamelogger, DialogCharacterController* controller);
		~DialogWindow();

		void initialize();

		void getResponse(const CeGuiString& msg);
		unsigned int count();
		void setCallback(std::string function);
		void setName(std::string name);
		void setImage(std::string imageset, std::string image);
		int getSelectedOption();

		void showNextText();

	private:
		std::map<std::string, std::string> mVariableValues;
		std::vector<std::string> mTextLines;
		NaturalLanguageProcessor::Responses mResponses;
		DialogCharacter* mBot;
		DialogResponse* mCurrentResponse;
		CEGUI::Listbox* mDialogOptions;
		CEGUI::StaticImage* mImage;
		CEGUI::Listbox* mQuestion;
		CEGUI::StaticText* mName;
		GameLoggerWindow* mGameLogger;
		DialogCharacterController* mController;
				
		bool handleSelectOption();	
		bool handleClose();

		static const CEGUI::colour COLOR_PLAYER_CHARACTER;
		static const CEGUI::colour COLOR_NON_PLAYER_CHARACTER;
		static const CeGuiString DIALOG_START;
		static const CeGuiString DIALOG_END;
	};

}
#endif
