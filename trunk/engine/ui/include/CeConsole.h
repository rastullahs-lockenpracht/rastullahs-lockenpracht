#ifndef __CeConsole_H__
#define __CeConsole_H__

#include "UiPrerequisites.h"

#include <vector>
#include <string>
#include <OgreSingleton.h>
#include <CEGUIBase.h>


namespace CEGUI
{
	class Window;
	class EventArgs;
	class Editbox;
	class Listbox;
}


namespace rl {

    class Interpreter;

	/** This is a Console System
	 */
	class _RlUiExport CeConsole : public Singleton<CeConsole>
	{
		public:
			~CeConsole();
			CeConsole(void);
			static CeConsole& getSingleton(void);
			static CeConsole* getSingletonPtr(void);

			void setInterpreter(Interpreter* interpreter);

			Interpreter* getInterpreter();

			void write(String output);

			bool isOpen(void) const;
			bool isClosed(void) const;
				
			void open(void);
			void close(void);

		private:
			CEGUI::Window* mConsoleWindow;
			CEGUI::Editbox* mCommandLine;
			CEGUI::Listbox* mDisplay;
			
			Interpreter* mInterpreter;
			bool mPrompt;

			enum State { CS_OPEN, CS_CLOSED, CS_OPENING, CS_CLOSING };
			State mState;

			void handleKeyDown(const CEGUI::EventArgs& e);
			void handleEnter(const CEGUI::EventArgs& e);
			void appendTextRow(CEGUI::String& text, const CEGUI::colour color);

			std::vector<std::string> mHistory;
			int mHistoryMarker;
			void cycleHistory(int skip);
	};
}

#endif

