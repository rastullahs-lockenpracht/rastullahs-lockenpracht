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

