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

#include "PartyManager.h"

#include <boost/bind.hpp>

#include "Creature.h"
#include "RulesMessages.h"

template <> rl::PartyManager* Ogre::Singleton<rl::PartyManager>::msSingleton = 0;

namespace rl
{

    PartyManager::PartyManager()
        : mActiveCharacter(NULL)
    {
        ///@todo watch characters for death, trigger game over if no-one is left
        LOG_MESSAGE("PartyManager", "Register message handler");
        mLifeStateChangeConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectLifeStateChanged>(
                boost::bind(&PartyManager::onGameObjectLifeStateChanged, this, _1, _2, _3));
    }

    void PartyManager::addCharacter(Creature* character)
    {
        mParty.push_back(character);
        MessagePump::getSingleton().sendMessage<MessageType_PlayerCharRemoved>(character);

        if (mActiveCharacter == NULL)
        {
            setActiveCharacter(character);
        }
    }

    void PartyManager::removeCharacter(Creature* character)
    {
        for (std::vector<Creature*>::iterator it = mParty.begin(); it != mParty.end(); ++it)
        {
            if (*it == character)
            {
                mParty.erase(it);
                MessagePump::getSingleton().sendMessage<MessageType_PlayerCharRemoved>(character);
                break;
            }
        }
        checkParty();
    }

    void PartyManager::setActiveCharacter(Creature* character)
    {
        if (!isInParty(character))
        {
            addCharacter(character);
        }
        if (mActiveCharacter != character)
        {
            Creature* oldActiveChar = mActiveCharacter;
            mActiveCharacter = character;
            MessagePump::getSingleton().sendMessage<MessageType_ActivePlayerCharChanged>(
                oldActiveChar, mActiveCharacter);
        }
    }

    Creature* PartyManager::getActiveCharacter() const
    {
        return mActiveCharacter;
    }

    std::vector<Creature*> PartyManager::getCharacters() const
    {
        return mParty;
    }

    bool PartyManager::isInParty(Creature* creature) const
    {
        for (Party::const_iterator it = mParty.begin(); it != mParty.end(); ++it)
        {
            if (*it == creature)
            {
                return true;
            }
        }

        return false;
    }

    bool PartyManager::onGameObjectLifeStateChanged(
        GameObject* gameobject, Effect::LifeState oldstate, Effect::LifeState newstate)
    {
        LOG_DEBUG("PartyManager", "Someone changed state.");
        if (0 != (newstate & Effect::LS_DEAD | Effect::LS_UNCONSCIOUS | Effect::LS_INCAPACITATED))
        {
            LOG_DEBUG("PartyManager", "Someone died or was rendered unconscious.");
            Creature* crea = dynamic_cast<Creature*>(gameobject);
            if (crea && isInParty(crea))
            {
                checkParty();
            }
        }

        return false;
    }

    void PartyManager::checkParty()
    {
        int alive = 0;
        for (std::vector<Creature*>::iterator it = mParty.begin(); it != mParty.end(); ++it)
        {
            Creature* cur = *it;
            if (!(cur->getLifeState() & Effect::LS_DEAD | Effect::LS_UNCONSCIOUS | Effect::LS_INCAPACITATED))
            {
                alive++;
            }
        }

        if (alive == 0)
        {
            MessagePump::getSingleton().sendMessage<MessageType_AllPlayerCharsDied>();
        }
    }
}
