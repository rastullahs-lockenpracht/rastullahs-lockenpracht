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
#include "GameObject.h"

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "Exception.h"
#include "ObjectStateChangeEventSource.h"
#include "Property.h"
#include "RulesSubsystem.h"

using namespace std;

namespace rl
{
	const CeGuiString GameObject::DEFAULT_VIEW_OBJECT_ACTION = "viewobject";

    const Ogre::String GameObject::CLASS_NAME = "GameObject";

    const Ogre::String GameObject::POSITION = "vec_position"; 
    const Ogre::String GameObject::ORIENTATION = "quat_orientation"; 
    const Ogre::String GameObject::NAME = "str_name"; 
    const Ogre::String GameObject::DESCRIPTION = "str_description"; 
    const Ogre::String GameObject::MESHFILE = "str_meshfile"; 
    const Ogre::String GameObject::GEOMETRY_TYPE = "int_geometrytype"; 
    const Ogre::String GameObject::MASS = "real_mass"; 

    GameObject::GameObject(unsigned int id)
		:   mId(id),
            mName(""),
            mDescription(""),
            mQueryFlags(0),
			mHighlightingEnabled(true),
			mActor(NULL),
            mActions(),
            mPosition(Ogre::Vector3::ZERO),
            mOrientation(Ogre::Quaternion::IDENTITY),
            mMass(0),
            mGeometryType(PhysicsManager::GT_NONE)
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
		Throw(OperationNotSupportedException, "clone not yet implemented.");
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

    const CeGuiString GameObject::getMeshfile() const
    {
        return mMeshfile;
    }

    void GameObject::setMeshfile(CeGuiString meshfile)
    {
        mMeshfile = meshfile;
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
	
    void GameObject::doDefaultAction(Creature* actor, GameObject* target)
    {
        Action* action = getDefaultAction(actor);
        if (action != NULL)
        {
            doAction(action, actor, target);
        }
        else
        {
            LOG_ERROR(
                Logger::RULES, 
                "GameObject " + getName() + " has no valid default action set.");
        }
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
        if (mActor != NULL)
        {
            mActor->setGameObject(NULL);
        }

        if (actor != NULL)
        {
            actor->setPosition(mPosition);
            actor->setOrientation(mOrientation);
		    actor->setGameObject(this);
        }
		
        mActor = actor;
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

	Action* GameObject::getDefaultAction(Creature* actor) const
	{
		return ActionManager::getSingleton().getAction(DEFAULT_VIEW_OBJECT_ACTION);
	}

    void GameObject::setHighlighted(bool highlight)
    {
        if (mHighlightingEnabled && mActor != NULL)
        {
            mActor->setHighlighted(highlight);
        }
    }

    bool GameObject::isHighlighted() const
    {
        return mActor != NULL && mActor->isHighlighted();
    }

    void GameObject::setPosition(const Ogre::Vector3& position)
    {
        if (mActor != NULL)
        {
            mActor->setPosition(position);
        }
        else
        {
            mPosition = position;
        }
    }

    const Ogre::Vector3& GameObject::getPosition() const
    {
        if (mActor != NULL)
        {
            return mActor->getPosition();
        }
        return mPosition;
    }

    void GameObject::setOrientation(const Ogre::Quaternion& orientation)
    {
        if (mActor != NULL)
        {
            mActor->setOrientation(orientation);
        }
        else
        {
            mOrientation = orientation;
        }
    }

    const Ogre::Quaternion& GameObject::getOrientation() const
    {
        if (mActor != NULL)
        {
            return mActor->getOrientation();
        }
        return mOrientation;
    }

    const PhysicsManager::GeometryType GameObject::getGeometryType() const
    {
        return mGeometryType;
    }

    void GameObject::setGeometryType(PhysicsManager::GeometryType type)
    {
        mGeometryType = type;
    }

    const Ogre::Real GameObject::getMass() const
    {
        return mMass;
    }

    void GameObject::setMass(const Ogre::Real mass)
    {
        mMass = mass;
    }

    const Property GameObject::getProperty(const Ogre::String& key) const
    {
        Property prop;
        if (key == POSITION)
        {
            prop.setValue(getPosition());
        }
        else if (key == ORIENTATION)
        {
            prop.setValue(getOrientation());
        }
        else if (key == NAME)
        {
            prop.setValue(mName);
        }
        else if (key == DESCRIPTION)
        {
            prop.setValue(mDescription);
        }
        else if (key == MESHFILE)
        {
            prop.setValue(mMeshfile);
        }
        else if (key == GEOMETRY_TYPE)
        {
            prop.setValue(mGeometryType);
        }
        else if (key == MASS)
        {
            prop.setValue(mMass);
        }
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of this gameobject ("+mName.c_str()+")");
        }

        return prop;
    }

    void GameObject::setProperty(const Ogre::String& key, const Property& value)
    {
        try 
        {
            if (key == POSITION)
            {
                setPosition(value.toVector());
            }
            else if (key == ORIENTATION)
            {
                setOrientation(value.toQuaternion());
            }
            else if (key == NAME)
            {
                setName(value.toString());
            }
            else if (key == DESCRIPTION)
            {
                setDescription(value.toString());
            }
            else if (key == MESHFILE)
            {
                setMeshfile(value.toString());
            }
            else if (key == GEOMETRY_TYPE)
            {
                setGeometryType(static_cast<PhysicsManager::GeometryType>(value.toInt()));
            }
            else if (key == MASS)
            {
                Ogre::Real mass;
                if (value.isInt())
                {
                    mass = value.toInt();
                }
                else if (value.isReal())
                {
                    mass = value.toReal();
                }
                setMass(mass);
            }
            else
            {
                LOG_WARNING(
                    Logger::RULES, 
                    key + " is not a property of this GameObject ("+mName+")");
            }
        }
        catch (WrongFormatException ex)
        {
            LOG_ERROR(
                Logger::RULES,
                "property " + key + " has the wrong format");
        }
    }

    PropertySet* GameObject::getAllProperties() const
    {
        PropertySet* ps = new PropertySet();
        ps->setProperty(NAME, Property(mName));
        ps->setProperty(DESCRIPTION, Property(mDescription));
        ps->setProperty(POSITION, Property(getPosition()));
        ps->setProperty(ORIENTATION, Property(getOrientation()));
        ps->setProperty(MESHFILE, Property(mMeshfile));
        ps->setProperty(GEOMETRY_TYPE, Property(mGeometryType));
        ps->setProperty(MASS, Property(mMass));

        return ps;
    }

    void GameObject::placeIntoScene()
    {
        Ogre::String actorName = Ogre::StringConverter::toString(mId);

        Actor* actor = ActorManager::getSingleton().createMeshActor(
                actorName,
                mMeshfile.c_str(),
                mGeometryType,
                mMass);

        actor->placeIntoScene();
        setActor(actor);
    }

    void GameObject::removeFromScene()
    {
        Actor* actor = mActor;
        setActor(NULL);
        actor->removeFromScene();
        ActorManager::getSingleton().destroyActor(actor);
    }

    void GameObject::setState(GameObjectState state)
    {
        mState = state;
    }

    GameObjectState GameObject::getState() const
    {
        return mState;
    }
}
