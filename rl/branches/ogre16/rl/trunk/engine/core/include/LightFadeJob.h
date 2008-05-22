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

#ifndef __RL_LIGHT_FADE_JOB_H__
#define __RL_LIGHT_FADE_JOB_H__

#include "CorePrerequisites.h"
#include "LightObject.h"

#include "Job.h"

namespace rl {

    class _RlCoreExport LightFadeJob : public Job
    {
    public:
        LightFadeJob(LightObject* lightObject, const Ogre::ColourValue& targetDiffuse, 
            const Ogre::ColourValue& targetSpecular, Ogre::Real duration = 1.0f);

        virtual bool execute(Ogre::Real time);
        virtual void discard();

    protected:
        /**
         *  Interpolates between two color values. The interpolated colour is returned in
         *  currentColour and depends on mTime and mDuration.
         **/
        bool interpolateColour(const Ogre::ColourValue& startColour, 
            const Ogre::ColourValue& endColour, Ogre::ColourValue& currentColour);
        int mSteps;
        LightObject* mLightObject;
        /// The time passed since job start.
        Ogre::Real mTime;
        /// The total duration of the fade job.
        Ogre::Real mDuration;
        Ogre::ColourValue mTargetDiffuse;
        Ogre::ColourValue mStartDiffuse;
        Ogre::ColourValue mTargetSpecular;
        Ogre::ColourValue mStartSpecular;
    };
}

#endif
