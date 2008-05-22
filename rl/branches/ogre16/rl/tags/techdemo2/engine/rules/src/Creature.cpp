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
#include "DsaManager.h"
#include "Eigenschaft.h"
#include "Exception.h"
#include "Kampftechnik.h"
#include "MeshObject.h"
#include "StateSet.h"
#include "Talent.h"
#include "Inventory.h"

namespace rl
{
    Creature::Creature(const CeGuiString name, const CeGuiString description)
        : GameObject(name, description), 
		mCurrentLe(NULL),
		mActiveWeapon(NULL),
		mInventory(NULL),
		mEigenschaften(),
		mWerte(),
		mTalente(),
        mKampftechniken(),
        mSonderfertigkeiten(),
        mContainer()
    {
		setWert(WERT_MOD_AE, 0);
		setWert(WERT_MOD_LE, 0);
		setWert(WERT_MOD_AT, 0);
		setWert(WERT_MOD_PA, 0);
		setWert(WERT_MOD_FK, 0);
		setWert(WERT_MOD_AU, 0);
		setWert(WERT_MOD_MR, 0);
		setWert(WERT_MOD_INI, 0);
		setWert(WERT_SOZIALSTATUS, 0);
		setWert(WERT_GS, 8);
		setWert(WERT_BE, 0);
		mEigenschaften[E_MUT] = new EigenschaftenStateSet();
		mEigenschaften[E_KLUGHEIT] = new EigenschaftenStateSet();
		mEigenschaften[E_INTUITION] = new EigenschaftenStateSet();
		mEigenschaften[E_CHARISMA] = new EigenschaftenStateSet();
		mEigenschaften[E_FINGERFERTIGKEIT] = new EigenschaftenStateSet();
		mEigenschaften[E_GEWANDTHEIT] = new EigenschaftenStateSet();
		mEigenschaften[E_KONSTITUTION] = new EigenschaftenStateSet();
		mEigenschaften[E_KOERPERKRAFT] = new EigenschaftenStateSet();

		mInventory = new Inventory(this);
    }

	Creature::~Creature()
    {
		if (mInventory){
			delete mInventory;
		}

        for( WertMap::iterator it=mWerte.begin();it!=mWerte.end(); it++ )
            delete it->second;
        mWerte.clear();
        for( EigenschaftMap::iterator it=mEigenschaften.begin();it!=mEigenschaften.end(); it++ )
            delete it->second;
        mEigenschaften.clear();
        for( TalentMap::iterator it=mTalente.begin();it!=mTalente.end(); it++ )
            delete it->second;
        mTalente.clear();
        mKampftechniken.clear();
        for( SonderfertigkeitMap::iterator it=mSonderfertigkeiten.begin();it!=mSonderfertigkeiten.end(); it++ )
            delete it->second;
        mSonderfertigkeiten.clear();
        for( ContainerMap::iterator it=mContainer.begin();it!=mContainer.end(); it++ )
            delete it->second;
        mContainer.clear();
    }

