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

	const CeGuiString& Action::getClassName() const
	{
		static CeGuiString className = (CEGUI::utf8*)"Action";
		return className;
	}

    const CeGuiString& Action::getDescription() const
    {
        return mDescription;
    }

    int Action::getTargetClass() const
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
