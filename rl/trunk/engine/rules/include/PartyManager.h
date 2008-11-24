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
#ifndef __PartyManager_H__
#define __PartyManager_H__

#include "RulesPrerequisites.h"

#include "Effect.h"
#include "MessagePump.h"

namespace rl
{
    class Creature;

    typedef std::vector<Creature*> Party;

    class _RlRulesExport PartyManager : public Ogre::Singleton<PartyManager>
    {
    public:
		PartyManager();
        
        Creature* getActiveCharacter() const;
        void setActiveCharacter(Creature* character);
        void addCharacter(Creature* character);
        void removeCharacter(Creature* character);
        Party getCharacters() const;
        bool isInParty(Creature* creature) const;
        
    private:
		Party mParty;
        Creature* mActiveCharacter;
	    MessagePump::ScopedConnection mLifeStateChangeConnection;

        bool onGameObjectLifeStateChanged(GameObject* gameobject, Effect::LifeState oldstate,
            Effect::LifeState newstate);
            
        void checkParty();
   };
}

#endif // __PartyManager_H__
