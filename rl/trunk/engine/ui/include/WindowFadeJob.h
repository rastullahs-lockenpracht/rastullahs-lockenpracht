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

#ifndef __RL_WINDOW_UPDATE_JOB_H__
#define __RL_WINDOW_UPDATE_JOB_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include "Job.h"

namespace rl {

    class CeGuiWindow;

    class WindowFadeJob : public Job
    {
    public:
        typedef enum {FADE_IN, FADE_OUT, FADE_OUT_AND_DESTROY} Mode;

        WindowFadeJob(CeGuiWindow* window, Mode mode,
            Ogre::Real targetAlpha, Ogre::Real changeRate = 4.0f);

        virtual bool execute(Ogre::Real time);
        virtual void discard();

    protected:
        CeGuiWindow* mWindow;
        Mode mMode;
        Ogre::Real mChangeRate;
        Ogre::Real mCurrentAlpha;
        Ogre::Real mTargetAlpha;
    };
}

#endif
