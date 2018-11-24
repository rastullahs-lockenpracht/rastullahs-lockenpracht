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
#include "stdinc.h" //precompiled header

#include "WindowFadeJob.h"

#include "AbstractWindow.h"
#include "CeGuiHelper.h"
#include "ItemDragContainer.h"
#include "WindowManager.h"
#include <CEGUI/WindowManager.h>

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    WindowFadeJob::WindowFadeJob(AbstractWindow* window, Mode mode, Real targetAlpha, Real changeRate)
        : Job(true, true)
        , mAbstractWindow(window)
        , mCEGUIWindow(window->getWindow())
        ,
        //        mItemDragContainer(NULL),
        mMode(mode)
        , mChangeRate(changeRate)
        , mCurrentAlpha(mode == FADE_IN ? 0.0f : 1.0f)
        , mTargetAlpha(mode == FADE_IN ? targetAlpha : 0.0f)
    {
        // Ensure visibility. This is independant of whether we're fading in or out.
        mCEGUIWindow->setVisible(true);
        mCEGUIWindow->setAlpha(mCurrentAlpha);
    }

    WindowFadeJob::WindowFadeJob(CEGUI::Window* window, Mode mode, Real targetAlpha, Real changeRate)
        : Job(true, true)
        , mAbstractWindow(NULL)
        , mCEGUIWindow(window)
        ,
        //        mItemDragContainer(NULL),
        mMode(mode)
        , mChangeRate(changeRate)
        , mCurrentAlpha(mode == FADE_IN ? 0.0f : 1.0f)
        , mTargetAlpha(mode == FADE_IN ? targetAlpha : 0.0f)
    {
        // Ensure visibility. This is independant of whether we're fading in or out.
        mCEGUIWindow->setVisible(true);
        mCEGUIWindow->setAlpha(mCurrentAlpha);
    }
    /*
        WindowFadeJob::WindowFadeJob(ItemDragContainer* window, Mode mode, Real targetAlpha, Real changeRate)
            : Job(true, true),
            mAbstractWindow(NULL),
            mCEGUIWindow(window),
            mItemDragContainer(window),
            mMode(mode),
            mChangeRate(changeRate),
            mCurrentAlpha(mode == FADE_IN ? 0.0f : 1.0f),
            mTargetAlpha(mode == FADE_IN ? targetAlpha : 0.0f)
        {
            // Ensure visibility. This is independant of whether we're fading in or out.
            mCEGUIWindow->setVisible(true);
            mCEGUIWindow->setAlpha(mCurrentAlpha);
        }
    */
    bool WindowFadeJob::execute(Real time)
    {
        mCurrentAlpha += time * mChangeRate * Math::Sign(mTargetAlpha - mCurrentAlpha);

        // Are we done now?
        if ((mMode == FADE_IN && mCurrentAlpha >= mTargetAlpha) || (mMode != FADE_IN && mCurrentAlpha <= mTargetAlpha))
        {
            discard();
            return true;
        }
        mCEGUIWindow->setAlpha(mCurrentAlpha);

        return false;
    }

    void WindowFadeJob::discard()
    {
        mCEGUIWindow->setAlpha(mTargetAlpha);
        mCEGUIWindow->setVisible(mMode == FADE_IN ? true : false);
        if (mMode == FADE_OUT_AND_DESTROY)
        {
            if (mAbstractWindow)
                WindowManager::getSingleton().destroyWindow(mAbstractWindow);
            //            else if(mItemDragContainer)
            //                delete mItemDragContainer;
            else
                CEGUI::WindowManager::getSingleton().destroyWindow(mCEGUIWindow);
        }
    }
}
