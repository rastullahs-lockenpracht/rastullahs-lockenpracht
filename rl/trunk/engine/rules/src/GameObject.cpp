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

#include "Action.h"
#include "ObjectStateChangeEventSource.h"
#include "GameObject.h"
#include "Exception.h"
#include "RulesSubsystem.h"

using namespace std;

namespace rl
{
    GameObject::GameObject(int id,
                           const CeGuiString& name,
                           const CeGuiString& description)
        :   mId(id),
            mName(name),
            mDescription(description)
    {
        // Standardactions registrieren

		// Eventsource erzeugen
		setObject(this);
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

    void GameObject::setName(CeGuiString& name)
    {
        mName = name;
    }

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

	void GameObject::addActionInGroup(Action* action, ActionGroup* group, int option)
    {
        addAction(action, option);
		action->setGroup(group);
	}

    void GameObject::removeAction(Action* action)
    {
		ActionOptionVector::iterator it = findAction(mActions.begin(), mActions.end(), action);
        if (it != mActions.end())
        {
            mActions.erase(it);
        }
    }

	
    const ActionVector GameObject::getValidActions(Creature* actor) const
    {
		ActionVector actions;
		for (ActionOptionVector::const_iterator it = mActions.begin(); it != mActions.end(); ++it)
		{
			if ((*it).second == Action::ACT_DISABLED)
				continue;
			//if ((*it).second > ACT_NEEDS_TALENT)
			if (!(*it).first->canDo(const_cast<GameObject*>(this), actor)) // Aktion nicht möglich
				continue;
				
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
            std::stringstream strstr;
			strstr << "[Name:'" << actionName.c_str() << "', Klasse: '" << className.c_str() << "'] ist eine dem Objekt unbekannte Aktion.";
            Throw(InvalidArgumentException, strstr.str());
        }
        
		doAction((*it).first, actor, target);
    }

	void GameObject::doAction(const CeGuiString& className,
							  const CeGuiString& actionName)
	{
		doAction(className, actionName, NULL, NULL);
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
		for (ActionOptionVector::iterator iter = begin; iter != end; ++iter)
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
		for (ActionOptionVector::iterator iter = begin; iter != end; ++iter)
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
		for (ActionOptionVector::iterator iter = begin; iter != end; ++iter)
			if ((*iter).first == action)
				return iter;

		return end;
	}
	
	void GameObject::setActor(Actor* actor)
	{
		mActor = actor;
		mActor->setGameObject(this);
	}
	
	Actor* GameObject::getActor()
	{
		return mActor;
	}


    bool GameObject::isHigligthingEnabled()
    {
        return mHighlightingEnabled;
    }
    
    void GameObject::setHigligthingEnabled( bool highlightenabled )
    {
        // Leuchtet zur Zeit, sollte aber nicht leuchten
        if( mActor != NULL && !highlightenabled && mActor->isHighlighted() )
        {
            mActor->setHighlighted( false );
        }

        mHighlightingEnabled = highlightenabled;
    }

	void GameObject::setString(CeGuiString key, CeGuiString value)
	{
        mAttributesString[key] = value;
		fireObjectStateChangeEvent();
	}

	void GameObject::setBool(CeGuiString key, bool value)
	{
		mAttributesBoolean[key] = value;
		fireObjectStateChangeEvent();
	}

	void GameObject::setInt(CeGuiString key, int value)
	{
		mAttributesInteger[key] = value;
		fireObjectStateChangeEvent();
	}

	void GameObject::setReal(CeGuiString key, Ogre::Real value)
	{
		mAttributesReal[key] = value;
		fireObjectStateChangeEvent();
	}

	CeGuiString GameObject::getString(CeGuiString key)
	{
		std::map<CeGuiString, CeGuiString>::iterator iter = mAttributesString.find(key);
		for (iter = mAttributesString.begin(); iter != mAttributesString.end(); ++iter)
		{
			CeGuiString str = "getString: ['";
			str.append((*iter).first);
			str.append("','");
			str.append((*iter).second);
			str.append("']");
			RulesSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, str);
		}
		iter = mAttributesString.find(key);

		if (iter != mAttributesString.end())
			return (*iter).second;
		else
			return "";
	}

	bool GameObject::getBool(CeGuiString key)
	{
		std::map<CeGuiString, bool>::iterator iter = mAttributesBoolean.find(key);
		if (iter != mAttributesBoolean.end())
			return (*iter).second;
		else
			return false;
	}

	int GameObject::getInt(CeGuiString key)
	{
		std::map<CeGuiString, int>::iterator iter = mAttributesInteger.find(key);
		if (iter != mAttributesInteger.end())
			return (*iter).second;
		else
			return 0;
	}

	Ogre::Real GameObject::getReal(CeGuiString key)
	{
		std::map<CeGuiString, Ogre::Real>::iterator iter = mAttributesReal.find(key);
		if (iter != mAttributesReal.end())
			return (*iter).second;
		else
			return 0.0;
	}
}
