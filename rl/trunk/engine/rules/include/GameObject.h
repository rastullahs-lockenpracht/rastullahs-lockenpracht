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

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "RulesPrerequisites.h"

#include "Actor.h"
#include "Action.h"
#include "ObjectStateChangeEventSource.h"

namespace rl
{
    class _RlRulesExport Creature;

    /**
    * \brief Basisklasse aller spielrelevanten Objekte in RL.
    * Stellt im Wesentlichen Methoden zur Identifikation von
    * Objekten innerhalb der Spielwelt bereit.
    * Hierbei handelt es sich um konkrete Objekte.
    * Abstrakte Konzepte, wie Date und Action erben nicht von dieser Klasse.
    *
    * @todo Ueberlegen, wie man Aktionen situativ aktivierbar macht.
    */
    class _RlRulesExport GameObject : public Ogre::UserDefinedObject, public ObjectStateChangeEventSource
    {
    public:
        GameObject(const CeGuiString& name,
                   const CeGuiString& description);
        virtual ~GameObject(void);

        virtual int getId() const;

        virtual const CeGuiString& getName() const;
        virtual void setName(CeGuiString& name);

        virtual const CeGuiString& getDescription() const;
        virtual void setDescription(CeGuiString& description);

		void addAction(Action* action, int option = Action::ACT_NORMAL);
        void addActionInGroup(Action* action, ActionGroup* group, int option = Action::ACT_NORMAL);
        void removeAction(Action* action);
	
		void setActor(Actor* actor);
		Actor* getActor();

        const ActionVector getValidActions(Creature* actor) const;
		virtual Action* getDefaultAction(Creature* actor) const;

        /** Eine Aktion des Gegenstandes wird ausgeloest.
         *  @param actionName der Name der auszuloesenden Aktion.
         *  @param actionClass der (Ruby-)Klassenname der auszuloesenden Aktion.
         *  @param actor der "Benutzer" des Gegenstandes,
         *         der die Aktion ausgeloest hat, kann auch
         *         <code>NULL</code> sein, falls die Aktion auf andere Weise
         *         getriggert wurde.
         *  @param target Ziel, auf das die Aktion gewirkt wird.
         */
        void doAction(const CeGuiString& actionName,
                      Creature* actor,
                      GameObject* target);

		void doAction(const CeGuiString& actionName);

		void doAction(Action* action,
                      Creature* actor,
                      GameObject* target);
					  
		bool activateAction(Action* action,
                      Creature* actor,
                      GameObject* target);

		typedef std::vector<std::pair<Action*, int> > ActionOptionVector;

        /// Soll der Aktor überhaupt leuchten?
        bool isHigligthingEnabled();
        void setHigligthingEnabled( bool highlightenabled );

		void setString(CeGuiString key, CeGuiString value);
		void setBool(CeGuiString key, bool value);
		void setInt(CeGuiString key, int value);
		void setReal(CeGuiString key, Ogre::Real real);

		CeGuiString getString(CeGuiString key);
		bool getBool(CeGuiString key);
		int getInt(CeGuiString key);
		Ogre::Real getReal(CeGuiString key);

		static const CeGuiString DEFAULT_VIEW_OBJECT_ACTION;

	private:
        ActionOptionVector mActions;
		ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin, ActionOptionVector::iterator end, const CeGuiString& actionName);
		ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin, ActionOptionVector::iterator end, const Action* action);
	
		Actor* mActor;

		std::map<CeGuiString, CeGuiString>	mAttributesString;
		std::map<CeGuiString, bool>		mAttributesBoolean;
		std::map<CeGuiString, int>			mAttributesInteger;
		std::map<CeGuiString, Ogre::Real>	mAttributesReal;

		static int sNextGameObjectId;
	
    protected:
        int mId;
        CeGuiString mName;
        CeGuiString mDescription;


        /// Soll das GameObject überhaupt leuchten?
        bool mHighlightingEnabled;
    };
}

#endif
