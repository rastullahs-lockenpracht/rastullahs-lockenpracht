#include "DsaManager.h"

#include "Eigenschaft.h"
#include "TalentCsvIterator.h"
#include "Talent.h"
#include "KampftechnikCsvIterator.h"
#include "Kampftechnik.h"
#include "Exception.h"
#include <stdlib.h>

template <>
rl::DsaManager* Singleton<rl::DsaManager> ::ms_Singleton = 0;
namespace rl
{
    DsaManager& DsaManager::getSingleton(void)
    {
        return Singleton<DsaManager>::getSingleton();
    }

    DsaManager* DsaManager::getSingletonPtr(void)
    {
        return Singleton<DsaManager>::getSingletonPtr();
    }

    DsaManager::DsaManager() : mTalente(), mEigenschaften()
    {
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));
        initializeEigenschaften();
        initializeTalente();
        initializeKampftechniken();
    }

    DsaManager::~DsaManager()
    {
    }

    void DsaManager::initializeEigenschaften()
    {
        mEigenschaften[E_MUT] = new Eigenschaft(
            E_MUT, "Mut", "");
        mEigenschaften[E_KLUGHEIT] = new Eigenschaft(
            E_KLUGHEIT, "Klugheit", "");
        mEigenschaften[E_INTUITION] = new Eigenschaft(
            E_INTUITION, "Intuition", "");
        mEigenschaften[E_CHARISMA] = new Eigenschaft(
            E_CHARISMA, "Charisma", "");
        mEigenschaften[E_FINGERFERTIGKEIT] = new Eigenschaft(
            E_FINGERFERTIGKEIT, "Fingerfertigkeit", "");
        mEigenschaften[E_GEWANDHEIT] = new Eigenschaft(
            E_GEWANDHEIT, "Gewandheit", "");
        mEigenschaften[E_KONSTITUTION] = new Eigenschaft(
            E_KONSTITUTION, "Konstitution", "");
        mEigenschaften[E_KOERPERKRAFT] = new Eigenschaft(
            E_KOERPERKRAFT, "Körperkraft", "");
    }

    void DsaManager::initializeTalente()
    {
        TalentCsvIterator it("talente.csv");
        while (it.hasNext())
        {
            it.next();
            Talent* t = it.createTalent();
            mTalente.insert(make_pair(t->getId(), t));
        }
    }

    void DsaManager::initializeKampftechniken()
    {
        KampftechnikCsvIterator it("kampftechniken.csv");
        while (it.hasNext())
        {
            it.next();
            Kampftechnik* k = it.createKampftechnik();
            mKampftechniken.insert(make_pair(k->getId(), k));
        }
    }

    RL_LONGLONG DsaManager::getTimestamp()
    {
        RlFail("Not yet implemented.");
        return -1;
    }

    int DsaManager::rollD20()
    {
        double d = rand();
        return static_cast<int>(d * 20.0 / RAND_MAX) + 1;
    }

    Tripel<int> DsaManager::roll3D20()
    {
        return Tripel<int>(rollD20(), rollD20(), rollD20());
    }

    int DsaManager::rollD6()
    {
        double d = rand();
        return static_cast<int>(d * 6.0 / RAND_MAX) + 1;
    }

    Talent* DsaManager::getTalent(int id) const
    {
        TalentMap::const_iterator it = mTalente.find(id);
        if (it != mTalente.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
    }

    Kampftechnik* DsaManager::getKampftechnik(int id) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(id);
        if (it != mKampftechniken.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
    }

    Eigenschaft* DsaManager::getEigenschaft(int id) const
    {
        if (id < 0 || id >= EIGENSCHAFT_COUNT)
        {
            Throw(InvalidArgumentException, "Ungueltige Eigenschaft");
        }
        return mEigenschaften[id];
    }

    int DsaManager::getEigenschaftIdFromString(const string& str)
    {
        if (str.size() != 2)
        {
            Throw(InvalidArgumentException, "String hat ungueltiges Format.");
        }
        int rval;
        switch (str[0]) {
            case 'M':
                rval = E_MUT;
                break;
            case 'I':
                rval = E_INTUITION;
                break;
            case 'C':
                rval = E_CHARISMA;
                break;
            case 'F':
                rval = E_FINGERFERTIGKEIT;
                break;
            case 'G':
                rval = E_GEWANDHEIT;
                break;
            case 'K':
                switch (str[1]) {
                    case 'L':
                        rval = E_KLUGHEIT;
                        break;
                    case 'O':
                        rval = E_KONSTITUTION;
                        break;
                    case 'K':
                        rval = E_KOERPERKRAFT;
                        break;
                    default:
                        Throw(InvalidArgumentException,
                            "String hat ungueltiges Format.");
                }
                break;
            default:
                Throw(InvalidArgumentException,
                    "String hat ungueltiges Format.");
        }
        return rval;
    }
}
