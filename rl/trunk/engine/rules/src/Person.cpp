#include "Person.h"

namespace rl {

	Person::Person(int id,
                 const std::string& name,
                 const std::string& description)
    	: Creature(id, name, description)
    {
		setWert(WERT_SOZIALSTATUS, 0);
    }
	
}
