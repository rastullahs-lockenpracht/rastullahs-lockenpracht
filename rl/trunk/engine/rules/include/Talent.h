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
        const int mEbe;
		const int mTalentGruppe;
    public:
        Talent(int id,
               const std::string& name,
               const std::string& description,
               const EigenschaftTripel& eigenschaften,
               int ebe,
			   int gruppe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
		int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        EigenschaftTripel getEigenschaften() const;
    };
}
#endif
