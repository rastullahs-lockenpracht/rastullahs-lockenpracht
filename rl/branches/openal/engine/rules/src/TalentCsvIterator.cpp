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

#include "TalentCsvIterator.h"
#include "DsaManager.h"
#include "Talent.h"
#include "Eigenschaft.h"

using namespace std;

namespace rl {

    TalentCsvIterator::TalentCsvIterator(const std::string filename)
        : mCsvFile(filename), mCurrentRow(-1)
    {
    }

    TalentCsvIterator::~TalentCsvIterator()
    {
    }

    bool TalentCsvIterator::hasNext() const
    {
        return (mCurrentRow + 1) < mCsvFile.getRowCount();
    }

    void TalentCsvIterator::next()
    {
        mCurrentRow++;
    }

    Talent* TalentCsvIterator::createTalent()
    {
        DsaManager& dm = DsaManager::getSingleton();

        int id = mCsvFile.getValueAsInt(mCurrentRow, 1);
        string name = mCsvFile.getValueAsString(mCurrentRow, 2);
        string desc = mCsvFile.getValueAsString(mCurrentRow, 3);
        EigenschaftTripel et(
            dm.getEigenschaftIdFromString(
                mCsvFile.getValueAsString(mCurrentRow, 4)),
            dm.getEigenschaftIdFromString(
                mCsvFile.getValueAsString(mCurrentRow, 5)),
            dm.getEigenschaftIdFromString(
                mCsvFile.getValueAsString(mCurrentRow, 6)));

        //string ebe = mCsvFile.getValueAsString(mCurrentRow, 7);
		
        return new Talent(id, name, desc, et, 0, id/100);
    }

}

