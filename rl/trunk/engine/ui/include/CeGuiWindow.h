#ifndef __CeGuiWindow_H__
#define __CeGuiWindow_H__

#include "UiPrerequisites.h"

#include "CEGUI.h"

namespace rl {

	class _RlUiExport CeGuiWindow
	{
	public:
		static CEGUI::Window* getWindow(const char* name);
		static CEGUI::Editbox* getEditbox(const char* name);
		static CEGUI::Listbox* getListbox(const char* name);
		static CEGUI::StaticText* getStaticText(const char* name);
		static CEGUI::StaticImage* getStaticImage(const char* name);
		static CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		static CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);

		static void addToRoot(CEGUI::Window* window);

		bool isInputWindow();

        virtual bool isVisible();
        virtual void setVisible(bool visible);

	protected:
		CeGuiWindow(const char* xmlfile, bool input = false);
		virtual ~CeGuiWindow();

		void show();
		void hide();

		virtual bool beforeShow();
		virtual bool beforeHide();
		
		CEGUI::Window* mWindow;
		bool mIsVisible;

	private:
		bool mWithInput;
		static int sNumWindows;
		
	};

}

#endif
