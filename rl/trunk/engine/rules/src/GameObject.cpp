#include "GameObject.h"
#include "Action.h"
#include "Exception.h"

using namespace std;

namespace rl
{
    GameObject::GameObject(int id,
                           const CeGuiString& name,
                           const CeGuiString& description) : mId(id),
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

    const CeGuiString& GameObject::getName() const
    {
        return mName;
    }

	/*const char* GameObject::getName() const
	{
		return mName.c_str();
	}*/

    void GameObject::setName(CeGuiString& name)
    {
        mName = name;
    }

	/*void GameObject::setName(const char* name)
    {
		mName = CeGuiString(name);
    }*/

    const CeGuiString& GameObject::getDescription() const
    {
        return mDescription;
    }

    void GameObject::setDescription(CeGuiString& description)
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

    void GameObject::removeAction(const CeGuiString& name)
    {
        ActionMap::iterator it = mActionMap.find(name);
        if (it != mActionMap.end())
        {
            mActionMap.erase(it);
        }
    }

    StringVector GameObject::getValidActions() const
    {
        StringVector rval(mActionMap.size());
        ActionMap::const_iterator it = mActionMap.begin();
        for (unsigned int i = 0; i < mActionMap.size(); i++)
        {
            rval[i] = (it++)->first;
        }
        return rval;
    }

    void GameObject::doAction(const CeGuiString& action,
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
