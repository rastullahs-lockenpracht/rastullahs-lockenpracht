#ifndef __DialogWindow_H__
#define __DialogWindow_H__

#include "UiPrerequisites.h"

#include <string>
#include <vector>
#include <map>

#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport DialogWindow : public CeGuiWindow
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
		void setImage(std::string imageset, std::string image);
		int getSelectedOption();
		void handleSelectOption();

		static void runTest();

	private:
		std::map<std::string, std::string> mVariableValues;
		std::vector<std::string> mTextLines;
		CEGUI::Listbox* mDialogOptions;
		CEGUI::StaticImage* mImage;
		CEGUI::StaticText* mQuestion;
		CEGUI::StaticText* mName;
				
		void updateValues();
	};

}
#endif
