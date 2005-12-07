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


#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"
#include "Talent.h"
#include "Eigenschaft.h"
#include "Kampftechnik.h"

namespace rl
{
    Creature::Creature(const CeGuiString& name, const CeGuiString& description)
        : GameObject(name, description), mCurrentLe(0)
    {
		setWert(WERT_MOD_AE, 0);
		setWert(WERT_MOD_LE, 0);
		setWert(WERT_MOD_AT, 0);
		setWert(WERT_MOD_PA, 0);
		setWert(WERT_MOD_FK, 0);
		setWert(WERT_MOD_AU, 0);
		setWert(WERT_MOD_MR, 0);
		setWert(WERT_MOD_INI, 0);
		mEigenschaften[E_MUT] = 0;
		mEigenschaften[E_KLUGHEIT] = 0;
		mEigenschaften[E_INTUITION] = 0;
		mEigenschaften[E_CHARISMA] = 0;
		mEigenschaften[E_FINGERFERTIGKEIT] = 0;
		mEigenschaften[E_GEWANDTHEIT] = 0;
		mEigenschaften[E_KONSTITUTION] = 0;
		mEigenschaften[E_KOERPERKRAFT] = 0;
		mCurrentBe = 0;
    }

	Creature::~Creature()
    {
    }

    int Creature::getEigenschaft(const CeGuiString& eigenschaftName) const
    {
		EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
		if (it == mEigenschaften.end())
		{
			Throw(InvalidArgumentException, "Eigenschaft nicht gefunden.");
		}
        return (*it).second;
    }

    void Creature::setEigenschaft(const CeGuiString& eigenschaftName, int value)
    {
        EigenschaftMap::iterator it = mEigenschaften.find(eigenschaftName);
        if (it == mEigenschaften.end())
        {
            Throw(InvalidArgumentException, "Eigenschaft nicht gefunden.");
        }
        (*it).second = value;
		fireObjectStateChangeEvent();
    }

    int Creature::getTalent(const CeGuiString& talentName) const
    {
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        return (*it).second;
    }

