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

    Combat* CombatManager::startCombat()
    {
        if (mCurrentCombat != NULL)
        {
            Throw(IllegalStateException, "There already is a combat running.");
        }

        mCurrentCombat = new Combat();

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
        String name = combatant->getCombatantTypeName();
        CombatantFactoryMap::iterator it = mCombatantFactories.find(name);
        if (it != mCombatantFactories.end())
        {
            return it->second->destroyCombatant(combatant);
        }
        else
        {
            Throw(IllegalArgumentException, "No such CombatantType registered: " + name);
        }
    }

    void CombatManager::registerCombatantFactory(const String& name, CombatantFactory* factory)
    {
        // If there was one already, it is overwritten.
        mCombatantFactories[name] = factory;
    }

    void CombatManager::unregisterCombatantFactory(CombatantFactory* factory)
    {
        for (CombatantFactoryMap::iterator it = mCombatantFactories.begin();
            it != mCombatantFactories.end(); ++it)
        {
            if (it->second == factory)
            {
                mCombatantFactories.erase(it);
                break;
            }
        }
    }
}
