#ifndef __DialogWindow_H__
#define __DialogWindow_H__

#include "NaturalLanguageProcessor.h"
#include "UiPrerequisites.h"


#include <string>
#include <vector>
#include <map>

#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport DialogWindow : public CeGuiWindow
	{
	public:
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
		void handleSelectOption();		

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
	};

}
#endif
