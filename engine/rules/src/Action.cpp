#include "Action.h"
#include "Exception.h"

namespace rl
{
    Action::Action(const std::string& name, const std::string& description) : mName(name),
                                                                              mDescription(description)
    {
    }

    Action::~Action()
    {
    }

    std::string Action::getName()
    {
        return mName;
    }

    std::string Action::getDescription()
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
