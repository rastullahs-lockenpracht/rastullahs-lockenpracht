#ifndef __TALENT_H__
#define __TALENT_H__

#include "RulesPrerequisites.h"
#include "Tripel.h"
#include "Eigenschaft.h"

namespace rl
{
    class _RlRulesExport Talent
    {
    private:
        const int mId;
        const std::string mName;
        const std::string mDescription;
        const EigenschaftTripel mEigenschaften;
        const std::string mEbe;
    public:
        Talent(int id,
               const std::string& name,
               const std::string& description,
               const EigenschaftTripel& eigenschaften,
               const std::string& ebe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
        /// Berechnet effektive Behinderung
        int calculateEbe(int be) const;
        EigenschaftTripel getEigenschaften() const;
    };
}
#endif
