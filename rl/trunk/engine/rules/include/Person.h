#ifndef __Person_h__
#define __Person_h__

#include "RulesPrerequisites.h"

#include "Creature.h"

namespace rl {

	class _RlRulesExport Person : public Creature
	{
	public:
		///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Person(int id,
                 const std::string& name,
                 const std::string& description);
		Person(int id,
                 const std::string& name,
                 const std::string& description,
				 const Creature::TalentMap& talents);
				
	};	
	
}


#endif
