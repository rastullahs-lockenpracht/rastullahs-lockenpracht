#include "Person.h"

namespace rl {

	Person::Person(int id,
                 const CeGuiString& name,
                 const CeGuiString& description)
    	: Creature(id, name, description)
    {
		setWert(WERT_SOZIALSTATUS, 0);
    }
	
}
