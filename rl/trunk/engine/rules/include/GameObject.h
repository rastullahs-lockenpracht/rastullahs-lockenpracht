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
#include "GameObjectState.h"
#include "ObjectStateChangeEventSource.h"
#include "Properties.h"
#include "PhysicsManager.h"

namespace rl
{
    class Actor;
    class Creature;

    static const unsigned long QUERYFLAG_GAMEOBJECT = 1;
    static const unsigned long QUERYFLAG_CREATURE   = 2;
    static const unsigned long QUERYFLAG_ITEM       = 4;
    static const unsigned long QUERYFLAG_CONTAINER  = 8;
    static const unsigned long QUERYFLAG_WEAPON     = 16;
    static const unsigned long QUERYFLAG_ARMOR      = 32;
    static const unsigned long QUERYFLAG_PERSON     = 64;

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

        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_POSITION; 
        static const Ogre::String PROPERTY_ORIENTATION; 
        static const Ogre::String PROPERTY_NAME;
        static const Ogre::String PROPERTY_DESCRIPTION; 
        static const Ogre::String PROPERTY_MESHFILE; 
        static const Ogre::String PROPERTY_GEOMETRY_TYPE; 
        static const Ogre::String PROPERTY_MASS; 
		static const Ogre::String PROPERTY_ACTIONS; 
		static const Ogre::String PROPERTY_DEFAULT_ACTION;

		static const CeGuiString DEFAULT_VIEW_OBJECT_ACTION;
        static const CeGuiString DEFAULT_VIEW_OBJECT_ACTION_DEBUG;

        GameObject(unsigned int id);
        virtual ~GameObject();

		virtual GameObject* clone();

        int getId() const;

        const CeGuiString getName() const;
        void setName(CeGuiString name);

        const CeGuiString getDescription() const;
        void setDescription(CeGuiString description);

        const CeGuiString getMeshfile() const;
        void setMeshfile(CeGuiString meshfile);

        const PhysicsManager::GeometryType getGeometryType() const;
        void setGeometryType(PhysicsManager::GeometryType type);

        const Ogre::Real getMass() const;
        void setMass(const Ogre::Real mass);

		void addAction(Action* action, int option = Action::ACT_NORMAL);
        void addActionInGroup(Action* action, ActionGroup* group, int option = Action::ACT_NORMAL);
        void removeAction(Action* action);
	
		void setActor(Actor* actor);
		Actor* getActor();

        const ActionVector getValidActions(Creature* actor) const;
		virtual Action* getDefaultAction(Creature* actor) const;

        /** Eine Aktion des Gegenstandes wird ausgeloest.
         *  @param actionName der Name der auszuloesenden Aktion.
         *  @param actor der "Benutzer" des Gegenstandes,
         *         der die Aktion ausgeloest hat, kann auch
         *         <code>NULL</code> sein, falls die Aktion auf andere Weise
         *         getriggert wurde.
         *  @param target Ziel, auf das die Aktion gewirkt wird.
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

    protected:
        int mId;
        GameObjectState mState;
        
        CeGuiString mName;
        CeGuiString mDescription;
        CeGuiString mMeshfile;
        
        Actor* mActor;
        
        // Query flags to be set to the actor, when placed into scene.
        unsigned long mQueryFlags;

        /// Soll das GameObject überhaupt leuchten?
        bool mHighlightingEnabled;

        Actor* createActor();
        void destroyActor();

		void doPlaceIntoScene();
		void doRemoveFromScene();

	private:
		static int sNextGameObjectId;	

		ActionOptionVector mActions;
        Ogre::Vector3 mPosition;
        Ogre::Quaternion mOrientation;
        Ogre::Real mMass;
		CeGuiString mDefaultAction;
        PhysicsManager::GeometryType mGeometryType;

		ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin,
            ActionOptionVector::iterator end, const CeGuiString actionName);
		ActionOptionVector::iterator findAction(ActionOptionVector::iterator
            begin, ActionOptionVector::iterator end, const Action* action);	
    };
}

#endif
