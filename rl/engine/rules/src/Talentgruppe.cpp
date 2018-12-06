
#include "stdinc.h"

#include "Talentgruppe.h"

namespace rl
{
    Talentgruppe::Talentgruppe(int id, const std::string& name, const std::string& description)
        : mId(id)
        , mName(name)
        , mDescription(description)
    {
    }

    bool Talentgruppe::operator==(const Talentgruppe& rhs) const
    {
        return mId == rhs.mId;
    }

    bool Talentgruppe::operator<(const Talentgruppe& rhs) const
    {
        return mId < rhs.mId;
    }

    int Talentgruppe::getId() const
    {
        return mId;
    }

    std::string Talentgruppe::getName() const
    {
        return mName;
    }

    std::string Talentgruppe::getDescription() const
    {
        return mDescription;
    }
}
