/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "Action.h"
#include "GameObject.h"
#include "Exception.h"
#include "GameActor.h"
#include "RulesSubsystem.h"

using namespace std;

namespace rl
{
    GameObject::GameObject(int id,
                           const CeGuiString& name,
                           const CeGuiString& description) : mId(id),
                                                             mName(name),
                                                             mDescription(description)
    {
        // Standardactions registrieren
    }

    GameObject::~GameObject(void)
    {
    }

    int GameObject::getId() const
    {
        return mId;
    }

    const CeGuiString& GameObject::getName() const
    {
        return mName;
    }

	/*const char* GameObject::getName() const
	{
		return mName.c_str();
	}*/

    void GameObject::setName(CeGuiString& name)
    {
        mName = name;
    }

	/*void GameObject::setName(const char* name)
    {
		mName = CeGuiString(name);
    }*/

    const CeGuiString& GameObject::getDescription() const
    {
        return mDescription;
    }

    void GameObject::setDescription(CeGuiString& description)
    {
        mDescription = description;
    }

    void GameObject::addAction(Action* action, int option)
    {
        if (action == NULL)
        {
            Throw(NullPointerException, "Parameter action ist NULL.");
        }

		mActions.push_back(make_pair(action, option));		
    }

    void GameObject::removeAction(Action* action)
    {
		ActionOptionVector::iterator it = findAction(mActions.begin(), mActions.end(), action);
        if (it != mActions.end())
        {
            mActions.erase(it);
        }
    }

    const ActionVector GameObject::getValidActions() const
    {
		ActionVector actions;
		for (ActionOptionVector::const_iterator it = mActions.begin(); it != mActions.end(); it++)
		{
			if ((*it).second == ACT_DISABLED)
				continue;
			//if ((*it).second > ACT_NEEDS_TALENT)
				
			actions.push_back((*it).first);
		}
        return actions;
    }

    void GameObject::doAction(const CeGuiString& className,
                              const CeGuiString& actionName,
                              Creature* actor,
                              GameObject* target)
    {
		ActionOptionVector::const_iterator it = findAction(mActions.begin(), mActions.end(), className, actionName);

		if (it == mActions.end())
        {
            Throw(InvalidArgumentException, "Dem Objekt unbekannte Aktion.");
        }
        
		doAction((*it).first, actor, target);
    }

	void GameObject::doAction(Action* action,
                              Creature* actor,
                              GameObject* target)
	{
		action->doAction(this, actor, target);
	}
	
	bool GameObject::activateAction(Action* action,
                      Creature* actor,
                      GameObject* target)
	{
		doAction(action, actor, target);
		return true;
	}

	GameObject::ActionOptionVector::iterator 
		GameObject::findAction(
			GameObject::ActionOptionVector::iterator begin, 
			GameObject::ActionOptionVector::iterator end, 
			const CeGuiString& actionName)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; iter++)
			if ((*iter).first->getName().compare(actionName) == 0)
				return iter;

		return end;
	}

	GameObject::ActionOptionVector::iterator 
		GameObject::findAction(
			GameObject::ActionOptionVector::iterator begin, 
			GameObject::ActionOptionVector::iterator end, 
			const CeGuiString& className,
			const CeGuiString& actionName)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; iter++)
		{
			Action* action = (*iter).first;

			if (action->getName().compare(actionName) == 0 &&
				action->getClassName().compare(className) == 0)
				return iter;
		}
		return end;
	}

	GameObject::ActionOptionVector::iterator 
		GameObject::findAction(
			GameObject::ActionOptionVector::iterator begin, 
			GameObject::ActionOptionVector::iterator end, 
			const Action* action)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; iter++)
			if ((*iter).first == action)
				return iter;

		return end;
	}
	
	void GameObject::setActor(GameActor* actor)
	{
		mActor = actor;
	}
	
	GameActor* GameObject::getActor()
	{
		return mActor;
	}

}
