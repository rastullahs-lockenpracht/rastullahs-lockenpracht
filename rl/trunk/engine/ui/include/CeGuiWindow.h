#ifndef __CeGuiWindow_H__
#define __CeGuiWindow_H__

#include "UiPrerequisites.h"

#include "CEGUI.h"

namespace rl {

	class _RlUiExport CeGuiWindow
	{
	public:
		CEGUI::Window* getWindow(const char* name);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::StaticText* getStaticText(const char* name);
		CEGUI::StaticImage* getStaticImage(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);

		static void addToRoot(CEGUI::Window* window);

		bool isInputWindow();

        virtual bool isVisible();
        virtual void setVisible(bool visible);

		const CeGuiString& getName() const;

		virtual ~CeGuiWindow();

	protected:
		CeGuiWindow(const char* xmlfile, bool input = false);
		
		void show();
		void hide();

		virtual bool beforeShow();
		virtual bool beforeHide();
	
		void centerWindow();
		
		CEGUI::Window* mWindow;
		bool mIsVisible;

	private:
		bool mWithInput;
		CeGuiString mNamePrefix;
		CeGuiString mName;

		static int sNumWindows;
		
	};

}

#endif
