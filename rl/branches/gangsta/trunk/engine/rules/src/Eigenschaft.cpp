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

#include "Eigenschaft.h"

namespace rl
{
    Eigenschaft::Eigenschaft(int id,
                             const CeGuiString& name,
							 const CeGuiString& nameAbbrev,
                             const CeGuiString& description) : mId(id),
                                                               mName(name),
															   mNameAbbreviation(nameAbbrev),
                                                               mDescription(description)
    {
    }

    bool Eigenschaft::operator==(const Eigenschaft& rhs) const
    {
        return mId == rhs.mId;
    }

    bool Eigenschaft::operator<(const Eigenschaft& rhs) const
    {
        return mId < rhs.mId;
    }

    int Eigenschaft::getId() const
    {
        return mId;
    }

    const CeGuiString& Eigenschaft::getName() const
    {
        return mName;
    }

    const CeGuiString& Eigenschaft::getNameAbbreviation() const
    {
        return mNameAbbreviation;
    }

    const CeGuiString& Eigenschaft::getDescription() const
    {
        return mDescription;
    }
}
