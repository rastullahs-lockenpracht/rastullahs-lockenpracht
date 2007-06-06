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

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "RulesPrerequisites.h"

#include "Action.h"
#include "GameObjectConstants.h"
#include "ObjectStateChangeEventSource.h"
#include "Properties.h"
#include "CoreDefines.h"

namespace rl
{
    class Actor;
    class Creature;
    class Effect;
    class EffectManager;

    /**
    * \brief Basisklasse aller spielrelevanten Objekte in RL.
    * Stellt im Wesentlichen Methoden zur Identifikation von
    * Objekten innerhalb der Spielwelt bereit.
    * Hierbei handelt es sich um konkrete Objekte.
    * Abstrakte Konzepte, wie Date und Action erben nicht von dieser Klasse.
    *
    * @todo Ueberlegen, wie man Aktionen situativ aktivierbar macht.
    */
    class _RlRulesExport GameObject 
        : public Ogre::UserDefinedObject, 
          public ObjectStateChangeEventSource,
          public PropertyHolder
    {
    public:
        typedef std::vector<std::pair<Action*, int> > ActionOptionVector;
        static const unsigned int NO_OBJECT_ID = 0;
        
        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_CLASS_ID;
        static const Ogre::String PROPERTY_OBJECT_ID;
        static const Ogre::String PROPERTY_BASE_CLASS;
        static const Ogre::String PROPERTY_POSITION; 
        static const Ogre::String PROPERTY_ORIENTATION; 
        static const Ogre::String PROPERTY_NAME;
        static const Ogre::String PROPERTY_DESCRIPTION; 
        static const Ogre::String PROPERTY_MESHFILE; 
  		static const Ogre::String PROPERTY_MESHPARTS; 
		static const Ogre::String PROPERTY_GEOMETRY_TYPE; 
        static const Ogre::String PROPERTY_MASS; 
        static const Ogre::String PROPERTY_ACTIONS; 
        static const Ogre::String PROPERTY_DEFAULT_ACTION;

        static const CeGuiString DEFAULT_VIEW_OBJECT_ACTION;
        static const CeGuiString DEFAULT_VIEW_OBJECT_ACTION_DEBUG;

        GameObject(unsigned int id);
        virtual ~GameObject();

        int getId() const;

        const Ogre::String getClassId() const;
        void setClassId(Ogre::String classId);
        
        const CeGuiString getName() const;
        void setName(CeGuiString name);

        const CeGuiString getDescription() const;
        void setDescription(CeGuiString description);

        const CeGuiString getMeshfile() const;
        void setMeshfile(CeGuiString meshfile);

		const MeshPartMap& getMeshParts() const;

        const GeometryType getGeometryType() const;
        void setGeometryType(GeometryType type);

        const Ogre::Real getMass() const;
        void setMass(const Ogre::Real mass);

        void addAction(Action* action, int option = Action::ACT_NORMAL);
        void addActionInGroup(Action* action, ActionGroup* group, int option = Action::ACT_NORMAL);
        void removeAction(Action* action);
    
        void setActor(Actor* actor);
        Actor* getActor();

        /**
         * Get all valid actions a character can perfom on this game object
         * 
         * @param actor the character
         * @return a vector of actions
         */
        const ActionVector getValidActions(Creature* actor) const;
        virtual Action* getDefaultAction(Creature* actor) const;

        /** Trigger an action of this game object
         *  @param actionName the action's name
         *  @param actor the "user" of this game object, can be <code>NULL</code> sein, 
                   if the action wasn't triggered by someone (e.g. by time)
         *  @param target the action's target (can be <code>NULL</code> if no other game objects are involved)
         */
        void doAction(const CeGuiString actionName,
                      Creature* actor,
                      GameObject* target);

        void doAction(const CeGuiString actionName);

        void doAction(Action* action,
                      Creature* actor,
                      GameObject* target);
                      
        bool activateAction(Action* action,
                      Creature* actor,
                      GameObject* target);

        void doDefaultAction(Creature* actor, GameObject* target);

        void setPosition(const Ogre::Vector3& position);
        void setOrientation(const Ogre::Quaternion& orientation);
        const Ogre::Quaternion& getOrientation() const;
        const Ogre::Vector3& getPosition() const;

        /// Soll der Aktor überhaupt leuchten?
        bool isHighlightingEnabled();
        void setHighlightingEnabled( bool highlightenabled );

        void setHighlighted(bool highlight);
        bool isHighlighted() const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;

        GameObjectState getState() const;
        virtual void setState(GameObjectState state);
        void placeIntoScene();
        void removeFromScene();

        unsigned long getQueryFlags() const;
        void addQueryFlag(unsigned long queryflag);

        virtual void onStateChange(GameObjectState oldState, GameObjectState newState);

        /**
         * Laesst einen Effekt auf der Kreatur wirken.
         * @param effect Zeiger auf den Effekt.
         * @ingroup CreatureRubyExports
         **/
        void addEffect(Effect* effect);
        void addEffectWithCheckTime(Effect* effect, RL_LONGLONG time);
        void addEffectWithCheckDate(Effect* effect, RL_LONGLONG date);
        void removeEffect(Effect* effect);

    protected:
        int mId;
        GameObjectState mState;
        
        CeGuiString mName;
        CeGuiString mDescription;
        CeGuiString mMeshfile;
		MeshPartMap mMeshParts;
        Ogre::String mClassId;
        
        Actor* mActor;
        
        // Query flags to be set to the actor, when placed into scene.
        unsigned long mQueryFlags;

        /// Soll das GameObject überhaupt leuchten?
        bool mHighlightingEnabled;

        /// Verwaltet die Effekte die auf die Kreatur wirken.
        EffectManager* mEffectManager;

        Actor* createActor();
        void destroyActor();

        void doPlaceIntoScene();
        void doRemoveFromScene();

        /**
         * Ueberprueft die wirkenden Effekte auf Lebendigkeit
         **/
        void checkEffects();

    private:
        static int sNextGameObjectId;    

        ActionOptionVector mActions;
        Ogre::Vector3 mPosition;
        Ogre::Quaternion mOrientation;
        Ogre::Real mMass;
        CeGuiString mDefaultAction;
        GeometryType mGeometryType;

        ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin,
            ActionOptionVector::iterator end, const CeGuiString actionName);
        ActionOptionVector::iterator findAction(ActionOptionVector::iterator
            begin, ActionOptionVector::iterator end, const Action* action);    
    };
}

#endif
