#ifndef __TALENT_H__
#define __TALENT_H__

#include "RulesPrerequisites.h"

#include <CEGUIString.h>

#include "Tripel.h"
#include "Eigenschaft.h"

namespace rl
{
    class _RlRulesExport Talent
    {
    private:
        const int mId;
        const CeGuiString mName;
        const CeGuiString mDescription;
        const EigenschaftTripel mEigenschaften;
        const int mEbe;
		const int mTalentGruppe;
    public:
        Talent(int id,
               const CeGuiString& name,
               const CeGuiString& description,
               const EigenschaftTripel& eigenschaften,
               int ebe,
			   int gruppe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        int getId() const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
		int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        EigenschaftTripel getEigenschaften() const;
    };
}
#endif
