/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __MessageWindow_H__
#define __MessageWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

namespace rl {
	
	class _RlUiExport MessageWindow :
		public AbstractWindow
	{
	public:
        enum Buttons 
        {
            OK,
            YES_NO
        };

        MessageWindow(
            const CeGuiString& title, 
            const CeGuiString& text, 
            const Buttons buttons);

        void setTitle(const CeGuiString& title);
        void setText(const CeGuiString& text);
        void setButtons(const CeGuiStringVector& buttonTexts);
        virtual bool onButtonClicked(int button, const CeGuiString& text);
	
	private:
        CEGUI::Window* mTextField;
        CEGUI::Window* mButtonPanel;
        int mNumButtons;

        const CeGuiStringVector getButtonTexts(const Buttons buttons) const;
	};
}

#endif
