#include "Kampftechnik.h"

namespace rl
{
    Kampftechnik::Kampftechnik(int id,
                   const std::string& name,
                   const std::string& description,
                   const std::string& ebe)
                                       : mId(id),
                                         mName(name),
                                         mDescription(description),
                                         mEbe(ebe)
    {
    }

    bool Kampftechnik::operator==(const Kampftechnik& rhs) const
    {
        return mId == rhs.mId;
    }

    bool Kampftechnik::operator<(const Kampftechnik& rhs) const
    {
        return mId < rhs.mId;
    }

    int Kampftechnik::getId() const
    {
        return mId;
    }

    std::string Kampftechnik::getName() const
    {
        return mName;
    }

    std::string Kampftechnik::getDescription() const
    {
        return mDescription;
    }
}
