/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#ifndef __Rl_CreaturePredicates_H__
#define __Rl_CreaturePredicates_H__

#include "DialogPrerequisites.h"
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
