/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#include "DsaManager.h"

#include "Eigenschaft.h"
#include "Talent.h"
#include "Kampftechnik.h"
#include "Person.h"
#include "RulesSubsystem.h"
#include "Date.h"
#include "DsaDataLoader.h"

#include "Exception.h"
#include "CoreSubsystem.h"

template <>
	rl::DsaManager* Ogre::Singleton<rl::DsaManager> ::ms_Singleton = 0;

using namespace std;
using CEGUI::utf8;

namespace rl
{
    DsaManager& DsaManager::getSingleton(void)
    {
        return Ogre::Singleton<DsaManager>::getSingleton();
    }

    DsaManager* DsaManager::getSingletonPtr(void)
    {
        return Ogre::Singleton<DsaManager>::getSingletonPtr();
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
		/// @warning So nicht lokalisierbar
        mEigenschaften[E_MUT] = new Eigenschaft(
            (utf8*)"Mut", (utf8*)"MU", (utf8*)"");
        mEigenschaften[E_KLUGHEIT] = new Eigenschaft(
            (utf8*)"Klugheit", (utf8*)"KL", (utf8*)"");
        mEigenschaften[E_INTUITION] = new Eigenschaft(
            (utf8*)"Intuition", (utf8*)"IN", (utf8*)"");
        mEigenschaften[E_CHARISMA] = new Eigenschaft(
            (utf8*)"Charisma", (utf8*)"CH", (utf8*)"");
        mEigenschaften[E_FINGERFERTIGKEIT] = new Eigenschaft(
            (utf8*)"Fingerfertigkeit", (utf8*)"FF", (utf8*)"");
        mEigenschaften[E_GEWANDTHEIT] = new Eigenschaft(
            (utf8*)"Gewandtheit", (utf8*)"GE", (utf8*)"");
        mEigenschaften[E_KONSTITUTION] = new Eigenschaft(
            (utf8*)"Konstitution", (utf8*)"KO", (utf8*)"");
        mEigenschaften[E_KOERPERKRAFT] = new Eigenschaft(
            (utf8*)"Körperkraft", (utf8*)"KK", (utf8*)"");
    }

    void DsaManager::initializeTalente()
    {
		mTalente.clear();
    }

	void DsaManager::_addTalent(Talent* talent)
	{
		mTalente.insert(make_pair(talent->getName(), talent));
	}

	void DsaManager::_addPerson(Person* person)
	{
		mPersonen.insert(make_pair(person->getId(), person));
	}

	void DsaManager::_addKampftechnik(Kampftechnik* kampftechnik)
	{
		mKampftechniken.insert(make_pair(kampftechnik->getName(), kampftechnik));
	}


    void DsaManager::initializeKampftechniken()
    {
		mKampftechniken.clear();
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

    Talent* DsaManager::getTalent(const CeGuiString& talentName) const
    {
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
    }

    Kampftechnik* DsaManager::getKampftechnik(const CeGuiString& kampftechnikName) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
        if (it != mKampftechniken.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(IllegalArgumentException, "Kampftechnik nicht gefunden.");
        }
    }

    Eigenschaft* DsaManager::getEigenschaft(const CeGuiString& eigenschaftName) const
    {
		EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
		if (it != mEigenschaften.end())
		{
			return (*it).second;
		}
		else
		{
			Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
		}
    }

	bool DsaManager::isDkDistance(const Weapon::Distanzklasse& dk, const Ogre::Real& distance) const
	{
		switch (dk)
		{
		case Weapon::DK_H:
			return distance < 1;
		case Weapon::DK_N:
			return 1 <= distance && distance < 2;
		case Weapon::DK_S:
			return 2 <= distance && distance < 3;
		case Weapon::DK_P:
			return 1 < distance;
		default:
			Throw(IllegalArgumentException, "Distanzklasse unbekannt");
		}
	}


/*	int DsaManager::getEigenschaftIdFromLongString(const CeGuiString& str) const
	{
		Logger::getSingleton().log(Logger::RULES, Ogre::LML_TRIVIAL, "DsaManager sucht "+str);
		for (int idx = 0; idx < EIGENSCHAFT_COUNT; idx++)
		{
			Logger::getSingleton().log(Logger::RULES, Ogre::LML_TRIVIAL, "DsaManager findet "+mEigenschaften[idx]->getName());
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
*/
	/**
	 * @todo SKT laden/erzeugen
	 */
	void DsaManager::initializeSkt()
	{

	}

	int DsaManager::getSteigerKosten(int column, int from) const
	{
		if (column < 0 || column >= SKT_COLUMNS)
			Throw(IllegalArgumentException, "Spalte in SKT nicht gefunden.");
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
            Throw(IllegalArgumentException, "Person nicht gefunden.");
        }
	}

	void DsaManager::loadDsaDataFile(const std::string& filename)
	{
		DsaDataLoader::loadData(filename);
	}
}
