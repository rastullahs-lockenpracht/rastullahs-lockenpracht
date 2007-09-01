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
#include "stdinc.h" //precompiled header

#include <sstream>

#include "EffectManager.h"
#include "DsaManager.h"
#include "Exception.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"
#include "ScriptWrapper.h"

namespace rl
{
	EffectManager::EffectManager()
	{
	}

	EffectManager::~EffectManager()
	{
		for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
		{
			//delete (*it);
            ScriptWrapper::getSingleton().disowned(*it);
		}
	}

	void EffectManager::checkEffects()
	{
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        Checklist::iterator checkIt = mChecklist.begin();
        if (checkIt == mChecklist.end()) return;
        while ( checkIt != mChecklist.end() && checkIt->first <= now )
        {
				std::stringstream debugInfo;
				debugInfo << "Effect check: " << checkIt->first << " now: " << now << std::endl;
                LOG_DEBUG(Logger::RULES,
					debugInfo.str());
            for (Effects::iterator effIt = checkIt->second.begin(); effIt != checkIt->second.end(); effIt++)
            {
                int nextCheck;
                nextCheck = (*effIt)->check();
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
	}

    void EffectManager::addTimeCheck(RL_LONGLONG timeUntilCheck, Effect* effect)
    {
        // Preconditions: time > 0, effect != NULL
        if (time <= 0) Throw(IllegalArgumentException, "time parameter is <= 0!");
        if (effect == NULL) Throw(IllegalArgumentException, "effect pointer is NULL!");
        // Get current ingame time and add timeUntilCheck
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        RL_LONGLONG timeForCheck = now + timeUntilCheck;
				std::stringstream debugInfo;
				debugInfo << "Adding check! now:" << now << " check: " << timeForCheck << std::endl;
                LOG_DEBUG(Logger::RULES,
					debugInfo.str());
        // Insert Sum and effect into the checklist
        mChecklist[timeForCheck].insert(effect);
    }

    void EffectManager::addDateCheck(RL_LONGLONG date, Effect* effect)
    {
        // Preconditions: date > now, effect != NULL
        RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();
        if (date <= now) Throw(IllegalArgumentException, "date lies in the past!");
        if (effect == NULL) Throw(IllegalArgumentException, "effect pointer is NULL!");
        // Insert date and effect into the checklist
				std::stringstream debugInfo;
				debugInfo << "Adding check! now:" << now << " check: " << date << std::endl;
                LOG_DEBUG(Logger::RULES,
					debugInfo.str());
        mChecklist[date].insert(effect);
    }


	void EffectManager::addEffect(Effect* effect)
	{
				std::stringstream debugInfo;
				debugInfo << "Adding effect " << effect->getName() << std::endl;
                LOG_DEBUG(Logger::RULES,
					debugInfo.str());
		for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
		{
			if ((*it)->getName() == effect->getName())
			{
				if ((*it)->getQuantifier() == Effect::QUANTIFIER_UNIQUE) return;
				if ((*it)->getQuantifier() == Effect::QUANTIFIER_UNIQUE_BUT_PROLONGABLE)
				{
					//TODO
					//Vielleicht alten durch neuen ersetzen?
					//oder (*it)->setDuration(effect->getDuration())
				}
				if ((*it)->getQuantifier() == Effect::QUANTIFIER_UNIQUE_BUT_CUMULATIVE)
				{
					for (int i = 0; i < effect->getStufe(); i++)
						(*it)->increaseStufe();
				}
			}
		}
        ScriptWrapper::getSingleton().owned( effect );
		mEffects.insert(effect);
		effect->enable();
	}

    void EffectManager::removeEffect(Effect* effect)
    {
      mEffects.erase(effect);
      ScriptWrapper::getSingleton().disowned(effect);
      ///@todo also remove from the check lists?
    }

    void EffectManager::removeEffect(CeGuiString name)
    {
		for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
		{
            if ((*it)->getName() == name)
            {
                ScriptWrapper::getSingleton().disowned(*it);
                mEffects.erase(it);
                return;
            }
        }
    }



    Effect::Status EffectManager::getStatus()
    {
      checkEffects();
      Effect::Status status = 0;
      for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
      {
            status = status | ((*it)->getStatus());
      }
      return status;
    }

    int EffectManager::getMod(CeGuiString target, Effect::ModType type, Effect::ModTag tag)
    {
      checkEffects();
      int mod = 0;
	  if (type == Effect::MODTYPE_MULT) mod = 1;
      for (Effects::iterator it = mEffects.begin(); it != mEffects.end(); it++)
      {
        mod += (*it)->getMod(target, type, tag);
      }
      return mod;
    }
}

