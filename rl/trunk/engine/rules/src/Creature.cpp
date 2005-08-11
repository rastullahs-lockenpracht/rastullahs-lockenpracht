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


#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"
#include "Talent.h"
#include "Eigenschaft.h"

namespace rl
{
    Creature::Creature(int id,
        const CeGuiString& name, const CeGuiString& description)
        : GameObject(id, name, description), mCurrentLe(0)
    {
		setWert(WERT_MOD_AE, 0);
		setWert(WERT_MOD_LE, 0);
		setWert(WERT_MOD_AT, 0);
		setWert(WERT_MOD_PA, 0);
		setWert(WERT_MOD_FK, 0);
		setWert(WERT_MOD_AU, 0);
		setWert(WERT_MOD_MR, 0);
		setWert(WERT_MOD_INI, 0);	
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

	void Creature::addTalent(int key)
	{
		TalentMap::const_iterator it = mTalente.find(key);
        if (it != mTalente.end())
        {
            Throw(InvalidArgumentException, "Talent nicht gefunden.");
        }
        
		mTalente[key] = 0;
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

	void Creature::addSe(int talentId)
	{
		///@todo Mit Code fuellen
	}

    int Creature::getSf(int key) const
    {
        SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(key);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(InvalidArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        return (*it).second;
    }

	void Creature::addSf(int key)
	{
		SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(key);
        if (it != mSonderfertigkeiten.end())
        {
            Throw(InvalidArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        
		mSonderfertigkeiten[key] = 0;
	}

    void Creature::setSf(int key, int value)
    {
		if (value < SF_MIN_VALUE || value > SF_MAX_VALUE)
		{
			Throw(OutOfRangeException, "Der Sonderfertigkeit soll ein" 
				"unzulässiger Wert zugewiesen werden");
		}
        SonderfertigkeitMap::iterator it = mSonderfertigkeiten.find(key);
        if (it == mTalente.end())
        {
            Throw(InvalidArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        (*it).second = value;
    }

    void Creature::modifyEigenschaft(int key, int mod)
    {
        mEigenschaften[key] = mEigenschaften[key] + mod;
		fireObjectStateChangeEvent();
    }

    void Creature::modifyTalent(int key, int mod)
    {
        TalentMap::iterator it = mTalente.find(key);
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

    int Creature::doAlternativeTalentprobe(int talentId, int spezialisierungId,
		int modifier, int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentId);
        EigenschaftTripel et(eigenschaft1Id, eigenschaft2Id, eigenschaft3Id);

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
			addSe(talentId);
			return RESULT_SPEKT_AUTOERFOLG;
		}
		if ( ((probe.first == 1) && (probe.second == 1)) || 
			 ((probe.first == 1) && (probe.third == 1)) ||
			 ((probe.second == 1) && (probe.third == 1))) 
		{
			addSe(talentId);
			return RESULT_AUTOERFOLG;
		}
		// Patzer
		if ((probe.first == 20) && (probe.second == 20) && (probe.third == 20)) 
		{
			addSe(talentId);
			return RESULT_SPEKT_AUTOMISSERFOLG; 
		}
		if ( ((probe.first == 20) && (probe.second == 20)) || 
			 ((probe.first == 20) && (probe.third == 20)) ||
			 ((probe.second == 20) && (probe.third == 20))) 
		{
			addSe(talentId);
			return RESULT_AUTOMISSERFOLG;
		}


        // Vor dem Vergleich hat man den Talentwert übrig.
		int taW = 0;
		try 
		{
			if (1 == getSf(spezialisierungId)) taW = 2;
		}
		catch(InvalidArgumentException){};
		taW += getTalent(talentId);
        int rval = taW - modifier;
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

    int Creature::doAlternativeTalentprobe(int talentId, int modifier, 
		int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id)
    {
		return doAlternativeTalentprobe(talentId, -1, modifier, eigenschaft1Id,
			eigenschaft2Id, eigenschaft3Id);
	}

    int Creature::doTalentprobe(int talentId, int spezialisierungId, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentId);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentId, spezialisierungId, modifier, 
			et.first, et. second, et.third);
	}

    int Creature::doTalentprobe(int talentId, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentId);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentId, -1, modifier, et.first, 
			et. second, et.third);
	}

    int Creature::doEigenschaftsprobe(int eigenschaftId, int modifier)
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
            rval = getEigenschaft(eigenschaftId) - (probe + modifier);
        }
        return rval;
    }

    pair<int, int> Creature::getKampftechnik(int kampftechnikId) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikId);
        if (it == mKampftechniken.end())
        {
            Throw(InvalidArgumentException, "Kampftechnik nicht gefunden.");
        }
        return (*it).second;
    }

    void Creature::setKampftechnik(int kampftechnikId, const pair<int, int>& value)
    {
        KampftechnikMap::iterator it = mKampftechniken.find(kampftechnikId);
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

	void Creature::setWert(int wertId, int wert)
	{
		WertMap::iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            mWerte.insert(make_pair(wertId, wert));
        }
        (*it).second = wert;
	}
}
