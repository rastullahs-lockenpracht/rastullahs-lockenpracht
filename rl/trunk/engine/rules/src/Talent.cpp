#include "Talent.h"
#include "DsaManager.h"

namespace rl
{
    Talent::Talent(int id,
                   const CeGuiString& name,
                   const CeGuiString& description,
                   const EigenschaftTripel& eigenschaften,
                   int ebe,
				   int gruppe)
                                       : mId(id),
                                         mName(name),
                                         mDescription(description),
                                         mEigenschaften(eigenschaften),
                                         mEbe(ebe),
										 mTalentGruppe(gruppe)
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

    CeGuiString Talent::getName() const
    {
        return mName;
    }

    CeGuiString Talent::getDescription() const
    {
        return mDescription;
    }

    EigenschaftTripel Talent::getEigenschaften() const
    {
        return mEigenschaften;
    }

	int Talent::calculateEbe(int be) const
	{
		if (mEbe == EBE_KEINE_BE)
			return 0;
		if (mEbe == EBE_BEx2)
			return be*2;
		
		return std::max(be + mEbe, 0);
	}

	int Talent::getEbe() const
	{
		return mEbe;
	}
}
