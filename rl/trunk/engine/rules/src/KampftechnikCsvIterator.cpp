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
        string name = mCsvFile.getValueAsString(mCurrentRow, 1);
        string desc = mCsvFile.getValueAsString(mCurrentRow, 2);
        string ebe = mCsvFile.getValueAsString(mCurrentRow, 3);
        return new Kampftechnik(name, desc, ebe);
    }
}
