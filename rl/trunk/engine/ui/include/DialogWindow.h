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

#include "NaturalLanguageProcessor.h"
#include "UiPrerequisites.h"


#include <string>
#include <vector>
#include <map>

#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport DialogWindow : public CeGuiWindow, public Ogre::Singleton<DialogWindow>
	{
	public:
        static DialogWindow& getSingleton(void);
        static DialogWindow* getSingletonPtr(void);

		DialogWindow(string dialogFile);
		~DialogWindow();

		void getResponse(std::string msg);
		void addLine(std::string text);
		void removeLine(int num);
		unsigned int count();
		void setVariableValue(std::string name, std::string value);
		void setCallback(std::string function);
		void setName(std::string name);
		void setQuestion(std::string question);
		void setImage(std::string imageset, std::string image);
		int getSelectedOption();

	private:
		std::map<std::string, std::string> mVariableValues;
		std::vector<std::string> mTextLines;
		std::map<int,std::string> mResponses;
		NaturalLanguageProcessor* mNlp;
		CEGUI::Listbox* mDialogOptions;
		CEGUI::StaticImage* mImage;
		CEGUI::MultiLineEditbox* mQuestion;
		CEGUI::StaticText* mName;
				
		void updateValues();
		bool handleSelectOption();	
		bool handleClose();
	};

}
#endif
