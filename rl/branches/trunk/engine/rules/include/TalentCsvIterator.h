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

#ifndef __TALENTCSVITERATOR_H__
#define __TALENTCSVITERATOR_H__

#include "RulesPrerequisites.h"
#include "TalentInputIterator.h"
#include "CsvFile.h"

namespace rl {

    class Talent;

    class TalentCsvIterator : public TalentInputIterator {
    public:
        TalentCsvIterator(const std::string filename);
        virtual ~TalentCsvIterator();
        virtual bool hasNext() const;
        virtual void next();
        virtual Talent* createTalent();
    private:
        CsvFile mCsvFile;
        int mCurrentRow;
    };
}
#endif