    int Creature::getAttackeBasis()
    {
        double es = getEigenschaftForBasiswertCalculation(E_MUT) +
            getEigenschaftForBasiswertCalculation(E_GEWANDTHEIT) +
            getEigenschaftForBasiswertCalculation(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getParadeBasis()
    {
        double es = getEigenschaftForBasiswertCalculation(E_INTUITION) +
            getEigenschaftForBasiswertCalculation(E_GEWANDTHEIT) +
            getEigenschaftForBasiswertCalculation(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getFernkampfBasis()
    {
        double es = getEigenschaftForBasiswertCalculation(E_INTUITION) +
            getEigenschaftForBasiswertCalculation(E_FINGERFERTIGKEIT) +
            getEigenschaftForBasiswertCalculation(E_KOERPERKRAFT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getInitiativeBasis()
    {
        int es = 2 * getEigenschaftForBasiswertCalculation(E_MUT) +
            getEigenschaftForBasiswertCalculation(E_INTUITION) +
            getEigenschaftForBasiswertCalculation(E_GEWANDTHEIT);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getMrBasis()
    {
        int es = getEigenschaftForBasiswertCalculation(E_MUT) +
            getEigenschaftForBasiswertCalculation(E_KLUGHEIT) +
            getEigenschaftForBasiswertCalculation(E_KONSTITUTION);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getLeBasis()
    {
        int es =  2 * getEigenschaftForBasiswertCalculation(E_KONSTITUTION) +
            getEigenschaftForBasiswertCalculation(E_KOERPERKRAFT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAuBasis()
    {
        int es = getEigenschaftForBasiswertCalculation(E_MUT) +
            getEigenschaftForBasiswertCalculation(E_KONSTITUTION) +
            getEigenschaftForBasiswertCalculation(E_GEWANDTHEIT);

        return static_cast<int>(es / 2.0 + 0.5);
    }

	int Creature::getAeBasis()
	{
        int es = getEigenschaftForBasiswertCalculation(E_MUT) +
			getEigenschaftForBasiswertCalculation(E_INTUITION) +
            getEigenschaftForBasiswertCalculation(E_CHARISMA);

        return static_cast<int>(es / 2.0 + 0.5);
    }

	int Creature::getWert(int wertId, bool getUnmodified)
	{
		checkEffects();
		WertMap::const_iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            Throw(IllegalArgumentException, "Wert nicht gefunden.");
        }
		return it->second->getValue(getUnmodified);
	}

	int Creature::getCurrentBe()
	{
		pair<int,int> be = mInventory->getOverallBe();
		return be.first+be.second;
	}

	void Creature::setWert(int wertId, int wert)
	{
		WertMap::iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
			StateSet* newWert = new StateSet();
			newWert->setOriginalValue(wert);
			mWerte.insert(make_pair(wertId, newWert));
        }
		else
		{
			it->second->setOriginalValue(wert);
		}
	}

	StateSet* Creature::getWertStateSet(int wertId)
	{
		checkEffects();
        WertMap::const_iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            Throw(IllegalArgumentException, "Wert nicht gefunden.");
        }
		return it->second;
	}

   void Creature::modifyLe(int mod, bool ignoreMax)
    {
        mCurrentLe += mod;
		if (!ignoreMax)
			mCurrentLe = min(mCurrentLe, getLeMax());
		if (mCurrentLe <= 0)
		{
			MeshObject* mo = dynamic_cast<MeshObject*>(getActor()->getControlledObject());
			mo->stopAllAnimations();
			mo->startAnimation("Niederschlag/Sturz", 1.0f, 1);
		}
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

    int Creature::getEigenschaft(const CeGuiString eigenschaftName)
    {
		checkEffects();
		EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
		if (it == mEigenschaften.end())
		{
			Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
		}
		return it->second->getValue();
    }

    int Creature::getEigenschaftForBasiswertCalculation(const CeGuiString eigenschaftName)
    {
		checkEffects();
		EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
		if (it == mEigenschaften.end())
		{
			Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
		}
		return it->second->getValueForBasiswertCalculation();
    }

    void Creature::setEigenschaft(const CeGuiString eigenschaftName, int value)
    {
        EigenschaftMap::iterator it = mEigenschaften.find(eigenschaftName);
        if (it == mEigenschaften.end())
        {
            Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
        }
		it->second->setOriginalValue( value );
		fireObjectStateChangeEvent();
    }

    void Creature::modifyEigenschaft(const CeGuiString eigenschaftName, int mod)
    {
		mEigenschaften[eigenschaftName]->setOriginalValue( mEigenschaften[eigenschaftName]->getOriginalValue() + mod );
		fireObjectStateChangeEvent();
    }

	EigenschaftenStateSet* Creature::getEigenschaftenStateSet(const CeGuiString eigenschaftName)
	{
		checkEffects();
        EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
        if (it == mEigenschaften.end())
        {
            Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
        }
		return it->second;
	}

    int Creature::getTalent(const CeGuiString talentName)
    {
		checkEffects();
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		return it->second->getValue();
    }

	void Creature::addTalent(const CeGuiString talentName, int value)
	{
		TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
			Throw(IllegalArgumentException, "Talent schon in mTalente enthalten.");
        }
	    DsaManager::getSingleton().getTalent(talentName); //ueberpruefe ob es das Talent ueberhaupt gibt
		mTalente[talentName] = new TalentStateSet();
		mTalente[talentName]->setOriginalValue(value);
		fireObjectStateChangeEvent();
	}

    void Creature::modifyTalent(const CeGuiString talentName, int mod)
    {
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		it->second->setOriginalValue( it->second->getOriginalValue() + mod );
		fireObjectStateChangeEvent();
    }

	const Creature::TalentMap& Creature::getAllTalents() const
	{
		return mTalente;
	}

    void Creature::setTalent(const CeGuiString talentName, int value)
    {
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		it->second->setOriginalValue( value );
		fireObjectStateChangeEvent();
    }

	void Creature::addSe(const CeGuiString talentName)
	{
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		it->second->setSe( true );
	}

	TalentStateSet* Creature::getTalentStateSet(const CeGuiString talentName)
	{
		checkEffects();
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		return it->second;
	}

	void Creature::addKampftechnik(const CeGuiString kampftechnikName, const pair<int,int>& value)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it != mKampftechniken.end())
		{
			Throw(IllegalArgumentException, 
				"Kampftechnik schon in mKampftechniken enthalten.");
		}
		//ueberpruefe ob es die Kampftechnik ueberhaupt gibt
		DsaManager::getSingleton().getKampftechnik(kampftechnikName); 
		mKampftechniken[kampftechnikName] = value;
		fireObjectStateChangeEvent();
	}

    pair<int, int> Creature::getKampftechnik(const CeGuiString kampftechnikName) const
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
        if (it == mKampftechniken.end())
        {
            Throw(IllegalArgumentException, "Kampftechnik nicht gefunden.");
        }
        return (*it).second;
    }

    void Creature::setKampftechnik(const CeGuiString kampftechnikName, const pair<int, int>& value)
    {
        KampftechnikMap::iterator it = mKampftechniken.find(kampftechnikName);
        if (it == mKampftechniken.end())
        {
            Throw(IllegalArgumentException, "Kampftechnik nicht gefunden.");
        }
        (*it).second = value;
		fireObjectStateChangeEvent();
    }

    int Creature::getSf(const CeGuiString sfName)
    {
		checkEffects();
        SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(IllegalArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
		return it->second->getValue();
    }

	void Creature::addSf(const CeGuiString sfName, int value)
	{
		SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
		if (it != mSonderfertigkeiten.end())
		{
			Throw(IllegalArgumentException, "Sonderfertigkeit schon in mSonderfertigkeiten enthalten.");
		}
		//ueberpruefe ob es die Sonderfertigkeit ueberhaupt gib
		DsaManager::getSingleton().getTalent(sfName);
		mSonderfertigkeiten[sfName] = new SonderfertigkeitenStateSet();
		mSonderfertigkeiten[sfName]->setOriginalValue( value );
		fireObjectStateChangeEvent();
	}

    void Creature::setSf(const CeGuiString sfName, int value)
    {
		if (value < SF_MIN_VALUE || value > SF_MAX_VALUE)
		{
			Throw(OutOfRangeException, "Der Sonderfertigkeit soll ein" 
				"unzulässiger Wert zugewiesen werden");
		}
        SonderfertigkeitMap::iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(IllegalArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
		it->second->setOriginalValue( value );
		fireObjectStateChangeEvent();
    }

	SonderfertigkeitenStateSet* Creature::getSonderfertigkeitenStateSet(const CeGuiString sfName)
	{
		checkEffects();
        SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(IllegalArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
		return it->second;
	}

    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, int spezialisierungId,
		int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
		if (((talent->getArt() == TALENT_ART_BASIS) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_BASIS)) || 
			((talent->getArt() == TALENT_ART_SPEZIAL) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_SPEZIAL)) || 
			((talent->getArt() == TALENT_ART_BERUF) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_BERUF)))
		{
			Throw(OutOfRangeException, "TaW zu niedrig");
		}
        EigenschaftTripel et(eigenschaft1Name, eigenschaft2Name, eigenschaft3Name);

        // Der Probenwurf
        Tripel<int> probe(DsaManager::getSingleton().roll3D20());

		// Glueckliche
		if ( (probe.first == 1) && (probe.second == 1) && (probe.third == 1) ) 
		{
			// BasisBoxS. 72 Der Wert wird sofort um 1 angehoben
			modifyTalent(talentName, +1); 
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
		int eBe = DsaManager::getSingleton().getTalent(talentName)->calculateEbe(getWert(WERT_BE));
		try 
		{
			//if (1 == getSf(sfName)) taW = 2; //Spezialisiereung?
		}
		catch(IllegalArgumentException){};
		taW += getTalent(talentName);
		int rval = taW - modifier - getTalentStateSet(talentName)->getProbenModifier()
			- getWertStateSet(WERT_MOD_ALL_TALENTPROBEN)->getProbenModifier() - eBe;
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

    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, int modifier, 
		CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
		return doAlternativeTalentprobe(talentName, -1, modifier, eigenschaft1Name,
			eigenschaft2Name, eigenschaft3Name);
	}

    int Creature::doTalentprobe(const CeGuiString talentName, int spezialisierungId, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, spezialisierungId, modifier, 
			et.first, et. second, et.third);
	}

    int Creature::doTalentprobe(const CeGuiString talentName, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, -1, modifier, et.first, 
			et. second, et.third);
	}

    int Creature::doEigenschaftsprobe(const CeGuiString eigenschaftName, int modifier)
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
			rval = getEigenschaft(eigenschaftName) - 
				(probe + modifier + getEigenschaftenStateSet(eigenschaftName)->getProbenModifier() 
				+ getWertStateSet(WERT_MOD_ALL_EIGENSCHAFTSPROBEN)->getProbenModifier());
        }
        return rval;
    }

	
	void Creature::addContainer(Container* container)
	{
		if (container == NULL)
			Throw(IllegalArgumentException, "Nullpointer uebergeben");
		ContainerMap::const_iterator it = mContainer.find(container->getName());
		if (it != mContainer.end())
		{
			Throw(IllegalArgumentException, 
				"Container bereits in mContainer enthalten.");
		}
		mContainer.insert(make_pair(container->getName(), container));
	}

