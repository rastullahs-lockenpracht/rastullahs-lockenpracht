/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "ActionManager.h"
#include "Action.h"
#include "Exception.h"

template <>
rl::ActionManager* Singleton<rl::ActionManager> ::ms_Singleton = 0;
namespace rl
{

	ActionVector::iterator findAction(ActionVector::iterator& begin, ActionVector::iterator& end, const CeGuiString& actionName)
	{
		for (ActionVector::iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0)
				return it;

		return end;
	}

	ActionVector::iterator findAction(ActionVector::iterator& begin, ActionVector::iterator& end, const CeGuiString& actionName, const CeGuiString& className)
	{
		for (ActionVector::iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0
				&& (*it)->getClassName().compare(className) == 0)
				return it;

		return end;
	}

	ActionVector::const_iterator findActionConst(ActionVector::const_iterator& begin, ActionVector::const_iterator& end, const CeGuiString& actionName, const CeGuiString& className)
	{
		for (ActionVector::const_iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0
				&& (*it)->getClassName().compare(className) == 0)
				return it;

		return end;
	}

    ActionManager& ActionManager::getSingleton(void)
    {
    return Singleton<ActionManager>::getSingleton();
    }

    ActionManager* ActionManager::getSingletonPtr(void)
    {
        return Singleton<ActionManager>::getSingletonPtr();
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

        mActions.push_back(action);
    }

	void ActionManager::unregisterAction(const CeGuiString& actionName, const CeGuiString& className)
	{
		ActionVector::iterator iter = findAction(mActions.begin(), mActions.end(), actionName, className);
		if (iter == mActions.end())
			Throw(InvalidArgumentException, "Aktion nicht gefunden");
		mActions.erase(iter);
	}

	Action* ActionManager::getAction(const CeGuiString& actionName, const CeGuiString& className) const
	{
		ActionVector::const_iterator iter = 
			findActionConst(
				mActions.begin(), 
				mActions.end(), 
				actionName, 
				className);
		if (iter == mActions.end())
			Throw(InvalidArgumentException, "Aktion nicht gefunden");
		return *iter;
	}


	Action* ActionManager::getDefaultAction(const CeGuiString& actionName) const
    {
		Action* action = NULL;
		for (ActionVector::const_iterator it = mActions.begin(); it != mActions.end(); it++)
        {
			if ((*it)->getName().compare(actionName) == 0)
			{
				action = (*it);
				break;
			}
        }
        if (action == NULL)
        {
            Throw(InvalidArgumentException, "Unbekannte Aktion.");
        }
        return action;
    }

	void ActionManager::registerInGameGlobalAction(Action* action, ActionGroup* group)
	{
		mInGameGlobalActions.push_back(action);
		action->setGroup(group);
	}

	Action* ActionManager::getInGameGlobalAction(const CeGuiString& actionName, const CeGuiString& className) const
	{
		ActionVector::const_iterator iter = 
			findActionConst(
				mInGameGlobalActions.begin(), 
				mInGameGlobalActions.end(), 
				actionName, 
				className);
		if (iter == mInGameGlobalActions.end())
			Throw(InvalidArgumentException, "Aktion nicht gefunden");
		return *iter;
	}

	const ActionVector& ActionManager::getInGameGlobalActions()
	{
		return mInGameGlobalActions;
	}
}
