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

#ifndef __Console_H__
#define __Console_H__

#include "UiPrerequisites.h"

#include <vector>
#include <string>

#include "CeGuiWindow.h"

#include <OgreSingleton.h>

namespace rl {

    class Interpreter;

	/** This is a Console System
	 */
	class _RlUiExport Console
	  : public Ogre::Singleton<Console>, public CeGuiWindow
	{
		public:
			Console();
			
			static Console& getSingleton();
			static Console* getSingletonPtr();

			void setInterpreter(Interpreter* interpreter);
			Interpreter* getInterpreter();

			void write(Ogre::String output);
				
		protected:
			bool beforeShow();

		private:
			CEGUI::Editbox* mCommandLine;
			CEGUI::Listbox* mDisplay;
			
			Interpreter* mInterpreter;
			bool mPrompt;

			bool handleKeyDown(const CEGUI::EventArgs& e);
			bool handleClose();
			void appendTextRow(CeGuiString& text, const CEGUI::colour color);

			std::vector<Ogre::String> mHistory;
			unsigned int mHistoryMarker;
			void cycleHistory(int skip);
	};
}

#endif