	Container* Creature::getContainer(const CeGuiString containerName) const
	{
		ContainerMap::const_iterator it = mContainer.find(containerName);
		if (it == mContainer.end())
		{
			Throw(IllegalArgumentException, "Container nicht in mContainer gefunden.");
		}
		return (*it).second;	
	}

	Container* Creature::removeContainer(const CeGuiString containerName)
	{
		ContainerMap::iterator it = mContainer.find(containerName);
		if (it == mContainer.end())
		{
			Throw(IllegalArgumentException, "Container nicht in mContainer gefunden.");
		}
		Container* rval = (*it).second;
		mContainer.erase(it);
		return rval;
	}
	

	Inventory* Creature::getInventory()
	{
		return mInventory;
	}
	
	void Creature::addWeapon(Weapon* weapon)
	{
		if (weapon == NULL)
			Throw(IllegalArgumentException, "Creature::addWeapon : Nullpointer uebergeben!");
		mInventory->addWeapon(weapon);
	}

	Weapon* Creature::getWeapon(int weaponId) const
	{
		return mInventory->getWeapon(weaponId);
	}

	Weapon* Creature::removeWeapon(int weaponId)
	{
		return mInventory->removeWeapon(weaponId);
	}

	void Creature::switchToWeapon(int weaponId)
	{
		mInventory->switchToWeapon(weaponId);
	}

