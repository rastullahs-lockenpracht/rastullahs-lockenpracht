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

#ifndef __RL_POLYNOMIC_SOUND_FADE_FUNCTOR_H__
#define __RL_POLYNOMIC_SOUND_FADE_FUNCTOR_H__

#include "CorePrerequisites.h"
#include "SoundFadeFunctor.h"
#include <vector>

namespace rl
{
    //    class SoundObject;
    typedef std::vector<std::pair<Ogre::Real, Ogre::Real>> InterpolationPoints;
    typedef std::vector<Ogre::Real> Parameters;

    class _RlCoreExport PolynomicSoundFadeFunctor : public SoundFadeFunctor
    {
    public:
        PolynomicSoundFadeFunctor(Ogre::Real duration, InterpolationPoints* points);
        ~PolynomicSoundFadeFunctor();

        virtual Ogre::Real operator()(Ogre::Real time);

    private:
        // Interpolation points
        InterpolationPoints* mPoints;
        Parameters* mParameters;
    };
}

#endif
