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

#ifndef __KAMPFTECHNIK_H__
#define __KAMPFTECHNIK_H__

#include "RulesPrerequisites.h"

#include <CEGUIString.h>

#include "Tripel.h"

namespace rl
{
    class _RlRulesExport Kampftechnik
    {
    private:
        const int mId;
        const CeGuiString mName;
        const CeGuiString mDescription;
        const CeGuiString mEbe;
    public:
        Kampftechnik(int id,
                     const CeGuiString& name,
                     const CeGuiString& description,
                     const CeGuiString& ebe);

        bool operator==(const Kampftechnik& rhs) const;
        bool operator<(const Kampftechnik& rhs) const;
        int getId() const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
        /// Berechnet effektive Behinderung
        int calculateEbe(int be) const;
    };
}
#endif