	void Creature::attachWeapon(Weapon* weapon)
	{
		// FIXME: Waffenknochen immer 13?
		getActor()->attachToSlot(
			weapon->getActor(), 
			"r_hand",
			"SLOT_HANDLE"); 
		 mActiveWeapon = weapon;
	}
	

	void Creature::detachWeapon(){
		if (mActiveWeapon != NULL)
		{
			getActor()->detach(mActiveWeapon->getActor());
			mActiveWeapon = NULL;
		}
	}

	Weapon* Creature::getActiveWeapon()
	{
		return mActiveWeapon;
	}

	int Creature::doAttacke(const CeGuiString kampftechnikName, int modifier)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it == mKampftechniken.end())
		{
			Throw(IllegalArgumentException, "kampftechnikName nicht in mKampftechniken gefunden");
		}
		int rval;
		int eBe = floor(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

		int probe = DsaManager::getSingleton().rollD20();
		if (probe == 1) // TODO: Bestätigen
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20) // TODO: Bestätigen
		{
			rval = RESULT_PATZER;
		}
		else
		{
			rval = getAttackeBasis() + (*it).second.first - (probe + modifier - eBe);
		}
		if (rval < 0) 
			return RESULT_MISSERFOLG;
		else 
			return RESULT_ERFOLG;
	}

	int Creature::doParade(const CeGuiString kampftechnikName, int modifier, bool guteParade)
	{
		KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
		if (it == mKampftechniken.end())
		{
			Throw(IllegalArgumentException, "kampftechnikName nicht in mKampftechniken gefunden");
		}
		int rval;
		int eBe = ceil(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

		int probe = DsaManager::getSingleton().rollD20();
		if (probe == 1) // TODO: Bestätigen
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20)
		{
			rval = RESULT_PATZER; // TODO: Bestätigen
		}
		else
		{
			// TODO: Gute Parade
			rval = getParadeBasis() + (*it).second.second - (probe + modifier - eBe);
		}
		if (rval < 0) 
			return RESULT_MISSERFOLG;
		else 
			return RESULT_ERFOLG;
	}

	int Creature::doInitiativeWurf(bool getMaxInitiave)
	{
		int rval = getInitiativeBasis();
		rval += getWert(WERT_MOD_INI);
		rval -= getWert(WERT_BE);
		if (getMaxInitiave) rval += 6;
		else rval += DsaManager::getSingleton().rollD6();
		return rval;
	}

	/** @TODO: Implement correctly */
	void Creature::applyDamage(int sp, Weapon* weapon)
	{
		modifyLe(-sp);
	}

	void Creature::addEffect(Effect* effect)
	{
		effect->setOwner(this);
		mEffectManager.addEffect(effect);
	}

	void Creature::checkEffects()
	{
		//TODO
		// Nur einmal pro Aktion ausfuehren
		mEffectManager.checkEffects();
	}
}
