#include "Kampftechnik.h"

namespace rl
{
    Kampftechnik::Kampftechnik(int id,
                   const CeGuiString& name,
                   const CeGuiString& description,
                   const CeGuiString& ebe)
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

    CeGuiString Kampftechnik::getName() const
    {
        return mName;
    }

    CeGuiString Kampftechnik::getDescription() const
    {
        return mDescription;
    }
}
