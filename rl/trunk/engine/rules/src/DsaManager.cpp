/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "DsaManager.h"

#include "Eigenschaft.h"
#include "Talent.h"
#include "KampftechnikCsvIterator.h"
#include "Kampftechnik.h"
#include "Person.h"
#include "RulesSubsystem.h"
#include "Date.h"

#include "Exception.h"
#include "CoreSubsystem.h"


template <>
rl::DsaManager* Singleton<rl::DsaManager> ::ms_Singleton = 0;

using namespace std;
using CEGUI::utf8;

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
            E_MUT, (utf8*)"Mut", (utf8*)"MU", (utf8*)"");
        mEigenschaften[E_KLUGHEIT] = new Eigenschaft(
            E_KLUGHEIT, (utf8*)"Klugheit", (utf8*)"KL", (utf8*)"");
        mEigenschaften[E_INTUITION] = new Eigenschaft(
            E_INTUITION, (utf8*)"Intuition", (utf8*)"IN", (utf8*)"");
        mEigenschaften[E_CHARISMA] = new Eigenschaft(
            E_CHARISMA, (utf8*)"Charisma", (utf8*)"CH", (utf8*)"");
        mEigenschaften[E_FINGERFERTIGKEIT] = new Eigenschaft(
            E_FINGERFERTIGKEIT, (utf8*)"Fingerfertigkeit", (utf8*)"FF", (utf8*)"");
        mEigenschaften[E_GEWANDTHEIT] = new Eigenschaft(
            E_GEWANDTHEIT, (utf8*)"Gewandtheit", (utf8*)"GE", (utf8*)"");
        mEigenschaften[E_KONSTITUTION] = new Eigenschaft(
            E_KONSTITUTION, (utf8*)"Konstitution", (utf8*)"KO", (utf8*)"");
        mEigenschaften[E_KOERPERKRAFT] = new Eigenschaft(
            E_KOERPERKRAFT, (utf8*)"Körperkraft", (utf8*)"KK", (utf8*)"");
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
		return mBaseTime + CoreSubsystem::getSingleton().getClock();
    }

	Date DsaManager::getCurrentDate()
	{
		return Date(getTimestamp());
	}

	void DsaManager::setTimestamp(const RL_LONGLONG time)
	{
		mBaseTime = time;
		CoreSubsystem::getSingleton().resetClock();
	}

	void DsaManager::setCurrentDate(const Date& date)
	{
		setTimestamp(date.getTimestamp());
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

	int DsaManager::roll(int d6, int d20)
	{
		int sum = 0;

		for (int d = 0; d < d6; d++)
			sum += rollD6();
		for (int d = 0; d < d20; d++)
			sum += rollD20();

		return sum;
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

	Talent* DsaManager::getTalent(const CeGuiString& name) const
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

	int DsaManager::getEigenschaftIdFromLongString(const CeGuiString& str) const
	{
		RulesSubsystem::getSingleton().log("DsaManager sucht "+str);
		for (int idx = 0; idx < EIGENSCHAFT_COUNT; idx++)
		{
			RulesSubsystem::getSingleton().log("DsaManager findet "+mEigenschaften[idx]->getName());
			if (mEigenschaften[idx]->getName().compare(str) == 0)
				return idx;
		}

		Throw(InvalidArgumentException, "Ungueltige Eigenschaft");
	}

	int DsaManager::getEigenschaftIdFromString(const CeGuiString& str) const
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
                rval = E_GEWANDTHEIT;
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
