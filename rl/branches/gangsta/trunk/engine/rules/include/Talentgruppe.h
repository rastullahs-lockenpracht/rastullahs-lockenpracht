#ifndef __TALENTGRUPPE_H__
#define __TALENTGRUPPE_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class _RlRulesExport Talentgruppe
    {
    private:
        const int mId;
        const std::string mName;
        const std::string mDescription;
    public:
        Talentgruppe(int id,
                     const std::string& name,
                     const std::string& description);
        bool operator==(const Talentgruppe& rhs) const;
        bool operator<(const Talentgruppe& rhs) const;
        int getId() const;
        std::string getName() const;
        std::string getDescription() const;
    };
}
#endif
