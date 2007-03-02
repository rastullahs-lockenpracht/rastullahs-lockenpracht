/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "Actor.h"
#include "Container.h"
#include "DsaManager.h"
#include "Eigenschaft.h"
#include "Exception.h"
#include "Inventory.h"
#include "Kampftechnik.h"
#include "MeshObject.h"
#include "StateSet.h"
#include "Talent.h"
#include "Weapon.h"

///@todo Just for debugging, remove when not needed anymore
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"

using namespace std;

namespace rl
{
    const Ogre::String Creature::CLASS_NAME = "Creature";

    Creature::Creature(unsigned int id)
        : GameObject(id), 
		mCurrentLe(0),
        mCurrentAu(0),
        mCurrentAe(0),
	mActiveWeapon(NULL),
		mInventory(NULL),
		mEigenschaften(),
		mWerte(),
		mTalente(),
        mKampftechniken(),
        mSonderfertigkeiten(),
        mErschoepfung(0),
        mBlind(0),
        mDead(0),
        mDeaf(0),
        mIncapacitated(0),
        mInvulnerable(0),
        mInvisible(0),
        mParalyzed(0),
        mSilenced(0),
        mSleeping(0),
        mUnconscious(0)
    {
        mQueryFlags = QUERYFLAG_CREATURE;

		//RlFail("Test");
		setWert(WERT_MOD_AE, 0);
		setWert(WERT_MOD_LE, 0);
		setWert(WERT_MOD_AT, 0);
		setWert(WERT_MOD_PA, 0);
		setWert(WERT_MOD_FK, 0);
		setWert(WERT_MOD_AU, 0);
		setWert(WERT_MOD_MR, 0);
		setWert(WERT_MOD_INI, 0);
		setWert(WERT_SOZIALSTATUS, 0);
        setWert(WERT_MOD_REGENERATION_LE, 0);
        setWert(WERT_MOD_REGENERATION_AE, 0);
        setWert(WERT_MOD_ERSCHOEPFUNGSSCHWELLE, 0);
        setWert(WERT_MOD_ALL_EIGENSCHAFTSPROBEN, 0);
        setWert(WERT_MOD_ALL_TALENTPROBEN, 0);
		setWert(WERT_GS, 8);
		setWert(WERT_BE, 0);
        setWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE, 0);
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

