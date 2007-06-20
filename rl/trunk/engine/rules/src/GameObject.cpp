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
#include "GameObject.h"

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "EffectManager.h"
#include "Exception.h"
#include "ObjectStateChangeEventSource.h"
#include "Property.h"
#include "RulesSubsystem.h"
#include "GameObjectManager.h"
#include "CoreSubsystem.h"
#include "PhysicsManager.h"

using namespace std;

namespace rl
{
    const CeGuiString GameObject::DEFAULT_VIEW_OBJECT_ACTION = "viewobject";
    const CeGuiString GameObject::DEFAULT_VIEW_OBJECT_ACTION_DEBUG = "viewobjectdebug";

    const Ogre::String GameObject::CLASS_NAME = "GameObject";

    const Ogre::String GameObject::PROPERTY_CLASS_ID = "classid"; 
    const Ogre::String GameObject::PROPERTY_BASE_CLASS = "baseclass"; 
    const Ogre::String GameObject::PROPERTY_POSITION = "position"; 
    const Ogre::String GameObject::PROPERTY_ORIENTATION = "orientation"; 
    const Ogre::String GameObject::PROPERTY_NAME = "name"; 
    const Ogre::String GameObject::PROPERTY_DESCRIPTION = "description"; 
    const Ogre::String GameObject::PROPERTY_MESHFILE = "meshfile"; 
	const Ogre::String GameObject::PROPERTY_MESHPARTS = "meshparts"; 
    const Ogre::String GameObject::PROPERTY_GEOMETRY_TYPE = "geometrytype"; 
    const Ogre::String GameObject::PROPERTY_MASS = "mass"; 
    const Ogre::String GameObject::PROPERTY_ACTIONS = "actions";
    const Ogre::String GameObject::PROPERTY_DEFAULT_ACTION = "defaultaction";

    GameObject::GameObject(unsigned int id)
        :   mId(id),
            mName(""),
            mDescription(""),
            mMeshfile(""),
			mMeshParts(),
            mQueryFlags(QUERYFLAG_GAMEOBJECT),
            mHighlightingEnabled(true),
            mActor(NULL),
            mActions(),
            mPosition(Ogre::Vector3::ZERO),
            mOrientation(Ogre::Quaternion::IDENTITY),
            mMass(0),
            mGeometryType(GT_NONE),
            mDefaultAction(DEFAULT_VIEW_OBJECT_ACTION),
            mState(GOS_LOADED)
    {
        mEffectManager = new EffectManager();

        // Standardactions registrieren
        Action* defaultAction = ActionManager::getSingleton().getAction(DEFAULT_VIEW_OBJECT_ACTION);
        if (defaultAction != NULL)
        {
            addAction(defaultAction);
        }

        // If game in developer mode, show GameObject debug window
        // in the radial menu
        //if( CoreSubsystem::getSingleton().getDeveloperMode() )
        //{
            defaultAction = ActionManager::getSingleton().getAction(DEFAULT_VIEW_OBJECT_ACTION_DEBUG);
            if (defaultAction != NULL)
            {
                addAction(defaultAction);
            }
        //}

        // Eventsource erzeugen
        setObject(this);
    }

    GameObject::~GameObject(void)
    {
    	delete mEffectManager;
    }

    int GameObject::getId() const
    {
        return mId;
    }

    const Ogre::String GameObject::getClassId() const
    {
        return mClassId;
    }
    
