#ifndef __KAMPFTECHNIKCSVITERATOR_H__
#define __KAMPFTECHNIKCSVITERATOR_H__

#include "RulesPrerequisites.h"
#include "CsvFile.h"

namespace rl {

    class Kampftechnik;

    class KampftechnikCsvIterator {
    public:
        KampftechnikCsvIterator(const std::string filename);
        virtual ~KampftechnikCsvIterator();
        virtual bool hasNext() const;
        virtual void next();
        virtual Kampftechnik* createKampftechnik();
    private:
        CsvFile mCsvFile;
        int mCurrentRow;
    };
}
#endif
