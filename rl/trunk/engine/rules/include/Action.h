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

#ifndef __RL_ACTION_H__
#define __RL_ACTION_H__

#include "RulesPrerequisites.h"
#include <CEGUIString.h>
#include <set>

namespace rl
{
    class _RlRulesExport Creature;
	class _RlRulesExport ActionGroup;

    static const int TC_NO_TARGET = 0;
    static const int TC_GAMEOBJECT = 1;
    static const int TC_CREATURE = 2;
    static const int TC_ITEM = 3;
	static const int TC_POINT = 4;
	static const int TC_RADIUS = 5;
	static const int TC_POINT_AND_RADIUS = 6;

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
        /**
        * @param name Name, mit der die Aktion dem Benutzer
        *             gegenüber dargestellt wird.
        *             Die gleiche, die intern verwendet wird.
        * @param descritpion Eine naehere Beschreibung.
        */
        Action(const CeGuiString& name, const CeGuiString& description);
        virtual ~Action();

        virtual const CeGuiString& getName() const;
        virtual const CeGuiString& getDescription() const;

		/**
		 * Gibt den Klassennamen der (Ruby-)Klasse zurück, muss in Ruby überschrieben werden
		 *
		 * @return Name der Ruby-Klasse
		 */
		virtual const CeGuiString& getClassName() const;

        /**
         * @return Die Art des auszuwaehlenden Zieles für die Aktion.
         * @todo rval sollte eigentlich ein enum sein.
         *       SWIG mag das so aber lieber.
         */
        virtual int getTargetClass() const;

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
                              GameObject* target);
							  
		void setGroup(ActionGroup* group);
		ActionGroup* getGroup() const;
    };
	
	class _RlRulesExport ActionGroup
	{
	public:
		ActionGroup(CeGuiString name, ActionGroup* parent = NULL);
		~ActionGroup();
	
		const CeGuiString& getName() const;
	
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