	void Creature::addTalent(const CeGuiString& talentName, int value)
	{
		TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
			Throw(InvalidArgumentException, "Talent schon in mTalente enthalten.");
        }
	    DsaManager::getSingleton().getTalent(talentName); //ueberpruefe ob es das Talent ueberhaupt gibt
		mTalente[talentName] = value;
		fireObjectStateChangeEvent();
	}

	const Creature::TalentMap& Creature::getAllTalents() const
	{
		return mTalente;
	}

    void Creature::setTalent(const CeGuiString& talentName, int value)
    {
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        (*it).second = value;
		fireObjectStateChangeEvent();
    }

	void Creature::addSe(const CeGuiString& talentName)
	{
		///@todo Mit Code fuellen
	}

    int Creature::getSf(const CeGuiString& sfName) const
    {
        SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(InvalidArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        return (*it).second;
    }

	void Creature::addSf(const CeGuiString& sfName, int value)
	{
		SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
		if (it != mSonderfertigkeiten.end())
		{
			Throw(InvalidArgumentException, "Sonderfertigkeit schon in mSonderfertigkeiten enthalten.");
		}
		//ueberpruefe ob es die Sonderfertigkeit ueberhaupt gib
		DsaManager::getSingleton().getTalent(sfName);
		mSonderfertigkeiten[sfName] = value;
		fireObjectStateChangeEvent();
	}

    void Creature::setSf(const CeGuiString& sfName, int value)
    {
		if (value < SF_MIN_VALUE || value > SF_MAX_VALUE)
		{
			Throw(OutOfRangeException, "Der Sonderfertigkeit soll ein" 
				"unzulässiger Wert zugewiesen werden");
		}
        SonderfertigkeitMap::iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(InvalidArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        (*it).second = value;
		fireObjectStateChangeEvent();
    }

    void Creature::modifyEigenschaft(const CeGuiString& eigenschaftName, int mod)
    {
        mEigenschaften[eigenschaftName] = mEigenschaften[eigenschaftName] + mod;
		fireObjectStateChangeEvent();
    }

    void Creature::modifyTalent(const CeGuiString& talentName, int mod)
    {
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        (*it).second += mod;
		fireObjectStateChangeEvent();
    }

    void Creature::modifyLe(int mod, bool ignoreMax)
    {
        mCurrentLe += mod;
		if (!ignoreMax)
			mCurrentLe = min(mCurrentLe, getLeMax());
		fireObjectStateChangeEvent();
    }

    int Creature::getLe()
    {
        return mCurrentLe;
    }
    
    int Creature::getLeMax()
    {
		return getLeBasis() + getWert(WERT_MOD_LE);
    }

    void Creature::modifyAe(int mod, bool ignoreMax)
    {
        mCurrentAe += mod;
		if (!ignoreMax)
			mCurrentAe = min(mCurrentAe, getAeMax());
		fireObjectStateChangeEvent();
    }

    int Creature::getAe()
    {
        return mCurrentAe;
    }
    
    int Creature::getAeMax()
    {
		return isMagic()?getAeBasis() + getWert(WERT_MOD_AE):0;
    }

	bool Creature::isMagic()
	{
		return getWert(WERT_MOD_AE) > 0;
	}

    void Creature::modifyAu(int mod, bool ignoreMax)
    {
        mCurrentAu += mod;
		if (!ignoreMax)
			mCurrentAu = min(mCurrentAu, getAuMax());
		fireObjectStateChangeEvent();
    }

    int Creature::getAu()
    {
        return mCurrentAu;
    }
    
    int Creature::getAuMax()
    {
		return getAuBasis() + getWert(WERT_MOD_AU);
    }

    int Creature::doAlternativeTalentprobe(const CeGuiString& talentName, int spezialisierungId,
		int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(eigenschaft1Name, eigenschaft2Name, eigenschaft3Name);

        // Der Probenwurf
        Tripel<int> probe(DsaManager::getSingleton().roll3D20());

		/**
		 * @warning Ich dachte ich haette irgendwo mal gelesen das bei einer
		 *          Dreifach-1 der Talentwert *sofort* um 1 angehoben wird,
		 *			bei einer Dreifach-20 sofort gesenkt. Weiss jemand was
		 *			darueber?
		 */
		// Glueckliche
		if ( (probe.first == 1) && (probe.second == 1) && (probe.third == 1) ) 
		{
			addSe(talentName);
			return RESULT_SPEKT_AUTOERFOLG;
		}
		if ( ((probe.first == 1) && (probe.second == 1)) || 
			 ((probe.first == 1) && (probe.third == 1)) ||
			 ((probe.second == 1) && (probe.third == 1))) 
		{
			addSe(talentName);
			return RESULT_AUTOERFOLG;
		}
		// Patzer
		if ((probe.first == 20) && (probe.second == 20) && (probe.third == 20)) 
		{
			addSe(talentName);
			return RESULT_SPEKT_AUTOMISSERFOLG; 
		}
		if ( ((probe.first == 20) && (probe.second == 20)) || 
			 ((probe.first == 20) && (probe.third == 20)) ||
			 ((probe.second == 20) && (probe.third == 20))) 
		{
			addSe(talentName);
			return RESULT_AUTOMISSERFOLG;
		}


        // Vor dem Vergleich hat man den Talentwert übrig.
		int taW = 0;
		int eBe = DsaManager::getSingleton().getTalent(talentName)->calculateEbe(mCurrentBe);
		try 
		{
			//if (1 == getSf(sfName)) taW = 2; //Spezialisiereung?
		}
		catch(InvalidArgumentException){};
		taW += getTalent(talentName);
        int rval = taW - modifier - eBe;
		// Bei negativen TaP*
		int handicap = 0;
		if (rval < 0)
		{
			handicap = -rval;
			rval = 0;
		}

        int diff1 = getEigenschaft(et.first) - probe.first - handicap;
        int diff2 = getEigenschaft(et.second) - probe.second - handicap;
        int diff3 = getEigenschaft(et.third) - probe.third - handicap;

        // Falls man in einer Eigenschaft hoeher gewurfelt hat,
        // wird die Differenz vom Talentwert abgezogen.
        rval = diff1 < 0 ? rval + diff1 : rval;
        rval = diff2 < 0 ? rval + diff2 : rval;
        rval = diff3 < 0 ? rval + diff3 : rval;

		// TaP* niemals größer als TaW (MFF14)
		if (rval > taW) rval = taW;
        return rval;
    }

    int Creature::doAlternativeTalentprobe(const CeGuiString& talentName, int modifier, 
		CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
		return doAlternativeTalentprobe(talentName, -1, modifier, eigenschaft1Name,
			eigenschaft2Name, eigenschaft3Name);
	}

    int Creature::doTalentprobe(const CeGuiString& talentName, int spezialisierungId, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, spezialisierungId, modifier, 
			et.first, et. second, et.third);
	}

    int Creature::doTalentprobe(const CeGuiString& talentName, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, -1, modifier, et.first, 
			et. second, et.third);
	}

    int Creature::doEigenschaftsprobe(const CeGuiString& eigenschaftName, int modifier)
    {
        int rval;

        int probe = DsaManager::getSingleton().rollD20();
        if (probe == 1)
        {
            rval = RESULT_GLUECKLICH;
        }
        else if (probe == 20)
        {
            rval = RESULT_PATZER;
        }
        else
        {
            rval = getEigenschaft(eigenschaftName) - (probe + modifier);
        }
        return rval;
    }

	void Creature::addKampftechnik(const CeGuiString& kampftechnikName, const pair<int,int>& value)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it != mKampftechniken.end())
		{
			Throw(InvalidArgumentException, 
				"Kampftechnik schon in mKampftechniken enthalten.");
		}
		//ueberpruefe ob es die Kampftechnik ueberhaupt gibt
		DsaManager::getSingleton().getKampftechnik(kampftechnikName); 
		mKampftechniken[kampftechnikName] = value;
		fireObjectStateChangeEvent();
	}

    pair<int, int> Creature::getKampftechnik(const CeGuiString& kampftechnikName) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
        if (it == mKampftechniken.end())
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
        return (*it).second;
    }

    void Creature::setKampftechnik(const CeGuiString& kampftechnikName, const pair<int, int>& value)
    {
        KampftechnikMap::iterator it = mKampftechniken.find(kampftechnikName);
        if (it == mKampftechniken.end())
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
        (*it).second = value;
    }

    int Creature::getAttackeBasis() const
    {
        double es = getEigenschaft(E_MUT) +
            getEigenschaft(E_GEWANDTHEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getParadeBasis() const
    {
        double es = getEigenschaft(E_INTUITION) +
            getEigenschaft(E_GEWANDTHEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getFernkampfBasis() const
    {
        double es = getEigenschaft(E_INTUITION) +
            getEigenschaft(E_FINGERFERTIGKEIT) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getInitiativeBasis() const
    {
        int es = 2 * getEigenschaft(E_MUT) +
            getEigenschaft(E_INTUITION) +
            getEigenschaft(E_GEWANDTHEIT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getMrBasis() const
    {
        int es = getEigenschaft(E_MUT) +
            getEigenschaft(E_KLUGHEIT) +
            getEigenschaft(E_KONSTITUTION);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getLeBasis() const
    {
        int es =  2 * getEigenschaft(E_KONSTITUTION) +
            getEigenschaft(E_KOERPERKRAFT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAuBasis() const
    {
        int es = getEigenschaft(E_MUT) +
            getEigenschaft(E_KONSTITUTION) +
            getEigenschaft(E_GEWANDTHEIT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

	int Creature::getAeBasis() const
	{
        int es = getEigenschaft(E_MUT) +
			getEigenschaft(E_INTUITION) +
            getEigenschaft(E_CHARISMA);

        return static_cast<int>(es / 2.0 + 0.5);
    }

	int Creature::getWert(int wertId) const
	{
		WertMap::const_iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            Throw(InvalidArgumentException, "Wert nicht gefunden.");
        }
        return (*it).second;
	}

	int Creature::getCurrentBe()
	{
		return mCurrentBe; /// @todo - Ruestungsgewoehnung
	}

	void Creature::setCurrentBe(int newBe)
	{
		mCurrentBe = newBe;
	}

	int Creature::getCurrentInitiativeModifier()
	{
		return mCurrentInitiativeModifier;
	}

	void Creature::setCurrentInitiativeModifier(int newIniMod)
	{
		mCurrentInitiativeModifier = newIniMod;
	}

	void Creature::setWert(int wertId, int wert)
	{
		WertMap::iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            mWerte.insert(make_pair(wertId, wert));
        }
		else
		{
	        (*it).second = wert;
		}
	}

	void Creature::addContainer(Container* container)
	{
		if (container == NULL)
			Throw(InvalidArgumentException, "Nullpointer uebergeben");
		ContainerMap::const_iterator it = mContainer.find(container->getName());
		if (it != mContainer.end())
		{
			Throw(InvalidArgumentException, 
				"Container bereits in mContainer enthalten.");
		}
		mContainer.insert(make_pair(container->getName(), container));
	}

	Container* Creature::getContainer(const CeGuiString& containerName) const
	{
		ContainerMap::const_iterator it = mContainer.find(containerName);
		if (it == mContainer.end())
		{
			Throw(InvalidArgumentException, "Container nicht in mContainer gefunden.");
		}
		return (*it).second;	
	}

	Container* Creature::removeContainer(const CeGuiString& containerName)
	{
		ContainerMap::iterator it = mContainer.find(containerName);
		if (it == mContainer.end())
		{
			Throw(InvalidArgumentException, "Container nicht in mContainer gefunden.");
		}
		Container* rval = (*it).second;
		mContainer.erase(it);
		return rval;
	}

	void Creature::addWeapon(Weapon* weapon)
	{
		if (weapon == NULL)
			Throw(InvalidArgumentException, "Nullpointer uebergeben");
		WeaponMap::const_iterator it = mWeapons.find(weapon->getId());
		if (it != mWeapons.end())
		{
			Throw(InvalidArgumentException, 
				"weaponId bereits in mWeapons enthalten.");
		}
		mWeapons.insert(make_pair(weapon->getId(), weapon));
	}

	Weapon* Creature::getWeapon(int weaponId) const
	{
		WeaponMap::const_iterator it = mWeapons.find(weaponId);
		if (it == mWeapons.end())
		{
			Throw(InvalidArgumentException, "weaponId nicht in mWeapons gefunden.");
		}
		return (*it).second;	
	}

	Weapon* Creature::removeWeapon(int weaponId)
	{
		WeaponMap::iterator it = mWeapons.find(weaponId);
		if (it == mWeapons.end())
		{
			Throw(InvalidArgumentException, "weaponId nicht in mWeapons gefunden.");
		}
		Weapon* rval = (*it).second;
		mWeapons.erase(it);
		return rval;
	}

	int Creature::doAttacke(const CeGuiString& kampftechnikName, int modifier)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it == mKampftechniken.end())
		{
			Throw(InvalidArgumentException, "kampftechnikName nicht in mKampftechniken gefunden");
		}
		int rval;
		int eBe = floor(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getCurrentBe())) / 2.0);

		int probe = DsaManager::getSingleton().rollD20();
		if (probe == 1)
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20)
		{
			rval = RESULT_PATZER;
		}
		else
		{
			rval = getAttackeBasis() + (*it).second.first - (probe + modifier - eBe);
		}
		if (rval < 0) return RESULT_MISSERFOLG;
		else return RESULT_ERFOLG;
	}

	int Creature::doParade(const CeGuiString& kampftechnikName, int modifier)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it == mKampftechniken.end())
		{
			Throw(InvalidArgumentException, "kampftechnikName nicht in mKampftechniken gefunden");
		}
		int rval;
		int eBe = ceil(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getCurrentBe())) / 2.0);

		int probe = DsaManager::getSingleton().rollD20();
		if (probe == 1)
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20)
		{
			rval = RESULT_PATZER;
		}
		else
		{
			rval = getParadeBasis() + (*it).second.second - (probe + modifier - eBe);
		}
		if (rval < 0) return RESULT_MISSERFOLG;
		else return RESULT_ERFOLG;
	}

	int Creature::doInitiativeWurf(bool getMaxInitiave)
	{
		int rval = getInitiativeBasis();
		rval += getCurrentInitiativeModifier();
		rval -= getCurrentBe();
		if (getMaxInitiave) rval += 6;
		else rval += DsaManager::getSingleton().rollD6();
		return rval;
	}
}
