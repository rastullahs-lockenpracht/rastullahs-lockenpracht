#include "Action.h"
#include "Exception.h"

namespace rl
{
    Action::Action(const CeGuiString& name, const CeGuiString& description) : mName(name),
                                                                              mDescription(description)
    {
    }

    Action::~Action()
    {
    }

    const CeGuiString& Action::getName() const
    {
        return mName;
    }

    const CeGuiString& Action::getDescription() const
    {
        return mDescription;
    }

    int Action::getTargetClass()
    {
        return TC_NO_TARGET;
    }

    void Action::doAction(GameObject* object,
                          Creature* actor,
                          GameObject* target)
    {
        //Beep(4000, 4000);
        //Throw(IllegalStateException, "Methode muss ueberschrieben werden.");
    }
};
