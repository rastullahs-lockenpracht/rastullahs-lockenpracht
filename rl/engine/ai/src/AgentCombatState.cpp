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

#include "Agent.h"
#include "AgentCombatState.h"
#include "Combat.h"
#include "CombatManager.h"
#include "Creature.h"
#include "CreatureControllerManager.h"
#include "CreatureWalkPathJob.h"

namespace rl
{

    AgentCombatState::AgentCombatState(Agent* agent)
        : AgentState(agent)
        , Combatant(CombatManager::getSingleton().getCurrentCombat(),
              CreatureControllerManager::getSingleton().getCreatureController(agent->getControlledCreature()))
        , mState()
    {
    }

    AgentCombatState::~AgentCombatState()
    {
    }

    Ogre::String AgentCombatState::getCombatantTypeName() const
    {
        return "default";
    }

    void AgentCombatState::requestCombatantAction()
    {
        // Think!
        Combat::CombatantSet opponents = mCombat->getAllOpponents(this);
        if (!opponents.empty())
        {
            Combatant* target = findOpponent(opponents);

            mCombat->registerParade(target);

            if (getCreature()->getLe() <= 5)
            {
                mCombat->registerBewegen(this, getFleeTarget(opponents));
            }
            else if (target)
            {
                // Are we in weapon range to opponent
                if (mCombat->canAttack(this, target))
                {
                    // Ok, we can attack
                    mCombat->registerAttacke(this, target);
                }
                else
                {
                    // We can't attack from here, so go to opponent.
                    mCombat->registerFolgen(this, target);
                }
            }
        }
        mCombat->registerCombatantRoundDone(this);
    }

    void AgentCombatState::update(const float elapsedTime)
    {
        // we don't use the vehicle (combat uses GoToJob, the vehicle would change the movement and reset the
        // animation!)
        // mAgent->updateVehicle(0, elapsedTime);
    }

    class DistanceComparator : std::binary_function<Combatant*, Combatant*, bool>
    {
    private:
        const Combatant* mActingCombatant;

    public:
        DistanceComparator(const Combatant* actingCombatant)
            : mActingCombatant(actingCombatant)
        {
        }

        bool operator()(const Combatant* c1, const Combatant* c2) const
        {
            Vector3 pos = mActingCombatant->getPosition();
            return pos.squaredDistance(c1->getPosition()) < pos.squaredDistance(c2->getPosition());
        }
    };

    Combatant* AgentCombatState::findOpponent(const Combat::CombatantSet& opponents) const
    {
        ///@todo support different search patterns (search for most dangerous, weakest, ... opponent)
        Combat::CombatantSet::const_iterator minIt
            = std::min_element(opponents.begin(), opponents.end(), DistanceComparator(this));
        return *minIt;
    }

    Vector3 AgentCombatState::getFleeTarget(const Combat::CombatantSet& opponents) const
    {
        Vector3 oppPosition;
        for (Combat::CombatantSet::const_iterator it = opponents.begin(); it != opponents.end(); ++it)
        {
            oppPosition += (*it)->getPosition();
        }
        oppPosition /= opponents.size();

        Vector3 pos = getPosition();
        Vector3 oppositeDirection = (pos - oppPosition).normalisedCopy();
        return pos + 10 * oppositeDirection;
    }
}
