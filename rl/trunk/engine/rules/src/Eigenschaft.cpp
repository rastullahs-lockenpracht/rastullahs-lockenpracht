#include "Eigenschaft.h"

namespace rl
{
    Eigenschaft::Eigenschaft(int id,
                             const CeGuiString& name,
							 const CeGuiString& nameAbbrev,
                             const CeGuiString& description) : mId(id),
                                                               mName(name),
															   mNameAbbreviation(nameAbbrev),
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

    const CeGuiString& Eigenschaft::getName() const
    {
        return mName;
    }

    const CeGuiString& Eigenschaft::getNameAbbreviation() const
    {
        return mNameAbbreviation;
    }

    const CeGuiString& Eigenschaft::getDescription() const
    {
        return mDescription;
    }
}