	int Creature::getWert(Wert wertId, bool getUnmodified)
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
        ///@todo Ruestungsgewoehnung?
		return be.first+be.second;
	}

	void Creature::setWert(Wert wertId, int value)
	{
		WertMap::iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
			StateSet* newWert = new StateSet();
			newWert->setOriginalValue(value);
			mWerte.insert(make_pair(wertId, newWert));
        }
		else
		{
			it->second->setOriginalValue(value);
		}
	}

	StateSet* Creature::getWertStateSet(Wert wertId)
	{
		checkEffects();
        WertMap::const_iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            Throw(IllegalArgumentException, 
                "Wert "+Ogre::StringConverter::toString(wertId)+" not found.");
        }
		return it->second;
	}

   void Creature::modifyLe(int mod, bool ignoreMax)
    {
        int oldLe = mCurrentLe;
        mCurrentLe += mod;
		if (!ignoreMax)
			mCurrentLe = min(mCurrentLe, getLeMax());
		if (mCurrentLe <= getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE) &&
            oldLe > getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE))
		{
            setIncapacitated(true);
			MeshObject* mo = static_cast<MeshObject*>(getActor()->getControlledObject());
			mo->stopAllAnimations();
            ///@todo Sturzanimation aufrufen, sobald sie verfuegbar ist.
			//mo->startAnimation("Niederschlag/Sturz", 1.0f, 1);
		}
        if (mCurrentLe <= 0 &&
            oldLe > 0)
        {
            setUnconscious(true);
        }
        if (mCurrentLe <= -getEigenschaft("KO") &&
            oldLe > -getEigenschaft("KO"))
        {
            setDead(true);
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

    void Creature::modifyAu(int mod, bool ignoreMax)
    {
        mCurrentAu = max(mCurrentAu + mod, 0);
		if (!ignoreMax)
			mCurrentAu = min(mCurrentAu, getAuMax());
        if (getAu() <= 0)
        {
            setIncapacitated(true);
        }
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

    void Creature::modifyAp(int modifier)
    {
        mAp.total += modifier;
    }
    
    int Creature::getAp()
    {
        return mAp.total;
    }
    
    void Creature::modifyUsedAp(int modifier)
    {
        mAp.used += modifier;
    }
    
    int Creature::getUsedAp()
    {
        return mAp.used;
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
            Talent::AusweichTalente ausweichTalente;
            ausweichTalente = 
                DsaManager::getSingleton().getTalent(talentName)->getAusweichTalente();
            Talent::AusweichTalente::const_iterator ausweichIt = ausweichTalente.begin();
            if (ausweichIt == ausweichTalente.end())
            {
                Throw(IllegalArgumentException, "Talent nicht gefunden.");
            }
            int rval = getTalent(ausweichIt->first) - ausweichIt->second;
            for (ausweichIt++; ausweichIt != ausweichTalente.end(); ausweichIt++)
            {
                int tempAusweichTaw = getTalent(ausweichIt->first) 
                    - ausweichIt->second;
                if (tempAusweichTaw > rval) rval = tempAusweichTaw;
            }
            return rval;
        } //if (it == mTalente.end())
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

    void Creature::addVorteil(const CeGuiString vorteilName, int value)
    {
        VorteilMap::const_iterator it = mVorteile.find(vorteilName);
        if (it != mVorteile.end())
        {
            Throw(IllegalArgumentException, "Vorteil schon in mVorteile enthalten");
        }
        mVorteile[vorteilName] = new TalentStateSet();
        mVorteile[vorteilName]->setOriginalValue( value );
    }

    bool Creature::hasVorteil(const CeGuiString vorteilName)
    {
        VorteilMap::const_iterator it = mVorteile.find(vorteilName);
        if (it != mVorteile.end())
        {
            return true;
        }
        else return false;
    }

    TalentStateSet* Creature::getVorteilStateSet(const CeGuiString vorteilName)
    {
		checkEffects();
        VorteilMap::const_iterator it = mVorteile.find(vorteilName);
        if (it == mVorteile.end())
        {
            Throw(IllegalArgumentException, "Vorteil nicht gefunden.");
        }
		else return it->second;
    }

    void Creature::addNachteil(const CeGuiString nachteilName, int value)
    {
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it != mNachteile.end())
        {
            Throw(IllegalArgumentException, "Nachteil schon in mNachteile enthalten");
        }
        mNachteile[nachteilName] = new EigenschaftenStateSet();
        mNachteile[nachteilName]->setOriginalValue( value );
    }

    bool Creature::hasNachteil(const CeGuiString nachteilName)
    {
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it != mNachteile.end())
        {
            return true;
        }
        else return false;
    }

    EigenschaftenStateSet* Creature::getNachteilStateSet(const CeGuiString nachteilName)
    {
		checkEffects();
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it == mNachteile.end())
        {
            Throw(IllegalArgumentException, "Nachteil nicht gefunden.");
        }
		else return it->second;
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

	void Creature::addSf(const CeGuiString sfName, SfStatus value)
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

    void Creature::setSf(const CeGuiString sfName, SfStatus value)
    {
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

    bool Creature::isBlind()
    {
        if (mBlind > 0) return true;
        else return false;
    }

    bool Creature::isDead()
    {
        if (mDead > 0) return true;
        else return false;
    }

    bool Creature::isDeaf()
    {
        if (mDeaf > 0) return true;
        else return false;
    }

    bool Creature::isIncapacitated()
    {
        if (mIncapacitated > 0) return true;
        else return false;
    }

    bool Creature::isInvulnerable()
    {
        if (mInvulnerable > 0) return true;
        else return false;
    }

    bool Creature::isInvisible()
    {
        if (mInvisible > 0) return true;
        else return false;
    }

    bool Creature::isParalyzed()
    {
        if (mParalyzed > 0) return true;
        else return false;
    }

    bool Creature::isSilenced()
    {
        if (mSilenced > 0) return true;
        else return false;
    }

    bool Creature::isSleeping()
    {
        if (mSleeping > 0) return true;
        else return false;
    }

    bool Creature::isUnconscious()
    {
        if (mUnconscious > 0) return true;
        else return false;
    }

    bool Creature::isImmovable()
    {
        if (isDead() ||
            isParalyzed() ||
            isSleeping() ||
            isUnconscious())
            return true;
        else
            return false;
    }

	bool Creature::isMagic()
	{
		return getWert(WERT_MOD_AE) > 0;
	}

    void Creature::setStatus(int& statusVariable, bool value, const Ogre::String& errorMessage)
    {
        if (value == true)
        {
            statusVariable++;
        }
        else //value == false
        {
            if (statusVariable > 0)
            {
                statusVariable--;
            }
            else
            {
                LOG_MESSAGE(Logger::RULES, errorMessage);
            }
        } //value == false
    }

    void Creature::setBlind(bool value)
    {
        setStatus(mBlind, value, 
            "Trying to remove blindness from a seeing creature.");
    }

    
    void Creature::setDead(bool value)
    {
        setStatus(mDead, value, 
            "Trying to revive a living creature.");
    }
    
    void Creature::setDeaf(bool value)
    {
        setStatus(mDeaf, value, 
            "Trying to remove deafness from a hearing creature.");
    }
    
    void Creature::setIncapacitated(bool value)
    {
        setStatus(mIncapacitated, value,
            "Trying to remove incapacitation from a non incapacitated "
            "creature.");
    }

    void Creature::setInvisible(bool value)
    {
        setStatus(mInvisible, value,
            "Trying to remove Invisibility from a visible creature.");
    }
    
    void Creature::setInvulnerable(bool value)
    {
        setStatus(mInvulnerable, value,
            "Trying to remove Invulnerability from a vulnerable creature.");
    }
    
    void Creature::setParalyzed(bool value)
    {
        setStatus(mParalyzed, value,
            "Trying to remove paralyzation from a non paralyzed creature.");
    }
    
    void Creature::setSilenced(bool value)
    {
        setStatus(mSilenced, value,
            "Trying to remove silence from a non silenced creature.");
    }
    
    void Creature::setSleeping(bool value)
    {
        setStatus(mSleeping, value,
            "Trying to awake a wake creature.");
    }

    void Creature::setUnconscious(bool value)
    {
        setStatus(mUnconscious, value,
            "Trying to awake a conscious creature.");
    }

    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, int spezialisierungId,
		int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
		if (((talent->getArt() == TALENT_ART_SPEZIAL) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_SPEZIAL)) || 
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

	
	Inventory* Creature::getInventory()
	{
		return mInventory;
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
		if (probe == 1) /// @todo Bestätigen
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20) /// @todo Bestätigen
		{
			rval = RESULT_PATZER;
		}
		else
		{
			rval = getAttackeBasis() + (*it).second.first - (probe + modifier 
				+ getWertStateSet(WERT_MOD_ALL_EIGENSCHAFTSPROBEN)->getProbenModifier() + eBe);
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
		if (probe == 1) /// @todo Bestätigen
		{
			rval = RESULT_GLUECKLICH;
		}
		else if (probe == 20)
		{
			rval = RESULT_PATZER; /// @todo Bestätigen
		}
		else
		{
			/// @todo Gute Parade
			rval = getParadeBasis() + (*it).second.second - (probe + modifier 
				+ getWertStateSet(WERT_MOD_ALL_EIGENSCHAFTSPROBEN)->getProbenModifier() + eBe);
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
		if (getMaxInitiave) 
        {
            rval += 6;
        }
        else
        {
            rval += DsaManager::getSingleton().rollD6();
        }
		return rval;
	}

	/// @todo Implement correctly
	void Creature::damageLe(int tp, int damageType)
	{
        /**@todo Was tun bei negativen TP? Exception? Fehlermeldung? Stillschweigend
             auf 0 setzen?*/
        if (tp < 0) 
        {
            tp = 0;
        }
        ///@todo auf Verletzlichkeiten und Immunitaeten achten
        if ((damageType&LEDAMAGE_FIRE) == LEDAMAGE_FIRE) CoreSubsystem::getSingleton().getRubyInterpreter()->execute("p \"Fire!\"");
        if ((damageType&LEDAMAGE_WATER) == LEDAMAGE_WATER) CoreSubsystem::getSingleton().getRubyInterpreter()->execute("p \"Water!\"");
        if ((damageType&LEDAMAGE_DEMONIC) == LEDAMAGE_DEMONIC) CoreSubsystem::getSingleton().getRubyInterpreter()->execute("p \"Demons!\"");
        if ((damageType&LEDAMAGE_TP_A) == LEDAMAGE_TP_A)
        {
            damageAu(tp, AUDAMAGE_NORMAL);
            tp = floor(tp/2.);
        }
		modifyLe(-tp);
	}

    void Creature::damageAe(int asp)
    {
        if (asp < 0)
        {
         /**@todo Was tun bei negativen AsP? Exception? Fehlermeldung? Stillschweigend
             auf 0 setzen?*/
           asp = 0;
        }
        modifyAe(-asp);
    }

    void Creature::damageAu(int aup, int damageType)
    {
        if (aup <= 0)
        {
         /**@todo Was tun bei negativen AuP? Exception? Fehlermeldung? Stillschweigend
             auf 0 setzen?*/
           aup = 0;
           ///@todo evtl. eine modifyErschoepfung()?
           mErschoepfung += DsaManager::getSingleton().rollD6();
           setIncapacitated(true);
        }
        modifyAu(-aup);
    }

    void Creature::regenerateLe(int modifier)
    {
        //Grundregeneration von 1W6
        int regeneratedLe = DsaManager::getSingleton().rollD6();
        //Addiere eventuelle Modifikatoren hinzu
        regeneratedLe += getWert(WERT_MOD_REGENERATION_LE);
        //Bei gelungener KO Probe addiere 1
        if (RESULT_ERFOLG <= doEigenschaftsprobe("KO", 
            getWertStateSet(WERT_MOD_REGENERATION_LE)->getProbenModifier()))
        {
            regeneratedLe++;
        }
        //modifiziere die aktuellen LE
        modifyLe(regeneratedLe);
    }

    void Creature::regenerateAe(int modifier)
    {
        //Grundregeneration von 1W6
        int regeneratedAe = DsaManager::getSingleton().rollD6();
        //Addiere eventuelle Modifikatoren hinzu
        regeneratedAe += getWert(WERT_MOD_REGENERATION_AE);
        //Bei gelungener KO Probe addiere 1
        if (RESULT_ERFOLG <= doEigenschaftsprobe("IN", 
            getWertStateSet(WERT_MOD_REGENERATION_AE)->getProbenModifier()))
        {
            regeneratedAe++;
        }
        //modifiziere die aktuellen AE
        modifyAe(regeneratedAe);
    }

    void Creature::regenerateAu(int modifier)
    {
        ///@todo Gibt es etwas das die Regeneration permanent modifiziert?
        //Grundregeneration von 3W6
        int regeneratedAu = DsaManager::getSingleton().rollD6()
            + DsaManager::getSingleton().rollD6() 
            + DsaManager::getSingleton().rollD6();
        //Addiere eventuelle Modifikatoren hinzu
        //regeneratedAu += getWert(WERT_MOD_REGENERATION_LE);
        //Bei gelungener KO Probe addiere 1
        if (RESULT_ERFOLG <= doEigenschaftsprobe("KO", 
            getWertStateSet(WERT_MOD_REGENERATION_LE)->getProbenModifier()))
        {
            regeneratedAu += 6;
        }
        //modifiziere die aktuellen AU
        modifyAu(regeneratedAu);
    }

	void Creature::addEffect(Effect* effect)
	{
		effect->setOwner(this);
		mEffectManager.addEffect(effect);
	}

	void Creature::checkEffects()
	{
		/// @todo Nur einmal pro Aktion ausfuehren
		mEffectManager.checkEffects();
	}

    void Creature::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        //if (key == Creature::PROPERTY_)
        //{
        //    mImageName = value.toString();
        //}
        //else
        //{
            GameObject::setProperty(key, value);
        //}
    }

    const Property Creature::getProperty(const Ogre::String &key) const
    {
        //if (key == Item::PROPERTY_IMAGENAME)
        //{
        //    return Property(mImageName);
        //}
        //else if (key == Item::PROPERTY_SIZE)
        //{
        //    return Property(mSize);
        //}
        //else
        //{
            return GameObject::getProperty(key);
        //}
    }

    PropertySet* Creature::getAllProperties() const
    {
        PropertySet* ps = GameObject::getAllProperties();
        //ps->setProperty(Item::PROPERTY_IMAGENAME, Property(mImageName));
        //ps->setProperty(Item::PROPERTY_SIZE, Property(mSize));

        return ps;
    }
}
