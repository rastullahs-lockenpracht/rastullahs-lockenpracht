#include "Person.h"

namespace rl {

	Person::Person(int id,
                 const std::string& name,
                 const std::string& description)
    	: Creature(id, name, description)
    {
    	
    }
	
	Person::Person(int id,
                 const std::string& name,
                 const std::string& description,
				 const Creature::TalentMap& talents)
    	: Creature(id, name, description, talents)
    {
    	
    }
		
	
	
}
