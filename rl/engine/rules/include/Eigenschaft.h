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

#ifndef __EIGENSCHAFT_H__
#define __EIGENSCHAFT_H__

#include "RulesPrerequisites.h"

#include "Tripel.h"

namespace rl
{
    static const int EIGENSCHAFT_COUNT = 8;

    static const CeGuiString E_MUT = "MU";
    static const CeGuiString E_KLUGHEIT = "KL";
    static const CeGuiString E_INTUITION = "IN";
    static const CeGuiString E_CHARISMA = "CH";
    static const CeGuiString E_FINGERFERTIGKEIT = "FF";
    static const CeGuiString E_GEWANDTHEIT = "GE";
	static const CeGuiString E_KONSTITUTION = "KO";
    static const CeGuiString E_KOERPERKRAFT = "KK";

    typedef Tripel<CeGuiString> EigenschaftTripel;

    class _RlRulesExport Eigenschaft
    {
    private:
        const CeGuiString mName;
		const CeGuiString mNameAbbreviation;
        const CeGuiString mDescription;
    public:
        Eigenschaft(const CeGuiString name,
					const CeGuiString nameAbbrev,
                    const CeGuiString description);

        bool operator==(const Eigenschaft& rhs) const;
        bool operator<(const Eigenschaft& rhs) const;

        const CeGuiString getName() const;
        const CeGuiString getNameAbbreviation() const;
        const CeGuiString getDescription() const;
    };
}

#endif //__EIGENSCHAFT_H__
