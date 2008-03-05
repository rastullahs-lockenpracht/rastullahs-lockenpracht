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
#ifndef __LogWindow_H__
#define __LogWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"

namespace rl
{

	class _RlUiExport LogWindow : public AbstractWindow
	{
	public:
		LogWindow();
		virtual ~LogWindow();
	
		void setVisible(bool visible, bool destroyAfterHide = false);
		bool update();

	private:
		CEGUI::MultiLineEditbox* mErrorLog;		
		CEGUI::MultiLineEditbox* mRastullahLog;
		CEGUI::MultiLineEditbox* mOgreLog;
	};

} // namespace rl

#endif
