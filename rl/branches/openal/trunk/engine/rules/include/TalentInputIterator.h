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
