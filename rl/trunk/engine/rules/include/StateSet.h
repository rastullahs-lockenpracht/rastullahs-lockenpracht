/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __STATESET_H__
#define __STATESET_H__

#include "RulesPrerequisites.h"

namespace rl
{

	class _RlRulesExport StateSet
	{
	public:
		StateSet();
		virtual ~StateSet();
		virtual int getValue(bool getUnmodfiedValue = false) const;
		virtual int getOriginalValue() const;
		virtual void setOriginalValue(int newValue);
		virtual int getModifier() const;
		virtual void setModifier(int newModifier);
		virtual int getMultiplier() const;
		virtual void setMultiplier(int newMultiplier);

	protected:
		/// Der unmodifizierte Wert
		int mOriginalValue;
		int mModifier;
		/// Dieser Faktor wird nach dem modifizieren auf den Wert draufmultipliziert.
		int mMultiplier;
	};
}
#endif //__STATESET_H__
