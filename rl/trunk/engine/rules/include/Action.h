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

#ifndef __RL_ACTION_H__
#define __RL_ACTION_H__

#include "RulesPrerequisites.h"
#include <set>

namespace rl
{
    class _RlRulesExport Creature;
	class _RlRulesExport ActionGroup;

    /**
    * @brief Abstrakte Basisklasse für Aktionen an Spielobjekten.
    * Spielobjekte (GameObject) besitzen einen Satz von Aktionen, die man auf
    * ihnen anwenden kann. Diese werden von dieser Klasse gekapselt.
    * Konkrete Aktionen erben von dieser Klasse und muessen
    * doAction() ueberschreiben. Diese Klasse wird in Ruby ueberschrieben.
	*
	* Identifikation von Aktionen geschieht über Name und Klassenname
    */
    class _RlRulesExport Action
    {
    private:
        CeGuiString mName;
        CeGuiString mDescription;
		ActionGroup* mGroup;
	
    public:

		static const int ACT_NORMAL = 1<<0;
		static const int ACT_INVIS_DESC = 1<<1;
		static const int ACT_DISABLED = 1<<2;
		static const int ACT_NEEDS_TALENT = 1<<8;

		static const int TC_NO_TARGET = 0;
		static const int TC_GAMEOBJECT = 1;
		static const int TC_CREATURE = 2;
		static const int TC_ITEM = 3;
		static const int TC_POINT = 4;
		static const int TC_RADIUS = 5;
		static const int TC_POINT_AND_RADIUS = 6;

        /**
        * @param name Name, mit der die Aktion dem Benutzer
        *             gegenüber dargestellt wird.
        *             Die gleiche, die intern verwendet wird.
        * @param descritpion Eine naehere Beschreibung.
        */
        Action(const CeGuiString name, const CeGuiString description);
        virtual ~Action();

        const CeGuiString getName() const;
        const CeGuiString getDescription() const;

		/**
		 * Gibt den Klassennamen der (Ruby-)Klasse zurück, muss in Ruby überschrieben werden
		 *
		 * @return Name der Ruby-Klasse
		 */
		//virtual const CeGuiString getClassName() const;

        /**
         * @return Die Art des auszuwaehlenden Zieles für die Aktion.
         * @todo rval sollte eigentlich ein enum sein.
         *       SWIG mag das so aber lieber.
         */
        virtual int getTargetClass() const;
        
        /**
         * Prueft, ob die Aktion vom actor mit dem object auch
         * durchgefuehrt werden kann.
         * @param object Das benutzte Object, für diese Aktion.
         * @param actor Der Benutzer des Objektes.
         *
         * @return true, falls die Aktion möglich ist, false sonst.
         */
        virtual bool canDo(GameObject* object, Creature* actor) const;

        /**
         * Die Aktion wird ausgeführt. Diese Methode wird in Ruby
         * überschrieben. Ableitende Klassen sollten dabei nicht neue
         * öffentliche Methoden einführen.
         * @param object Das benutzte Object, für diese Aktion.
         *        Das kann auch ein Zauberspruch, etc sein.
         * @param actor Der Benutzer des Objektes.
         * @param target Das (erste) Ziele der Aktion.
         *
         */
        virtual void doAction(GameObject* object,
                              Creature* actor,
                              GameObject* target = NULL);
							  
		void setGroup(ActionGroup* group);
		ActionGroup* getGroup() const;
    };
	
	class _RlRulesExport ActionGroup
	{
	public:
		ActionGroup(CeGuiString name, ActionGroup* parent = NULL);
		~ActionGroup();
	
		const CeGuiString getName() const;
		const ActionGroup* const getParent() const;
	
		typedef std::set<ActionGroup*> ChildrenList;
	
	private:
		void addChild(ActionGroup* child);
		void removeChild(ActionGroup* child);
		void removeParent();	
	
		ActionGroup* mParent;
		ChildrenList mChildren;
		CeGuiString mName;
	};
}

#endif
