#ifndef __ACTIONMANAGER_H__
#define __ACTIONMANAGER_H__

#include "RulesPrerequisites.h"

#include <OgreSingleton.h>
#include <CEGUIString.h>
#include <map>
using Ogre::Singleton;

namespace rl
{
	class Action;
	
	typedef std::map<CeGuiString, Action*> ActionMap;
        
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
        ActionMap mActionMap;
        
    public:
        ActionManager();
        ~ActionManager();

        void registerAction(Action* action);
        void unregisterAction(const CeGuiString& actionName);

        Action* getDefaultAction(const CeGuiString& actionName) const;

        static ActionManager& getSingleton(void);
        static ActionManager* getSingletonPtr(void);
    };
}

#endif