    void GameObject::setClassId(Ogre::String classId)
    {
        mClassId = classId;
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

	const MeshPartMap& GameObject::getMeshParts() const
	{
		return mMeshParts;
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
        if (mActor != actor)
        {
            if (mActor != NULL)
            {
                mActor->setGameObject(NULL);
                mActor->setQueryFlags(0);
            }

            if (actor != NULL)
            {
                if (actor->isInScene())
                {
                    actor->setPosition(mPosition);
                    actor->setOrientation(mOrientation);
                    actor->setQueryFlags(mQueryFlags);
                }
                actor->setGameObject(this);
            }
            
            mActor = actor;
        }
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
        return ActionManager::getSingleton().getAction(mDefaultAction);
    }

    void GameObject::setHighlighted(bool highlight)
    {
        if (mHighlightingEnabled && mActor != NULL)
        {
            mActor->setHighlighted(highlight, getName());
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

    const GeometryType GameObject::getGeometryType() const
    {
        return mGeometryType;
    }

    void GameObject::setGeometryType(GeometryType type)
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

    void GameObject::addEffect(Effect* effect)
	{
		mEffectManager->addEffect(effect);
	}
    
    void GameObject::addEffectWithCheckTime(Effect* effect, RL_LONGLONG time)
    {
      addEffect(effect);
      mEffectManager->addTimeCheck(time, effect);
    }
    
    void GameObject::addEffectWithCheckDate(Effect* effect, RL_LONGLONG date)
    {
      addEffect(effect);
      mEffectManager->addDateCheck(date, effect);
    }

	void GameObject::checkEffects()
	{
		/// @todo Nur einmal pro Aktion ausfuehren
		mEffectManager->checkEffects();
	}

    void GameObject::removeEffect(Effect* effect)
	{
		mEffectManager->removeEffect(effect);
	}
    
    const Property GameObject::getProperty(const Ogre::String& key) const
    {
        Property prop;
        if (key == PROPERTY_POSITION)
        {
            prop.setValue(getPosition());
        }
        else if (key == PROPERTY_ORIENTATION)
        {
            prop.setValue(getOrientation());
        }
        else if (key == PROPERTY_NAME)
        {
            prop.setValue(mName);
        }
        else if (key == PROPERTY_DESCRIPTION)
        {
            prop.setValue(mDescription);
        }
        else if (key == PROPERTY_MESHFILE)
        {
            prop.setValue(mMeshfile);
        }
        else if (key == PROPERTY_GEOMETRY_TYPE)
        {
            prop.setValue(mGeometryType);
        }
        else if (key == PROPERTY_MASS)
        {
            prop.setValue(mMass);
        }
		else if (key == PROPERTY_MESHPARTS)
		{
			PropertyMap map;
			for (MeshPartMap::const_iterator
					it = mMeshParts.begin(); it != mMeshParts.end(); ++it)
			{
				map[(*it).first] = Property((*it).second);
			}
			prop.setValue(map);
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
            if (key == PROPERTY_POSITION)
            {
                setPosition(value.toVector3());
            }
            else if (key == PROPERTY_ORIENTATION)
            {
                setOrientation(value.toQuaternion());
            }
            else if (key == PROPERTY_NAME)
            {
                setName(value.toString());
            }
            else if (key == PROPERTY_DESCRIPTION)
            {
                setDescription(value.toString());
            }
            else if (key == PROPERTY_MESHFILE)
            {
                setMeshfile(value.toString());
            }
            else if (key == PROPERTY_MESHPARTS)
            {
				PropertyMap map = value.toMap();
				for (PropertyMap::const_iterator 
					it = map.begin(); it != map.end(); ++it)
				{
					mMeshParts[(*it).first.c_str()] = (*it).second.toString().c_str();
				}
            }
            else if (key == PROPERTY_GEOMETRY_TYPE)
            {
                setGeometryType(PhysicsManager::convertStringToGeometryType(value.toString().c_str()));            
            }
            else if (key == PROPERTY_MASS)
            {
                Ogre::Real mass = 0.0;
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
            else if (key == PROPERTY_ACTIONS)
            {
                PropertyVector vecVal = value.toArray();
                for (PropertyVector::const_iterator it = vecVal.begin(); it != vecVal.end(); it++)
                {
                    Ogre::String actionName = (*it).toString().c_str();
                    Action* act = ActionManager::getSingleton().getAction(actionName);

                    if (act != NULL)
                    {
                        addAction(act);
                    }
                    else
                    {
                        LOG_ERROR(Logger::RULES, "'"+actionName+"' not registered at ActionManager.");
                    }
                }
            }
            else if (key == PROPERTY_DEFAULT_ACTION)
            {
                mDefaultAction = value.toString().c_str();
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
        ps->setProperty(PROPERTY_NAME, Property(mName));
        ps->setProperty(PROPERTY_DESCRIPTION, Property(mDescription));
        ps->setProperty(PROPERTY_POSITION, Property(getPosition()));
        ps->setProperty(PROPERTY_ORIENTATION, Property(getOrientation()));
        ps->setProperty(PROPERTY_MESHFILE, Property(mMeshfile));
        ps->setProperty(PROPERTY_MESHPARTS, getProperty(PROPERTY_MESHPARTS));
        ps->setProperty(PROPERTY_GEOMETRY_TYPE, Property(mGeometryType));
        ps->setProperty(PROPERTY_MASS, Property(mMass));

        return ps;
    }

    Actor* GameObject::createActor()
    {
        if (mActor == NULL)
        {
            Ogre::String actorName = Ogre::StringConverter::toString(mId);
			Actor* actor = NULL;

			if (mMeshfile.empty() && mMeshParts.empty())
			{
				LOG_ERROR(
					Logger::RULES, 
					"Neither mesh file nor mesh parts are set on gameobject '" + getName()
					+ "' (id: " + getId() + "). Can't create actor!");
			}
			else if (!mMeshParts.empty())
			{
				actor = ActorManager::getSingleton().createMeshActor(
						actorName,
						mMeshfile.c_str(),
						mMeshParts,
						mGeometryType,
						mMass);
			}
			else
			{
				actor = ActorManager::getSingleton().createMeshActor(
						actorName,
						mMeshfile.c_str(),
						mGeometryType,
						mMass);
			}

			if (actor == NULL)
			{
				LOG_ERROR(
					Logger::RULES, 
					"Error creating actor '"
					+ actorName	+ "'.");
			}
            return actor;
        }
        else
        {
            return mActor;
        }
    }

    void GameObject::destroyActor()
    {
        if (mActor != NULL)
        {
            ActorManager::getSingleton().destroyActor(mActor);
            mActor = NULL;
        }
    }

    void GameObject::placeIntoScene()
    {
        setState(GOS_IN_SCENE);
    }

    void GameObject::removeFromScene()
    {
        setState(GOS_LOADED);
    }

    void GameObject::doPlaceIntoScene()
    {
        if (mState != GOS_IN_SCENE)
        {
            Actor* actor = createActor();

            if (actor != NULL)
            {
                actor->placeIntoScene();
                setActor(actor);

                GameObjectState tmpState = mState;
                mState = GOS_IN_SCENE;
                GameObjectManager::getSingleton().gameObjectStateChanged(this, tmpState, mState);
                onStateChange(tmpState, GOS_IN_SCENE);
            }
            else {
                LOG_ERROR(
                    Logger::RULES, 
                    "Error placing gameobject '"
                    + Ogre::StringConverter::toString(mId)
                    + "' into scene "
                    + mMeshfile);
            }
        }
    }

    void GameObject::doRemoveFromScene()
    {
        if (mState == GOS_IN_SCENE)
        {
            Actor* actor = mActor;
            mOrientation = actor->getWorldOrientation();
            mPosition = actor->getWorldPosition();

            setActor(NULL);
            actor->removeFromScene();
            ActorManager::getSingleton().destroyActor(actor);

            GameObjectState tmpState = mState;
            mState = GOS_LOADED;
            GameObjectManager::getSingleton().gameObjectStateChanged(this, tmpState, mState);
        }
    }

    void GameObject::setState(GameObjectState targetstate)
    {
        if (targetstate == mState)
        {
            return;
        }

        GameObjectState oldState = mState;

        if (targetstate == GOS_LOADED && mState == GOS_IN_SCENE)
        {
            // Statechange event is triggered in this function
            doRemoveFromScene();
            onStateChange(oldState, targetstate);
        }
        else if (targetstate == GOS_IN_SCENE && mState == GOS_LOADED)
        {
            //Statechange event is triggered in this function
            doPlaceIntoScene();
            onStateChange(oldState, targetstate);
        }
        else
        {
            LOG_ERROR(
                Logger::RULES, 
                "GameObject '" + getName() 
                + "' could not change state from "
                + Ogre::StringConverter::toString(mState) + " to "
                + Ogre::StringConverter::toString(targetstate));
        }
    }

    void GameObject::onStateChange(GameObjectState oldState, GameObjectState newState)
    {
    }

    GameObjectState GameObject::getState() const
    {
        return mState;
    }

    unsigned long GameObject::getQueryFlags() const
    {
        return mQueryFlags;
    }

    void GameObject::addQueryFlag(unsigned long queryflag)
    {
        mQueryFlags |= queryflag;
    }
}
