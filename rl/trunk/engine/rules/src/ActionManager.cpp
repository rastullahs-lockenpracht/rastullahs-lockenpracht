#include "ActionManager.h"
#include "Action.h"
#include "Exception.h"

template <>
rl::ActionManager* Singleton<rl::ActionManager> ::ms_Singleton = 0;
namespace rl
{
    ActionManager& ActionManager::getSingleton(void)
    {
    return Singleton<ActionManager>::getSingleton();
    }

    ActionManager* ActionManager::getSingletonPtr(void)
    {
        return Singleton<ActionManager>::getSingletonPtr();
    }

    ActionManager::ActionManager()
    {
    }

    ActionManager::~ActionManager()
    {
    }

    void ActionManager::registerAction(Action* action)
    {
        if (action == NULL)
        {
            Throw(NullPointerException, "Parameter action ist NULL.");
        }

        mActionMap[action->getName()] = action;
    }

    Action* ActionManager::getDefaultAction(const CeGuiString& actionName) const
    {
        ActionMap::const_iterator it = mActionMap.find(actionName);
        if (it == mActionMap.end())
        {
            Throw(InvalidArgumentException, "Unbekannte Aktion.");
        }
        return it->second;
    }
}
