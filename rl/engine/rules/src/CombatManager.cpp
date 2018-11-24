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
#include "Kampfaktion.h"

using namespace Ogre;

template <> rl::CombatManager* Singleton<rl::CombatManager>::msSingleton = 0;

namespace rl
{

    CombatManager::CombatManager()
        : mCurrentCombat(NULL)
    {
        Kampfaktion* ka = new AttackeAktion();
        mKampfaktionen.insert(std::make_pair(ka->getName(), ka));
        ka = new ParadeAktion();
        mKampfaktionen.insert(std::make_pair(ka->getName(), ka));
        ka = new AusweichenAktion();
        mKampfaktionen.insert(std::make_pair(ka->getName(), ka));
        ka = new BewegenAktion();
        mKampfaktionen.insert(std::make_pair(ka->getName(), ka));
        ka = new FolgenAktion();
        mKampfaktionen.insert(std::make_pair(ka->getName(), ka));
    }

    CombatManager::~CombatManager()
    {
        for (std::map<CeGuiString, Kampfaktion*>::iterator it = mKampfaktionen.begin(); it != mKampfaktionen.end();
             ++it)
        {
            delete it->second;
        }
        mKampfaktionen.clear();
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

    void CombatManager::stopCombat()
    {
        delete mCurrentCombat;
        mCurrentCombat = NULL;
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
        for (CombatantFactoryMap::iterator it = mCombatantFactories.begin(); it != mCombatantFactories.end(); ++it)
        {
            if (it->second == factory)
            {
                mCombatantFactories.erase(it);
                break;
            }
        }
    }

    Kampfaktion* CombatManager::getKampfaktion(const CeGuiString& name)
    {
        std::map<CeGuiString, Kampfaktion*>::const_iterator it = mKampfaktionen.find(name);
        if (it != mKampfaktionen.end())
        {
            return it->second;
        }
        else
        {
            return NULL;
        }
    }
}
