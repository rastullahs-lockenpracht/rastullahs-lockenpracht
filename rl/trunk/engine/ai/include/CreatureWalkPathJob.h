/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#ifndef __CreatureWalkPathJob_H__
#define __CreatureWalkPathJob_H__

#include "AiPrerequisites.h"

#include "Landmark.h"
#include "LandmarkPath.h"

#include "Job.h"
#include "MovingCreature.h"

namespace rl
{
    class _RlAiExport CreatureWalkPathJob : public Job
    {
    public:
        CreatureWalkPathJob(const Ogre::String& name, MovingCreature* movingCreature, Landmark* startLandmark);
        virtual ~CreatureWalkPathJob();

        void addLandmark(const Ogre::String& name, const Ogre::Vector3& position);
        void addLandmark(Landmark* lm);

        virtual bool execute(Ogre::Real time);
    private:
        LandmarkPath mLandmarkPath;
        Ogre::String mName;
        MovingCreature* mMovingCreature;

        Landmark* mCurrentLandmark;
        Landmark* mNextLandmark;
    };
}

#endif