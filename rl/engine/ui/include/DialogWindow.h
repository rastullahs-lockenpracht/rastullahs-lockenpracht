#ifndef __DialogWindow_H__
#define __DialogWindow_H__

#include "UiPrerequisites.h"

#include <string>
#include <vector>
#include <map>

#include <CEGUI.h>

namespace rl {

	class _RlUiExport DialogWindow
	{
	public:
		DialogWindow();
		~DialogWindow();

		void addLine(std::string text);
		void removeLine(int num);
		unsigned int count();
		void setVariableValue(std::string name, std::string value);
		void setCallback(std::string function);
		void setName(std::string name);
		void setQuestion(std::string question);
		void show();
		void hide();
		int getSelectedOption();

	private:
		std::map<std::string, std::string> mVariableValues;
		std::vector<std::string> mTextLines;
		CEGUI::Listbox* mDialogOptions;
				
		void updateValues();
	};

}
#endif