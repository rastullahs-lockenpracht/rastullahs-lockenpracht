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

#include "WindowFadeJob.h"

#include "CeGuiWindow.h"
#include "CeGuiHelper.h"
#include "WindowManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    WindowFadeJob::WindowFadeJob(CeGuiWindow* window, Mode mode, Real targetAlpha, Real changeRate)
        : Job(true, true),
        mWindow(window),
        mMode(mode),
        mChangeRate(changeRate),
        mCurrentAlpha(mode == FADE_IN ? 0.0f : 1.0f),
        mTargetAlpha(mode == FADE_IN ? targetAlpha : 0.0f)
    {
        // Ensure visibility. This is independant of whether we're fading in or out.
        mWindow->getWindow()->setVisible(true);
        mWindow->getWindow()->setAlpha(mCurrentAlpha);
    }

    bool WindowFadeJob::execute(Real time)
    {
        mCurrentAlpha += time * mChangeRate * Math::Sign(mTargetAlpha - mCurrentAlpha);

        // Are we done now?
        if ((mMode == FADE_IN && mCurrentAlpha >= mTargetAlpha) ||
            (mMode != FADE_IN && mCurrentAlpha <= mTargetAlpha))
        {
            discard();
            return true;
        }
        mWindow->getWindow()->setAlpha(mCurrentAlpha);

        return false;
    }

    void WindowFadeJob::discard()
    {
        mWindow->getWindow()->setAlpha(mTargetAlpha);
        mWindow->getWindow()->setVisible(mMode == FADE_IN ? true : false);
        if (mMode == FADE_OUT_AND_DESTROY)
        {
            WindowManager::getSingleton().destroyWindow(mWindow);
        }
    }
}
