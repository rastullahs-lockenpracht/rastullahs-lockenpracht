#include "Eigenschaft.h"

namespace rl
{
    Eigenschaft::Eigenschaft(int id,
                             const std::string& name,
                             const std::string& description) : mId(id),
                                                               mName(name),
                                                               mDescription(description)
    {
    }

    bool Eigenschaft::operator==(const Eigenschaft& rhs) const
    {
        return mId == rhs.mId;
    }

    bool Eigenschaft::operator<(const Eigenschaft& rhs) const
    {
        return mId < rhs.mId;
    }

    int Eigenschaft::getId() const
    {
        return mId;
    }

    std::string Eigenschaft::getName() const
    {
        return mName;
    }

    std::string Eigenschaft::getDescription() const
    {
        return mDescription;
    }
}
