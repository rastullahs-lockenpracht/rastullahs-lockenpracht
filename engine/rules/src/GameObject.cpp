#include "GameObject.h"
#include "Action.h"
#include "Exception.h"

using namespace std;

namespace rl
{
    GameObject::GameObject(int id,
                           const std::string& name,
                           const std::string& description) : mId(id),
                                                             mName(name),
                                                             mDescription(description)
    {
        // Standardactions registrieren
    }

    GameObject::~GameObject(void)
    {
    }

    int GameObject::getId() const
    {
        return mId;
    }

    std::string GameObject::getName() const
    {
        return mName;
    }

    void GameObject::setName(const std::string& name)
    {
        mName = name;
    }

    std::string GameObject::getDescription() const
    {
        return mDescription;
    }

    void GameObject::setDescription(const std::string& description)
    {
        mDescription = description;
    }

    void GameObject::addAction(Action* action)
    {
        if (action == NULL)
        {
            Throw(NullPointerException, "Parameter action ist NULL.");
        }

        mActionMap[action->getName()] = action;
    }

    void GameObject::removeAction(const std::string& name)
    {
        ActionMap::iterator it = mActionMap.find(name);
        if (it != mActionMap.end())
        {
            mActionMap.erase(it);
        }
    }

    StringVector GameObject::getValidActions() const
    {
        vector<std::string> rval(mActionMap.size());
        ActionMap::const_iterator it = mActionMap.begin();
        for (unsigned int i = 0; i < mActionMap.size(); i++)
        {
            rval[i] = (it++)->first;
        }
        return rval;
    }

    void GameObject::doAction(const std::string& action,
                              Creature* actor,
                              GameObject* target)
    {
        ActionMap::iterator it = mActionMap.find(action);
        if (it == mActionMap.end())
        {
            Throw(InvalidArgumentException, "Unbekannte Aktion.");
        }
        it->second->doAction(this, actor, target);
    }
}
