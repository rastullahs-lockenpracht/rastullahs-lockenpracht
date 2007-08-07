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
#include "stdinc.h"

#include "CombatManager.h"

#include "Combat.h"
#include "Combatant.h"
#include "Exception.h"

using namespace Ogre;

template<> rl::CombatManager* Singleton<rl::CombatManager>::ms_Singleton = 0;

namespace rl
{

    CombatManager::CombatManager() : mCurrentCombat(NULL)
    {
    }

    CombatManager::~CombatManager()
    {
    }

    Combat* CombatManager::startCombat(Combatant* character, Combatant* firstOpponent)
    {
        if (mCurrentCombat != NULL)
        {
            Throw(IllegalStateException, "There already is a combat running.");
        }

        mCurrentCombat = new Combat(character);
        mCurrentCombat->addOpponent(firstOpponent);

        return mCurrentCombat;
    }

    Combat* CombatManager::getCurrentCombat() const
    {
        return mCurrentCombat;
    }

    Combatant* CombatManager::createCombatant(Creature* creature, const String& combatantType)
    {
        CombatantFactoryMap::iterator it = mCombatantFactories.find(combatantType);
        if (it != mCombatantFactories.end())
        {
            return it->second->createCombatant(creature);
        }
        else
        {
            Throw(IllegalArgumentException, "No such CombatantType registered: " + combatantType);
        }
    }

    void CombatManager::destroyCombatant(Combatant* combatant)
    {
    }

    void CombatManager::registerCombatantFactory(const String& name, CombatantFactory* factory)
    {
    }

    void CombatManager::unregisterCombatantFactory(CombatantFactory* factory)
    {
    }
}
