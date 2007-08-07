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
    class CreatureController;

    /// This class manages CreatureControllers, which provide an API for moving the creature
    /// around in the scene.
    /// CreatureControllers are created on demand and a reference is kept here, so that no more
    /// than one CreatureController is created per Creature.
    class _RlRulesExport CreatureControllerManager : 
        public GameTask,
        public Ogre::Singleton<CreatureControllerManager>,
        public PhysicsGenericContactCallback
    {
    public:
        CreatureControllerManager();
        ~CreatureControllerManager();

        /// Returns a CreatureController that can be used to control given Creature.
        /// There is only one controller per Creature at a given time.
        /// If no such controller exists yet, it is created.
        CreatureController* getCreatureController(Creature* creature);

        /// This function detaches a controller attached to the given Creature, if any.
        void detachController(Creature* creature);

        void run(Ogre::Real elapsedTime);

        // override from GameTask
        const Ogre::String& getName() const;

        // Newton Contact Callback
        int userProcess();
    protected:
        typedef std::map<Creature*, CreatureController*> ControllerMap;
        ControllerMap mControllers;

        typedef std::map<OgreNewt::Body*, CreatureController*> BodyControllerMap;
        BodyControllerMap mBodyControllers;
    };
}
#endif
