#ifndef __KAMPFTECHNIK_H__
#define __KAMPFTECHNIK_H__

#include "RulesPrerequisites.h"

#include <CEGUIString.h>

#include "Tripel.h"

namespace rl
{
    class _RlRulesExport Kampftechnik
    {
    private:
        const int mId;
        const CeGuiString mName;
        const CeGuiString mDescription;
        const CeGuiString mEbe;
    public:
        Kampftechnik(int id,
                     const CeGuiString& name,
                     const CeGuiString& description,
                     const CeGuiString& ebe);

        bool operator==(const Kampftechnik& rhs) const;
        bool operator<(const Kampftechnik& rhs) const;
        int getId() const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
        /// Berechnet effektive Behinderung
        int calculateEbe(int be) const;
    };
}
#endif
