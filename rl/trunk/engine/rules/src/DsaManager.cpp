#include "DsaManager.h"

#include "Eigenschaft.h"
#include "Talent.h"
#include "KampftechnikCsvIterator.h"
#include "Kampftechnik.h"
#include "Person.h"

#include "Exception.h"


template <>
rl::DsaManager* Singleton<rl::DsaManager> ::ms_Singleton = 0;

using namespace std;

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
		initializeSkt();

        initializeTalente();
        initializeKampftechniken();		
    }

    DsaManager::~DsaManager()
    {
    }


    void DsaManager::initializeEigenschaften()
    {
        mEigenschaften[E_MUT] = new Eigenschaft(
            E_MUT, "Mut", "MU", "");
        mEigenschaften[E_KLUGHEIT] = new Eigenschaft(
            E_KLUGHEIT, "Klugheit", "KL", "");
        mEigenschaften[E_INTUITION] = new Eigenschaft(
            E_INTUITION, "Intuition", "IN", "");
        mEigenschaften[E_CHARISMA] = new Eigenschaft(
            E_CHARISMA, "Charisma", "CH", "");
        mEigenschaften[E_FINGERFERTIGKEIT] = new Eigenschaft(
            E_FINGERFERTIGKEIT, "Fingerfertigkeit", "FF", "");
        mEigenschaften[E_GEWANDHEIT] = new Eigenschaft(
            E_GEWANDHEIT, "Gewandheit", "GE", "");
        mEigenschaften[E_KONSTITUTION] = new Eigenschaft(
            E_KONSTITUTION, "Konstitution", "KO", "");
        mEigenschaften[E_KOERPERKRAFT] = new Eigenschaft(
            E_KOERPERKRAFT, "Körperkraft", "KK", "");
    }

    void DsaManager::initializeTalente()
    {
		mTalente.clear();
    }

	void DsaManager::_addTalent(Talent* talent)
	{
		mTalente.insert(make_pair(talent->getId(), talent));
	}

	void DsaManager::_addPerson(Person* person)
	{
		mPersonen.insert(make_pair(person->getId(), person));
	}

    void DsaManager::initializeKampftechniken()
    {
		mKampftechniken.clear();

/*        KampftechnikCsvIterator it("kampftechniken.csv");
        while (it.hasNext())
        {
            it.next();
            Kampftechnik* k = it.createKampftechnik();
            mKampftechniken.insert(make_pair(k->getId(), k));
        }*/
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

	Talent* DsaManager::getTalent(std::string name) const
    {
		for (TalentMap::const_iterator it = mTalente.begin(); it != mTalente.end(); it++)
		{
			if ((*it).second->getName().compare(name) == 0)
			{
				return (*it).second;
			}
		}
        
		Throw(InvalidArgumentException, "Talent nicht gefunden.");
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

    int DsaManager::getEigenschaftIdFromString(const string& str) const
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

	/**
	 * @todo SKT laden/erzeugen
	 */
	void DsaManager::initializeSkt()
	{

	}

	int DsaManager::getSteigerKosten(int column, int from) const
	{
		if (column < 0 || column >= SKT_COLUMNS)
			Throw(InvalidArgumentException, "Spalte in SKT nicht gefunden.");
		if (from < 0)
			return 0;
		else if (from < SKT_ROWS)
			return mSteigerkostenTabelle[column][from];
		else
			return 0;
	}

	int DsaManager::getSteigerKosten(int column, int from, int to) const
	{
		int sum = 0;

		for (int i=from; i<to; i++)
			sum += getSteigerKosten(column, i);

		return sum;
	}

	Person* DsaManager::getPerson(int id) const
	{
		PersonMap::const_iterator it = mPersonen.find(id);
        if (it != mPersonen.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(InvalidArgumentException, "Person nicht gefunden.");
        }
	}
}
