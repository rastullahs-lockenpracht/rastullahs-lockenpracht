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

#include "KampftechnikCsvIterator.h"
#include "Kampftechnik.h"

using namespace std;

namespace rl {

    KampftechnikCsvIterator::KampftechnikCsvIterator(const std::string filename)
        : mCsvFile(filename), mCurrentRow(-1)
    {
    }

    KampftechnikCsvIterator::~KampftechnikCsvIterator()
    {
    }

    bool KampftechnikCsvIterator::hasNext() const
    {
        return (mCurrentRow + 1) < mCsvFile.getRowCount();
    }

    void KampftechnikCsvIterator::next()
    {
        mCurrentRow++;
    }

    Kampftechnik* KampftechnikCsvIterator::createKampftechnik()
    {
        int id = mCsvFile.getValueAsInt(mCurrentRow, 0);
        string name = mCsvFile.getValueAsString(mCurrentRow, 1);
        string desc = mCsvFile.getValueAsString(mCurrentRow, 2);
        string ebe = mCsvFile.getValueAsString(mCurrentRow, 3);
        return new Kampftechnik(id, name, desc, ebe);
    }
}
