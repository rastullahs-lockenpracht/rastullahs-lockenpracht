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
#include "stdinc.h" //precompiled header

#include "EffectManager.h"

#include <sstream>

#include "CoreSubsystem.h"
#include "Creature.h"
#include "DsaManager.h"
#include "EffectFactory.h"
#include "Exception.h"
#include "MessagePump.h"
#include "RulesMessages.h"
#include "ScriptWrapper.h"

namespace rl
{
    const Ogre::String EffectManager::PROPERTY_TIMECHECK = "timechecks";
    const Ogre::String EffectManager::PROPERTY_TIME = "time";
    const Ogre::String EffectManager::PROPERTY_EFFECT = "effect";

    EffectManager::EffectManager(GameObject* gameobject)
        : mGameObject(gameobject)
        , mCheckEffectsRunning(false)
    {
    }

    EffectManager::~EffectManager()
    {
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            // delete (*it);
            ScriptWrapper::getSingleton().disowned(*it);
        }
    }

    void EffectManager::checkEffects()
    {
        if (mCheckEffectsRunning)
        {
            return;
        }
        mCheckEffectsRunning = true;
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        Checklist::iterator checkIt = mChecklist.begin();
        if (checkIt == mChecklist.end())
            return;
        while (checkIt != mChecklist.end() && checkIt->first <= now)
        {
            std::stringstream debugInfo;
            debugInfo << "Effect check: " << checkIt->first << " now: " << now << std::endl;
            LOG_DEBUG(Logger::RULES, debugInfo.str());
            for (Effects::iterator effIt = checkIt->second.begin(); effIt != checkIt->second.end(); effIt++)
            {
                int nextCheck;
                nextCheck = (*effIt)->timeCheck();
                switch (nextCheck)
                {
                case Effect::REMOVE:
                    removeEffect(*effIt);
                    break;
                case Effect::PERMANENT:
                    break;
                default:
                    addTimeCheck(nextCheck, *effIt);
                }
            }
            mChecklist.erase(checkIt++);
        }

        mCheckEffectsRunning = false;
    }

    void EffectManager::addTimeCheck(RL_LONGLONG timeUntilCheck, Effect* effect)
    {
        // Preconditions: time > 0, effect != NULL
        if (time <= 0)
            Throw(IllegalArgumentException, "time parameter is <= 0!");
        if (effect == NULL)
            Throw(IllegalArgumentException, "effect pointer is NULL!");
        // Get current ingame time and add timeUntilCheck
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        RL_LONGLONG timeForCheck = now + timeUntilCheck;
        std::stringstream debugInfo;
        debugInfo << "Adding check! now:" << now << " check: " << timeForCheck << std::endl;
        LOG_DEBUG(Logger::RULES, debugInfo.str());
        // Insert Sum and effect into the checklist
        mChecklist[timeForCheck].push_back(effect);
    }

    void EffectManager::addDateCheck(RL_LONGLONG date, Effect* effect)
    {
        // Preconditions: date > now, effect != NULL
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        if (date <= now)
        {
            Throw(IllegalArgumentException, "date lies in the past!");
        }
        if (effect == NULL)
        {
            Throw(IllegalArgumentException, "effect pointer is NULL!");
        }
        // Insert date and effect into the checklist
        std::stringstream debugInfo;
        debugInfo << "Adding check! now:" << now << " check: " << date << std::endl;
        LOG_DEBUG(Logger::RULES, debugInfo.str());
        mChecklist[date].push_back(effect);
    }

    void EffectManager::addEffect(Effect* effect)
    {
        Effect::LifeState oldState = getLifeState();

        LOG_DEBUG(Logger::RULES, "Adding effect " + effect->getName());
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            Effect* cur = *it;
            if (cur->getName() == effect->getName())
            {
                if (cur->getQuantifier() == Effect::QUANTIFIER_UNIQUE)
                {
                    return;
                }
                else if (cur->getQuantifier() == Effect::QUANTIFIER_UNIQUE_BUT_PROLONGABLE)
                {
                    // TODO
                    // Vielleicht alten durch neuen ersetzen?
                    // oder (*it)->setDuration(effect->getDuration())
                }
                else if (cur->getQuantifier() == Effect::QUANTIFIER_UNIQUE_BUT_CUMULATIVE)
                {
                    for (int i = 0; i < effect->getStufe(); i++)
                    {
                        cur->increaseStufe();
                    }
                }
            }
        }
        ScriptWrapper::getSingleton().owned(effect);
        mEffects.push_back(effect);
        effect->enable();

        checkStateChange(oldState);
    }

    void EffectManager::removeEffect(Effect* effect)
    {
        Effect::LifeState oldState = getLifeState();

        effect->disable();
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); ++it)
        {
            if (*it == effect)
            {
                mEffects.erase(it);
                break;
            }
        }
        ScriptWrapper::getSingleton().disowned(effect);
        ///@todo also remove from the check lists?

        checkStateChange(oldState);
    }

    void EffectManager::removeEffect(CeGuiString name)
    {
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                removeEffect(*it);
                return;
            }
        }
    }

    Effect::LifeState EffectManager::getLifeState()
    {
        checkEffects();
        Effect::LifeState status = 0;
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            status = status | ((*it)->getLifeState());
        }
        return status;
    }

    void EffectManager::checkStateChange(Effect::LifeState oldState)
    {
        Effect::LifeState currentState = getLifeState();
        if (currentState != oldState)
        {
            MessagePump::getSingleton().sendMessage<MessageType_GameObjectLifeStateChanged>(
                mGameObject, oldState, currentState);
        }
    }

    int EffectManager::getMod(CeGuiString target, Effect::ModType type, Effect::ModTag tag)
    {
        checkEffects();
        int mod = 0;
        if (type == Effect::MODTYPE_MULT)
            mod = 1;
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            mod += (*it)->getMod(target, type, tag);
        }
        return mod;
    }

    CeGuiString EffectManager::getEffects()
    {
        CeGuiString retVal = "";
        for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
        {
            retVal += (*it)->getName() + "\n";
            retVal += (*it)->getDescription() + "\n";
        }
        return retVal;
    }

    const Property EffectManager::getProperty(const CeGuiString& key) const
    {
        Property rval;
        if (key == Creature::PROPERTY_EFFECTS)
        {
            PropertyMap map;

            std::map<Effect*, int> effectIds;
            PropertyArray effectProps;
            for (int i = 0, size = mEffects.size(); i < size; ++i)
            {
                Effect* cur = mEffects[i];
                effectProps.push_back(cur->getAllProperties()->toPropertyMap());
                effectIds[cur] = i;
            }

            PropertyArray timeCheck;
            for (std::map<RL_LONGLONG, Effects>::const_iterator itTc = mChecklist.begin(); itTc != mChecklist.end();
                 ++itTc)
            {
                for (Effects::const_iterator itEff = itTc->second.begin(); itEff != itTc->second.end(); ++itEff)
                {
                    PropertyMap tcElem;
                    tcElem[EffectManager::PROPERTY_EFFECT] = effectIds[*itEff];
                    tcElem[EffectManager::PROPERTY_TIME] = itTc->first;
                    timeCheck.push_back(tcElem);
                }
            }

            map[Creature::PROPERTY_EFFECTS] = effectProps;
            map[EffectManager::PROPERTY_TIMECHECK] = timeCheck;
            rval.setValue(map);
        }
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of EffectManager.");
        }
        return rval;
    }

    void EffectManager::setProperty(const CeGuiString& key, const Property& value)
    {
        try
        {
            if (key == Creature::PROPERTY_EFFECTS)
            {
                PropertyMap map = value.toMap();

                std::vector<Effect*> effectIds;

                PropertyArray arr = map[Creature::PROPERTY_EFFECTS].toArray();
                for (int i = 0, size = arr.size(); i < size; ++i)
                {
                    PropertyMap cur = arr[i].toMap();
                    Ogre::String name = cur[Effect::PROPERTY_NAME].toString().c_str();
                    int stufe = cur[Effect::PROPERTY_STUFE];
                    Effect* eff = EffectFactoryManager::getSingleton().createEffect(name, stufe);
                    eff->setProperties(cur);
                    mEffects.push_back(eff);
                    ///@TODO: activate effects?
                }

                PropertyArray timeCheckProp = map[EffectManager::PROPERTY_TIMECHECK].toArray();
                for (int i = 0, size = timeCheckProp.size(); i < size; ++i)
                {
                    PropertyMap cur = timeCheckProp[i].toMap();
                    RL_LONGLONG time = cur[EffectManager::PROPERTY_TIME].toLong();
                    Effect* eff = mEffects[cur[EffectManager::PROPERTY_EFFECT].toInt()];
                    mChecklist[time].push_back(eff);
                }
            }
            else
            {
                LOG_WARNING(Logger::RULES, key + " is not a property of EffectManager");
            }
        }
        catch (WrongFormatException ex)
        {
            LOG_ERROR(Logger::RULES, "property " + key + " has the wrong format");
        }
    }

    PropertyKeys EffectManager::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert(Creature::PROPERTY_EFFECTS);
        return keys;
    }
}
