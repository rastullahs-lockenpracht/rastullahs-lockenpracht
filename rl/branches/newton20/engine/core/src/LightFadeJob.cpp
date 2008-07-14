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
#include "stdinc.h" //precompiled header

#include "LightFadeJob.h"


namespace rl
{
    LightFadeJob::LightFadeJob(LightObject* lightObject, const Ogre::ColourValue& targetDiffuse,
        const Ogre::ColourValue& targetSpecular, Ogre::Real duration)
        : Job(true, true),
        mLightObject(lightObject),
        mDuration(duration)
    {
        if (mLightObject != NULL)
        {
            mStartDiffuse = mLightObject->getDiffuseColour();
            mStartSpecular = mLightObject->getSpecularColour();
        }
    }

    bool LightFadeJob::execute(Ogre::Real time)
    {
        // Precondition: mLightObject != NULL
        if (mLightObject == NULL)
        {
            discard();
            return true;
        }

        // add passed time to total time
        mTime += time;

        bool diffuseInterpolationFinished = false;
        bool specularInterpolationFinished = false;

        // interpolate colour values and set light source
        Ogre::ColourValue currentDiffuse;
        diffuseInterpolationFinished = interpolateColour(mStartDiffuse, mTargetDiffuse, currentDiffuse);
        mLightObject->setDiffuseColour(currentDiffuse);
        Ogre::ColourValue currentSpecular;
        specularInterpolationFinished = interpolateColour(mStartSpecular, mTargetSpecular, currentSpecular);
        mLightObject->setSpecularColour(currentSpecular);

        // Are we done now?
        if (diffuseInterpolationFinished && specularInterpolationFinished)
        {
            discard();
            return true;
        }
        //mWindow->getWindow()->setAlpha(mCurrentAlpha);

        return false;
    }

    void LightFadeJob::discard()
    {
        /// @todo check if light source is invisible?
    }

    bool LightFadeJob::interpolateColour(const Ogre::ColourValue& startColour,
        const Ogre::ColourValue& endColour, Ogre::ColourValue& currentColour)
    {
        // Postcondition: mTime <= mDuration
        if (mDuration < mTime)
            return true;

        // calculate current factor
        float factor;
        if (mTime == mDuration)
        {
            factor = 1.0f;
        }
        else
        {
            factor = (float)mTime / (float)mDuration;
        }

        // vresult = (1-factor)*vstart + factor*vend
        currentColour = (1.0f - factor) * startColour + factor * endColour;

        // if (finished) return true
        if (mTime == mDuration)
            return true;
        else
            return false;
    }
}
