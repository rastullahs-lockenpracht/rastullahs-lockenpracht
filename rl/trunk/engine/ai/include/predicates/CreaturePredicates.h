/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#ifndef __Rl_CreaturePredicates_H__
#define __Rl_CreaturePredicates_H__

#include "AiPrerequisites.h"
#include "Predicates.h"
using namespace MadaBot;

namespace rl
{
	class Creature;

	class CreaturePredicates :
		public Predicates<CeGuiString>
	{
	public:
		CreaturePredicates(Creature* pCharacter);
		virtual ~CreaturePredicates(void);
		
		virtual CeGuiString getPredicate(const CeGuiString& pName) const=0;

		virtual void setPredicate(const CeGuiString& pName, const CeGuiString& pValue);

		virtual CeGuiString getType() const=0;

	protected:
		Creature* mCharacter;
		int mModifier;
	};
}
#endif
