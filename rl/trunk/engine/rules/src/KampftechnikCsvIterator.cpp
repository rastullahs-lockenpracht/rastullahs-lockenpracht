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
