#ifndef __CeGuiWindow_H__
#define __CeGuiWindow_H__

#include "UiPrerequisites.h"

#include "CEGUI.h"

namespace rl {

	class _RlUiExport CeGuiWindow
	{
	public:
		static CEGUI::Window* getWindow(const char* name);
		static CEGUI::Editbox* getEditBox(const char* name);
		static CEGUI::Listbox* getListbox(const char* name);
		static CEGUI::StaticText* getStaticText(const char* name);
		static CEGUI::StaticImage* getStaticImage(const char* name);

		static void addToRoot(CEGUI::Window* window);

		void show();
		void hide();
		bool isInputWindow();

	protected:
		CeGuiWindow(const char* xmlfile, bool input = false);
		virtual ~CeGuiWindow();

		virtual bool beforeShow();
		virtual bool beforeHide();
		
		CEGUI::Window* mWindow;
	private:
		bool mWithInput;
	};

}

#endif