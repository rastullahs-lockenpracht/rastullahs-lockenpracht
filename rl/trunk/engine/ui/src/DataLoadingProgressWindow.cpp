/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "DataLoadingProgressWindow.h"

namespace rl {

DataLoadingProgressWindow::DataLoadingProgressWindow()
: CeGuiWindow("dataloadingprogresswindow.xml", CeGuiWindow::WND_SHOW, false)
{
	mProgressBar = getProgressBar("DataLoadingProgressWindow/ProgressBar");
	centerWindow();
}

DataLoadingProgressWindow::~DataLoadingProgressWindow()
{
}

bool DataLoadingProgressWindow::dataLoadedEventRaised(rl::DataLoadedEvent *anEvent)
{
	float percent = anEvent->getPercentLoaded();

	mProgressBar->setProgress(percent/100.0);

	if (percent == 100.0)
	{
		destroyWindow();
	}
	else if (percent != 100.0
		&& !isVisible())
	{
		setVisible(true);
	}

	return true;
}

}
