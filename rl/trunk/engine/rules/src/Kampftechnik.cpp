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

#include "Kampftechnik.h"
#include "DsaManager.h"

namespace rl
{
    Kampftechnik::Kampftechnik(const CeGuiString& name,
                   const CeGuiString& description,
                   int ebe)
                                       : mName(name),
                                         mDescription(description),
                                         mEbe(ebe)
    {
    }

    bool Kampftechnik::operator==(const Kampftechnik& rhs) const
    {
        return mName == rhs.mName;
    }

    bool Kampftechnik::operator<(const Kampftechnik& rhs) const
    {
        return mName < rhs.mName;
    }

    CeGuiString Kampftechnik::getName() const
    {
        return mName;
    }

    CeGuiString Kampftechnik::getDescription() const
    {
        return mDescription;
    }

	int Kampftechnik::getEbe() const
	{
		return mEbe;
	}

	int Kampftechnik::calculateEbe(int be) const
	{
		if (mEbe == EBE_KEINE_BE)
			return 0;
		if (mEbe == EBE_BEx2)
			return be*2;

		return std::max(be + mEbe, 0);
	}

}
