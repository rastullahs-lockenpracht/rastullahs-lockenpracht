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
