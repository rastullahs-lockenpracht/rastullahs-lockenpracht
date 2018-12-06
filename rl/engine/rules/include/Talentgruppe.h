/* (C) 2003-2008. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */
#ifndef __TALENTGRUPPE_H__
#define __TALENTGRUPPE_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class _RlRulesExport Talentgruppe
    {
    private:
        const int mId;
        const std::string mName;
        const std::string mDescription;
    public:
        Talentgruppe(int id,
                     const std::string& name,
                     const std::string& description);
        bool operator==(const Talentgruppe& rhs) const;
        bool operator<(const Talentgruppe& rhs) const;
        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
    };
}
#endif //__TALENTGRUPPE_H__
