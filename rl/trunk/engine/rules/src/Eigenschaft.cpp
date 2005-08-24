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

#include "Eigenschaft.h"

namespace rl
{
    Eigenschaft::Eigenschaft(const CeGuiString& name,
							 const CeGuiString& nameAbbrev,
                             const CeGuiString& description) : mName(name),
															   mNameAbbreviation(nameAbbrev),
                                                               mDescription(description)
    {
    }

    bool Eigenschaft::operator==(const Eigenschaft& rhs) const
    {
        return mNameAbbreviation == rhs.mNameAbbreviation;
    }

    bool Eigenschaft::operator<(const Eigenschaft& rhs) const
    {
        return mNameAbbreviation < rhs.mNameAbbreviation;
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
