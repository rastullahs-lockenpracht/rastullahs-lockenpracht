#include "ActionManager.h"
#include "Action.h"
#include "Exception.h"

template <>
rl::ActionManager* Singleton<rl::ActionManager> ::ms_Singleton = 0;
namespace rl
{

	ActionVector::iterator findAction(ActionVector::iterator& begin, ActionVector::iterator& end, const CeGuiString& actionName)
	{
		for (ActionVector::iterator it = begin; it != end; it++)
			if ((*it)->getName().compare(actionName) == 0)
				return it;

		return end;
	}

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

        mActions.push_back(action);
    }

    Action* ActionManager::getDefaultAction(const CeGuiString& actionName) const
    {
		Action* action = NULL;
		for (ActionVector::const_iterator it = mActions.begin(); it != mActions.end(); it++)
			if ((*it)->getName().compare(actionName) == 0)
			{
				action = (*it);
				break;
			}

        if (action == NULL)
        {
            Throw(InvalidArgumentException, "Unbekannte Aktion.");
        }
        return *it;
    }
}
