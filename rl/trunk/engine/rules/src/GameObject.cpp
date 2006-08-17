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

#include "Action.h"
#include "ObjectStateChangeEventSource.h"
#include "GameObject.h"
#include "Exception.h"
#include "RulesSubsystem.h"
#include "ActionManager.h"

using namespace std;

namespace rl
{
	const CeGuiString GameObject::DEFAULT_VIEW_OBJECT_ACTION = "viewobject";
	int GameObject::sNextGameObjectId = 1;

    GameObject::GameObject(const CeGuiString name,
                           const CeGuiString description)
		:   mId(GameObject::sNextGameObjectId++),
            mName(name),
            mDescription(description),
			mHighlightingEnabled(true),
			mActor(NULL)
    {
        // Standardactions registrieren
		Action* defaultAction = ActionManager::getSingleton().getAction(DEFAULT_VIEW_OBJECT_ACTION);
		if (defaultAction != NULL)
			addAction(defaultAction);

		// Eventsource erzeugen
		setObject(this);
    }

    GameObject::~GameObject(void)
    {
	}

	GameObject* GameObject::clone()
	{
		return new GameObject(mName,mDescription);
	}

    int GameObject::getId() const
    {
        return mId;
    }

    const CeGuiString GameObject::getName() const
    {
        return mName;
    }

    void GameObject::setName(CeGuiString name)
    {
        mName = name;
    }

    const CeGuiString GameObject::getDescription() const
    {
        return mDescription;
    }

    void GameObject::setDescription(CeGuiString description)
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
		LOG_MESSAGE(Logger::RULES,
			"Bei GameObject #"+Ogre::StringConverter::toString(mId)+
			" ("+getName()+") wurde Aktion "+action->getName().c_str()+" hinzugefügt.");
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
			LOG_MESSAGE(Logger::RULES, "Untersuche Aktion "+(*it).first->getName());
			if ((*it).second == Action::ACT_DISABLED)
				continue;
			//if ((*it).second > ACT_NEEDS_TALENT)
			if (actor != NULL && !(*it).first->canDo(const_cast<GameObject*>(this), actor)) // Aktion nicht möglich
				continue;
				
			actions.push_back((*it).first);
		}
        return actions;
    }

    void GameObject::doAction( const CeGuiString actionName,
                              Creature* actor,
                              GameObject* target)
    {
		ActionOptionVector::const_iterator it = 
            findAction(mActions.begin(), mActions.end(), actionName);

		if (it == mActions.end())
        {
            LOG_ERROR(
                Logger::RULES, 
                "'" + actionName + "' ist eine dem Objekt unbekannte Aktion.");
        }
        else
        {
		    doAction((*it).first, actor, target);
        }
    }

	void GameObject::doAction(const CeGuiString actionName)
	{
		doAction(actionName, NULL, NULL);
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
			const CeGuiString actionName)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; ++iter)
		{
			Action* action = (*iter).first;

			if (action->getName().compare(actionName) == 0)
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
		if (NULL == actor)
			Throw(IllegalArgumentException, "Actor must not be NULL");

		mActor = actor;
		mActor->setGameObject(this);
	}
	
	Actor* GameObject::getActor()
	{
		return mActor;
	}


    bool GameObject::isHighlightingEnabled()
    {
        return mHighlightingEnabled;
    }
    
    void GameObject::setHighlightingEnabled( bool highlightenabled )
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
			LOG_MESSAGE(Logger::RULES, str);
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

	Action* GameObject::getDefaultAction(Creature* actor) const
	{
		return ActionManager::getSingleton().getAction(DEFAULT_VIEW_OBJECT_ACTION);
	}
}
