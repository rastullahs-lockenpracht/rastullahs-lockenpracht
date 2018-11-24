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

#include "Action.h"
#include "ActionManager.h"
#include "Exception.h"
#include "RulesSubsystem.h"

#include "ScriptWrapper.h"

template <> rl::ActionManager* Singleton<rl::ActionManager>::msSingleton = 0;
namespace rl
{

    ActionVector::iterator findAction(
        ActionVector::iterator begin, ActionVector::iterator end, const CeGuiString actionName)
    {
        for (ActionVector::iterator it = begin; it != end; it++)
            if ((*it)->getName().compare(actionName) == 0)
                return it;

        return end;
    }

    ActionVector::const_iterator findActionConst(
        ActionVector::const_iterator begin, ActionVector::const_iterator end, const CeGuiString actionName)
    {
        for (ActionVector::const_iterator it = begin; it != end; it++)
            if ((*it)->getName().compare(actionName) == 0)
                return it;

        return end;
    }

    ActionManager::ActionManager()
    {
    }

    ActionManager::~ActionManager()
    {
    }

    void ActionManager::registerAction(Action* action)
    {
        if (action == NULL)
        {
            Throw(NullPointerException, "Parameter action ist NULL.");
        }

        if (mActions.find(action->getName()) != mActions.end())
        {
            Throw(IllegalArgumentException, ("Action " + action->getName() + " bereits registriert").c_str());
        }

        ScriptWrapper::getSingleton().owned(action);

        mActions.insert(std::make_pair(action->getName(), action));
        LOG_MESSAGE(Logger::RULES, "Action " + action->getName() + " beim ActionManager registriert");
    }

    void ActionManager::unregisterAction(const CeGuiString actionName)
    {
        ActionMap::iterator iter = mActions.find(actionName);
        if (iter == mActions.end())
            Throw(IllegalArgumentException, "Aktion nicht gefunden");

        ScriptWrapper::getSingleton().disowned((*iter).second);
        mActions.erase(iter);

        LOG_MESSAGE(Logger::RULES, "Action " + actionName + " beim ActionManager geloescht");
    }

    Action* ActionManager::getAction(const CeGuiString actionName) const
    {
        LOG_MESSAGE(Logger::RULES, "Suche Action " + actionName);
        ActionMap::const_iterator iter = mActions.find(actionName);
        if (iter == mActions.end())
            return NULL;
        return (*iter).second;
    }

    void ActionManager::registerInGameGlobalAction(Action* action, ActionGroup* group)
    {
        LOG_MESSAGE(Logger::RULES, "Globale Aktion " + action->getName() + " hinzugefuegt.");
        mInGameGlobalActions.push_back(action);
        action->setGroup(group);
    }

    Action* ActionManager::getInGameGlobalAction(const CeGuiString actionName) const
    {
        ActionVector::const_iterator iter
            = findActionConst(mInGameGlobalActions.begin(), mInGameGlobalActions.end(), actionName);
        if (iter == mInGameGlobalActions.end())
            return NULL;
        return *iter;
    }

    const ActionVector& ActionManager::getInGameGlobalActions()
    {
        return mInGameGlobalActions;
    }
}
