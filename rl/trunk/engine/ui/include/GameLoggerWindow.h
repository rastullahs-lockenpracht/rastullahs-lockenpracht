#ifndef __GameLoggerWindow_H__
#define __GameLoggerWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport GameLoggerWindow : public CeGuiWindow
	{
	public:
		GameLoggerWindow();

		void logDialogEvent(const std::string text);
		void logFightEvent(const std::string text);
		void logLevelupEvent(const std::string text);
		void logMiscEvent(const std::string text);

		void logEvent(const std::string text, const CEGUI::colour color);
	};


}

#endif