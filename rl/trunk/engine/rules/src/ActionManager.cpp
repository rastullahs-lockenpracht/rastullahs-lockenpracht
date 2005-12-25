/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "ActionManager.h"
#include "Action.h"
#include "Exception.h"
#include "RulesSubsystem.h"

#include "ScriptObjectRepository.h"

template <>
rl::ActionManager* Singleton<rl::ActionManager> ::ms_Singleton = 0;
namespace rl
{

	ActionVector::iterator findAction(ActionVector::iterator begin, ActionVector::iterator end, const CeGuiString& actionName)
	{
		for (ActionVector::iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0)
				return it;

		return end;
	}

	ActionVector::const_iterator findActionConst(ActionVector::const_iterator begin, ActionVector::const_iterator end, const CeGuiString& actionName)
	{
		for (ActionVector::const_iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0)
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

		if (mActions.find(action->getName()) != mActions.end())
		{
			Throw(
				InvalidArgumentException, 
				("Action "+action->getName()+" bereits registriert").c_str());
		}

		ScriptObjectRepository::getSingleton().own(action);

		mActions.insert(std::make_pair(action->getName(), action));
		Logger::getSingleton().log(Logger::RULES,
			Ogre::LML_TRIVIAL, "Action "+action->getName()+" beim ActionManager registriert");
    }

	void ActionManager::unregisterAction(const CeGuiString& actionName)
	{
		ActionMap::iterator iter = mActions.find(actionName);
		if (iter == mActions.end())
			Throw(InvalidArgumentException, "Aktion nicht gefunden");

		ScriptObjectRepository::getSingleton().disown((*iter).second);
		mActions.erase(iter);

		Logger::getSingleton().log(Logger::RULES,
			Ogre::LML_TRIVIAL, "Action "+actionName+" beim ActionManager gelöscht");
	}

	Action* ActionManager::getAction(const CeGuiString& actionName) const
	{
		Logger::getSingleton().log(Logger::RULES,
			Ogre::LML_TRIVIAL, "Suche Action " + actionName);
		ActionMap::const_iterator iter = mActions.find(actionName);
		if (iter == mActions.end())
			return NULL;
		return (*iter).second;
	}

	void ActionManager::registerInGameGlobalAction(Action* action, ActionGroup* group)
	{
		Logger::getSingleton().log(Logger::RULES,
			Ogre::LML_TRIVIAL, "Globale Aktion " + action->getName() + " hinzugefuegt.");
		mInGameGlobalActions.push_back(action);
		action->setGroup(group);
	}

	Action* ActionManager::getInGameGlobalAction(const CeGuiString& actionName) const
	{
		ActionVector::const_iterator iter = 
			findActionConst(
				mInGameGlobalActions.begin(), 
				mInGameGlobalActions.end(), 
				actionName);
		if (iter == mInGameGlobalActions.end())
			return NULL;
		return *iter;
	}

	const ActionVector& ActionManager::getInGameGlobalActions()
	{
		return mInGameGlobalActions;
	}
}
