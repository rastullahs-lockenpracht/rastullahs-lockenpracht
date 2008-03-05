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
#ifndef __TALENTINPUTITERATOR_H__
#define __TALENTINPUTITERATOR_H__

#include "RulesPrerequisites.h"

namespace rl {

    class Talent;

    class TalentInputIterator {
    protected:
        TalentInputIterator() {};
    public:
        virtual ~TalentInputIterator() {};
        virtual bool hasNext() const = 0;
        virtual void next() = 0;
        virtual Talent* createTalent() = 0;
    };
}
#endif
