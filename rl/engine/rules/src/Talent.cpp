#include "Talent.h"

namespace rl
{
    Talent::Talent(int id,
                   const std::string& name,
                   const std::string& description,
                   const EigenschaftTripel& eigenschaften,
                   const std::string& ebe)
                                       : mId(id),
                                         mName(name),
                                         mDescription(description),
                                         mEigenschaften(eigenschaften),
                                         mEbe(ebe)
    {
    }

    bool Talent::operator==(const Talent& rhs) const
    {
        return mId == rhs.mId;
    }

    bool Talent::operator<(const Talent& rhs) const
    {
        return mId < rhs.mId;
    }

    int Talent::getId() const
    {
        return mId;
    }

    std::string Talent::getName() const
    {
        return mName;
    }

    std::string Talent::getDescription() const
    {
        return mDescription;
    }

    EigenschaftTripel Talent::getEigenschaften() const
    {
        return mEigenschaften;
    }
}
