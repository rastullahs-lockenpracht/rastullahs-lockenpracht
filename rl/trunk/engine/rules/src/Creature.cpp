#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"
#include "Talent.h"
#include "Eigenschaft.h"

namespace rl
{
    Creature::Creature(int id,
        const std::string& name, const std::string& description)
        : GameObject(id, name, description), mCurrentLe(0)
    {
    }

	Creature::Creature(int id,
        const std::string& name, const std::string& description,
		const TalentMap& talents)
        : GameObject(id, name, description), mCurrentLe(0)
    {
		mTalente = talents;
    }

    Creature::~Creature()
    {
    }

    int Creature::getEigenschaft(int key) const
    {
        return mEigenschaften[key];
    }

    void Creature::setEigenschaft(int key, int value)
    {
        mEigenschaften[key] = value;
    }

    int Creature::getTalent(int key) const
    {
        TalentMap::const_iterator it = mTalente.find(key);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        return (*it).second;
    }

	const Creature::TalentMap& Creature::getAllTalents() const
	{
		return mTalente;
	}

    void Creature::setTalent(int key, int value)
    {
        TalentMap::iterator it = mTalente.find(key);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        (*it).second = value;
    }

    void Creature::modifyEigenschaft(int key, int mod)
    {
        mEigenschaften[key] = mEigenschaften[key] + mod;
    }

    void Creature::modifyTalent(int key, int mod)
    {
        TalentMap::iterator it = mTalente.find(key);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        (*it).second += mod;
    }

    ///@todo Maximum beachten.
    void Creature::modifyLe(int mod)
    {
        mCurrentLe += mod;
    }

    int Creature::getLe()
    {
        return mCurrentLe;
    }

    ///@todo Richten für negativen Talentwert.
    ///@todo Kritischer Patzer/Erfolg.
    int Creature::doTalentprobe(int id, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(id);
        EigenschaftTripel et(talent->getEigenschaften());

        // Der Probenwurf
        Tripel<int> probe(DsaManager::getSingleton().roll3D20());

        // Vor dem Vergleich hat man den Talentwert übrig.
        int rval = getTalent(id) - modifier;

        int diff1 = getEigenschaft(et.first) - probe.first;
        int diff2 = getEigenschaft(et.second) - probe.second;
        int diff3 = getEigenschaft(et.third) - probe.third;

        // Falls man in einer Eigenschaft hoeher gewurfelt hat,
        // wird die Differenz vom Talentwert abgezogen.
        rval = diff1 < 0 ? rval + diff1 : rval;
        rval = diff2 < 0 ? rval + diff2 : rval;
        rval = diff3 < 0 ? rval + diff3 : rval;
        return rval;
    }

    int Creature::doEigenschaftsprobe(int id, int modifier)
    {
        int rval;

        int probe = DsaManager::getSingleton().rollD6();
        if (probe == 1)
        {
            rval = 100;
        }
        else if (probe == 20)
        {
            rval = -100;
        }
        else
        {
            rval = getEigenschaft(id) - (probe + modifier);
        }
        return rval;
    }

    pair<int, int> Creature::getKampftechnik(int id) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(id);
        if (it == mKampftechniken.end())
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
        return (*it).second;
    }

    void Creature::setKampftechnik(int id, const pair<int, int>& value)
    {
        KampftechnikMap::iterator it = mKampftechniken.find(id);
        if (it == mKampftechniken.end())
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
        (*it).second = value;
    }

    int Creature::getAttackeBasis()
    {
        double es = getEigenschaft(E_MUT) +
            getEigenschaft(E_GEWANDHEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getParadeBasis()
    {
        double es = getEigenschaft(E_INTUITION) +
            getEigenschaft(E_GEWANDHEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getFernkampfBasis()
    {
        double es = getEigenschaft(E_INTUITION) +
            getEigenschaft(E_FINGERFERTIGKEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getInitiativeBasis()
    {
        int es = 2 * getEigenschaft(E_MUT) +
            getEigenschaft(E_INTUITION) +
            getEigenschaft(E_GEWANDHEIT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getMrBasis()
    {
        int es = getEigenschaft(E_MUT) +
            getEigenschaft(E_KLUGHEIT) +
            getEigenschaft(E_KONSTITUTION);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getLeBasis()
    {
        int es =  2 * getEigenschaft(E_KONSTITUTION) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAuBasis()
    {
        int es = getEigenschaft(E_MUT) +
            getEigenschaft(E_KONSTITUTION) +
            getEigenschaft(E_GEWANDHEIT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

}
