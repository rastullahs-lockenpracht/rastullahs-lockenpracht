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
                 const CeGuiString& name,
                 const CeGuiString& description);
	};	
}


#endif
