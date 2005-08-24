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

#ifndef __ACTIONMANAGER_H__
#define __ACTIONMANAGER_H__

#include "RulesPrerequisites.h"

#include <OgreSingleton.h>
#include <vector>
using Ogre::Singleton;

namespace rl
{
	class Action;
	class ActionGroup;
	
	typedef std::vector<Action*> ActionVector;
        
    /**
    * \brief Abstrakte Basisklasse fuer Aktionen an Spielobjekten.
    * Spielobjekte (GameObject) besitzen einen Satz von Aktionen, die man auf
    * ihnen anwenden kann. Diese werden von dieser Klasse gekapselt.
    * Konkrete Aktionen erben von dieser Klasse und muessen
    * doUserAction() ueberschreiben.
    */
    class _RlRulesExport ActionManager : 
    	public Singleton<ActionManager>
    {
    private:
        ActionVector mActions;
		ActionVector mInGameGlobalActions;
        
    public:
        ActionManager();
        ~ActionManager();

        void registerAction(Action* action);
        void unregisterAction(const CeGuiString& actionName, const CeGuiString& className);
        Action* getDefaultAction(const CeGuiString& actionName) const;
		Action* getAction(const CeGuiString& actionName, const CeGuiString& className) const;

		void registerInGameGlobalAction(Action* action, ActionGroup* group);
		Action* getInGameGlobalAction(const CeGuiString& actionName, const CeGuiString& className) const;
		const ActionVector& ActionManager::getInGameGlobalActions();
	
        static ActionManager& getSingleton(void);
        static ActionManager* getSingletonPtr(void);
    };
}

#endif
