/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __Rl_DialogScriptObject_H__
#define __Rl_DialogScriptObject_H__

#include "DialogPrerequisites.h"

namespace rl
{
	class Creature;

	class _RlDialogExport DialogScriptObject
	{
	public:
		DialogScriptObject();
		virtual ~DialogScriptObject(void);

		virtual int calcOptionValue(const CeGuiString& optionName)=0;
		virtual int calcResponseValue(const CeGuiString& responseName)=0;

		DialogCharacter* getParent() const;
		void setParent(DialogCharacter* parent);

		Creature* getDialogPartner() const;
		Creature* getCharacter() const;

	private:
		DialogCharacter* mParent;

	};

	inline DialogCharacter* DialogScriptObject::getParent() const
	{
		return mParent;
	}

	inline void DialogScriptObject::setParent(DialogCharacter* parent)
	{
		mParent = parent;
	}
}

#endif
