/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __EIGENSCHAFT_H__
#define __EIGENSCHAFT_H__

#include "RulesPrerequisites.h"

#include "Tripel.h"

namespace rl
{
    static const int EIGENSCHAFT_COUNT = 8;

    static const int E_MUT = 0;
    static const int E_KLUGHEIT = 1;
    static const int E_INTUITION = 2;
    static const int E_CHARISMA = 3;
    static const int E_FINGERFERTIGKEIT = 4;
    static const int E_GEWANDTHEIT = 5;
    static const int E_KONSTITUTION = 6;
    static const int E_KOERPERKRAFT = 7;

    typedef Tripel<int> EigenschaftTripel;

    class _RlRulesExport Eigenschaft
    {
    private:
        const int mId;
        const CeGuiString mName;
		const CeGuiString mNameAbbreviation;
        const CeGuiString mDescription;
    public:
        Eigenschaft(int id,
                    const CeGuiString& name,
					const CeGuiString& nameAbbrev,
                    const CeGuiString& description);

        bool operator==(const Eigenschaft& rhs) const;
        bool operator<(const Eigenschaft& rhs) const;

        int getId() const;
        const CeGuiString& getName() const;
        const CeGuiString& getNameAbbreviation() const;
        const CeGuiString& getDescription() const;
    };
}

#endif
