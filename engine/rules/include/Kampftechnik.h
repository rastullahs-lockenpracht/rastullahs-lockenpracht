#ifndef __KAMPFTECHNIK_H__
#define __KAMPFTECHNIK_H__

#include "RulesPrerequisites.h"
#include "Tripel.h"

namespace rl
{
    class _RlRulesExport Kampftechnik
    {
    private:
        const int mId;
        const std::string mName;
        const std::string mDescription;
        const std::string mEbe;
    public:
        Kampftechnik(int id,
                     const std::string& name,
                     const std::string& description,
                     const std::string& ebe);

        bool operator==(const Kampftechnik& rhs) const;
        bool operator<(const Kampftechnik& rhs) const;
        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
        /// Berechnet effektive Behinderung
        int calculateEbe(int be) const;
    };
}
#endif
