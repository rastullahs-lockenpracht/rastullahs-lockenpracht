#ifndef __ACTIONMANAGER_H__
#define __ACTIONMANAGER_H__

#include "RulesPrerequisites.h"

namespace rl
{
    /**
    * \brief Abstrakte Basisklasse für Aktionen an Spielobjekten.
    * Spielobjekte (GameObject) besitzen einen Satz von Aktionen, die man auf
    * ihnen anwenden kann. Diese werden von dieser Klasse gekapselt.
    * Konkrete Aktionen erben von dieser Klasse und muessen
    * doUserAction() ueberschreiben.
    */
    class _RlRulesExport ActionManager : public Singleton<ActionManager>
    {
    private:
        ActionMap mActionMap;
    public:
        ActionManager();
        ~ActionManager();

        void registerAction(Action* action);
        void unregisterAction(const std::string& actionName);

        Action* getDefaultAction(const std::string& actionName) const;

        static ActionManager& getSingleton(void);
        static ActionManager* getSingletonPtr(void);
    };
}

#endif
