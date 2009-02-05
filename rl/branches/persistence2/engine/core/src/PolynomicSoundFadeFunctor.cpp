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

#include "PolynomicSoundFadeFunctor.h"


namespace rl
{
    PolynomicSoundFadeFunctor::PolynomicSoundFadeFunctor(Ogre::Real duration, InterpolationPoints *points) :
        SoundFadeFunctor(duration),
        mPoints(points),
		mParameters(NULL)
    {
		mParameters = new Parameters(points->size());
		int n = points->size() - 1;
		for(int i = 0; i < n; i++)
		{
			(*mParameters)[i] = (*mPoints)[i].second;
		}
		for(int k = 1; k <= n; k++)
		{
			for(int i = n; i >= k; i--)
			{
				(*mParameters)[i] = 1.0f * ((*mParameters)[i] - (*mParameters)[i - 1]) / ((*mPoints)[i].first - (*mPoints)[i - k].first);
			}
		}
    }

	PolynomicSoundFadeFunctor::~PolynomicSoundFadeFunctor()
	{
		if (mPoints != NULL)
		{
			delete mPoints;
		}
		if (mParameters != NULL)
		{
			delete mParameters;
		}
	}

    Ogre::Real PolynomicSoundFadeFunctor::operator()(Ogre::Real time)
    {
        int n = mPoints->size() - 1;
		Ogre::Real p = (*mPoints)[n].first;
		for(int k = n - 1; k >= 0; k--)
		{
			p *= (time - (*mPoints)[k].first) + (*mParameters)[k];
		}
		return p;
    }

}
