/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __KAMPFTECHNIK_H__
#define __KAMPFTECHNIK_H__

#include "RulesPrerequisites.h"

#include "Tripel.h"

namespace rl
{
	/// Class encapsules DSA Kampftechniken as Raufen, Dolche, Hiebwaffen, etc.
    class _RlRulesExport Kampftechnik
    {
    private:
        const CeGuiString mName;
        const CeGuiString mDescription;
        const int mEbe;
    public:
        Kampftechnik(const CeGuiString name,
                     const CeGuiString description,
                     int ebe);

        bool operator==(const Kampftechnik& rhs) const;
        bool operator<(const Kampftechnik& rhs) const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
		int getEbe() const;
        /// Berechnet effektive Behinderung
        int calculateEbe(int be) const;
    };
}
#endif //__KAMPFTECHNIK_H__
