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


#ifndef __MovingCreatureManager_H__
#define __MovingCreatureManager_H__

#include "RulesPrerequisites.h"
#include "GameTask.h"
#include "PhysicsGenericContactCallback.h"
#include <vector>
#include <map>



namespace rl
{
    class MovingCreature;

    class _RlRulesExport MovingCreatureManager : 
        public GameTask,
        public Ogre::Singleton<MovingCreatureManager>,
        public PhysicsGenericContactCallback
    {
    public:
        MovingCreatureManager();
        ~MovingCreatureManager();

        /**
         * adds a new movingCreature, this function is not intended to be used directly; 
         * each MovingCreature is automatically added. a creature is always added in a
         * idle state
         */
        void add(MovingCreature *movingCreature);

        /**
         * removes a movingCreature, this function is not intended to be used directly, 
         * destroy the movingCreature instead.
         */
        void remove(MovingCreature *movingCreature);

        void run(Ogre::Real elapsedTime);

        /**
         * Sets a movingCreature in an active state, this means that it is updated every
         * frame. This must be the case, if the creature is moving. If a creature is in an
         * idle state, it is only updated less often
         */
        void setActive(MovingCreature* movingCreature);

        const Ogre::String & getName() const {return mName;}

        // Newton Contact Callback
        int userProcess();
    protected:
        typedef std::vector<MovingCreature*> MovingCreatureVector;
        MovingCreatureVector mActiveCreatures;
        MovingCreatureVector mIdleCreatures;
        MovingCreatureVector mAddToActiveCreatures;
        Ogre::Real mUpdateIdleTime;
        Ogre::Real mTimeSinceLastIdleUpdate;
        Ogre::String mName;

        typedef std::map<OgreNewt::Body*,MovingCreature*> MovingCreatureBodyMap;
        MovingCreatureBodyMap mMovingCreatureFromBody;
    };
}


#endif
