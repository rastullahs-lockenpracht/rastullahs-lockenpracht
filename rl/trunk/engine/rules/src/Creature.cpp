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
#include "EffectManager.h"
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
    const Ogre::String Creature::PROPERTY_BEHAVIOURS = "behaviours";

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
        mMovementType(0)
    {
        mEffectManager = new EffectManager();
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
		setWert(WERT_GS, 8);
		setWert(WERT_BE, 0);
        setWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE, 0);
		mEigenschaften[E_MUT] = 0;
		mEigenschaften[E_KLUGHEIT] = 0;
		mEigenschaften[E_INTUITION] = 0;
		mEigenschaften[E_CHARISMA] = 0;
		mEigenschaften[E_FINGERFERTIGKEIT] = 0;
		mEigenschaften[E_GEWANDTHEIT] = 0;
		mEigenschaften[E_KONSTITUTION] = 0;
		mEigenschaften[E_KOERPERKRAFT] = 0;

		mInventory = new Inventory(this);
    }

	Creature::~Creature()
    {
        delete mInventory;
	delete mEffectManager;

        for( WertMap::iterator it=mWerte.begin();it!=mWerte.end(); it++ )
            delete it->second;
        mWerte.clear();
        mKampftechniken.clear();
        for( SonderfertigkeitMap::iterator it=mSonderfertigkeiten.begin();it!=mSonderfertigkeiten.end(); it++ )
            delete it->second;
        mSonderfertigkeiten.clear();
    }

    int Creature::getAttackeBasis()
    {
        double es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE) +
            getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE) +
              getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getParadeBasis()
    {
      double es = getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE) +
            getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE) +
            getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getFernkampfBasis()
    {
      double es = getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE) +
            getEigenschaft(E_FINGERFERTIGKEIT, Effect::MODTAG_RECALCULATE) +
            getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getInitiativeBasis()
    {
      int es = 2 * getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getMrBasis()
    {
      int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_KLUGHEIT, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getLeBasis()
    {
      int es =  2 * getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAuBasis()
    {
      int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 2.0 + 0.5);
    }

	int Creature::getAeBasis()
	{
      int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE) +
          getEigenschaft(E_CHARISMA, Effect::MODTAG_RECALCULATE);

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
            ///@todo set incapacitated.
			MeshObject* mo = static_cast<MeshObject*>(getActor()->getControlledObject());
			mo->stopAllAnimations();
            ///@todo Sturzanimation aufrufen, sobald sie verfuegbar ist.
			//mo->startAnimation("Niederschlag/Sturz", 1.0f, 1);
		}
        if (mCurrentLe <= 0 &&
            oldLe > 0)
        {
            ///@todo set unconscious
        }
        if (mCurrentLe <= -getEigenschaft("KO") &&
            oldLe > -getEigenschaft("KO"))
        {
            ///@todo set dead
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

    void Creature::modifyAu(float mod, bool ignoreMax)
    {
        mCurrentAu = max(mCurrentAu + mod, 0.0f);
		if (!ignoreMax)
			mCurrentAu = min(mCurrentAu, float(getAuMax()));
        if (getAu() <= 0)
        {
            ///@todo set incapacitated
        }
		fireObjectStateChangeEvent();
    }

    float Creature::getAu()
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

    int Creature::getEigenschaft(const CeGuiString eigenschaftName, Effect::ModTag tag)
    {
		checkEffects();
		EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
		if (it == mEigenschaften.end())
		{
			Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
		}
		int result = it->second;
        result += mEffectManager->getMod(eigenschaftName, Effect::MODTYPE_WERTMOD, tag);
        return result;
    }

    void Creature::setEigenschaft(const CeGuiString eigenschaftName, int value)
    {
        EigenschaftMap::iterator it = mEigenschaften.find(eigenschaftName);
        if (it == mEigenschaften.end())
        {
            Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
        }
		it->second = value;
        fireObjectStateChangeEvent();
    }

    void Creature::modifyEigenschaft(const CeGuiString eigenschaftName, int mod)
    {
		mEigenschaften[eigenschaftName] += mod ;
		fireObjectStateChangeEvent();
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
		return it->second;
    }

	void Creature::addTalent(const CeGuiString talentName, int value)
	{
		TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
			Throw(IllegalArgumentException, "Talent schon in mTalente enthalten.");
        }
	    DsaManager::getSingleton().getTalent(talentName); //ueberpruefe ob es das Talent ueberhaupt gibt
		mTalente[talentName] = value;
		fireObjectStateChangeEvent();
	}

    void Creature::modifyTalent(const CeGuiString talentName, int mod)
    {
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		it->second += mod ;
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
		it->second = value;
		fireObjectStateChangeEvent();
    }

	void Creature::addSe(const CeGuiString talentName)
	{
        TalentMap::iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent nicht gefunden.");
        }
		//it->second->setSe( true );
        /// @todo Implement SEs
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
        mVorteile[vorteilName] = value;
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

    void Creature::addNachteil(const CeGuiString nachteilName, int value)
    {
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it != mNachteile.end())
        {
            Throw(IllegalArgumentException, "Nachteil schon in mNachteile enthalten");
        }
        mNachteile[nachteilName] = value;
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

    int Creature::getSchlechteEigenschaft(const CeGuiString nachteilName)
    {
		checkEffects();
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it == mNachteile.end())
        {
            Throw(IllegalArgumentException, "Nachteil nicht gefunden.");
        }
		else return it->second + mEffectManager->getMod(nachteilName, Effect::MODTYPE_WERTMOD);
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

    Effect::Status Creature::getStatus()
    {
      return mEffectManager->getStatus();
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


    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, Effect::ModTag spezialisierung,
		int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
		if (((talent->getArt() == TALENT_ART_SPEZIAL) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_SPEZIAL)) || 
			((talent->getArt() == TALENT_ART_BERUF) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_BERUF)))
		{
          /// @todo Find proper return value. Prompt an error message?
			//Throw(OutOfRangeException, "TaW zu niedrig");
          return -1;
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
		int eBe = DsaManager::getSingleton().getTalent(talentName)->calculateEbe(getWert(WERT_BE));
		int taW = getTalent(talentName);
		int rval = taW - modifier - mEffectManager->getMod(talentName, Effect::MODTYPE_PROBENMOD, spezialisierung)
            - mEffectManager->getMod(Effect::ALL_TALENTE, Effect::MODTYPE_PROBENMOD, spezialisierung) - eBe;
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
		return doAlternativeTalentprobe(talentName, Effect::MODTAG_NONE, modifier, eigenschaft1Name,
			eigenschaft2Name, eigenschaft3Name);
	}

    int Creature::doTalentprobe(const CeGuiString talentName, Effect::ModTag spezialisierung, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, spezialisierung, modifier, 
			et.first, et. second, et.third);
	}

    int Creature::doTalentprobe(const CeGuiString talentName, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
		return doAlternativeTalentprobe(talentName, Effect::MODTAG_NONE, modifier, et.first, 
			et. second, et.third);
	}

    int Creature::doEigenschaftsprobe(const CeGuiString eigenschaftName, int modifier, Effect::ModTag tag)
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
                (probe + modifier + mEffectManager->getMod(eigenschaftName, Effect::MODTYPE_PROBENMOD, tag) + mEffectManager->getMod(Effect::ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD, tag));
        }
        return rval;
    }

	
	Inventory* Creature::getInventory() const
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
		int eBe = (int)floor(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

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
                + mEffectManager->getMod(Effect::ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD) + eBe);
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
		int eBe = (int)ceil(float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

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
                + mEffectManager->getMod(Effect::ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD) + eBe);
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
            tp = (int)floor(tp/2.);
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

    void Creature::damageAu(float aup, int damageType)
    {
        if (aup < 0)
        {
         /**@todo Was tun bei negativen AuP? Exception? Fehlermeldung? Stillschweigend
             auf 0 setzen?*/
           aup = 0;
           ///@todo evtl. eine modifyErschoepfung()?
           mErschoepfung += DsaManager::getSingleton().rollD6();
           ///@todo set incapacitated
        }
        // bei Überanstrengung, kostet alles doppelte Au!!!!!
        if( mErschoepfung > getEigenschaft("KO") )
            aup *= 2;
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

    void Creature::regenerateAu(int modifier, float factor, float time)
    {
        // das Ganze nur jede Spielrunde machen
        static float lastSpielrunde = 0;
        static int regeneratedAu = DsaManager::getSingleton().rollD6()
                + DsaManager::getSingleton().rollD6() 
                + DsaManager::getSingleton().rollD6();
        lastSpielrunde += time;


        if( getAu() == getAuMax() )
        {
            lastSpielrunde = 0;
            return;
        }


        while( lastSpielrunde >= Date::ONE_SPIELRUNDE )
        {
            lastSpielrunde -= Date::ONE_SPIELRUNDE;


            ///@todo Gibt es etwas das die Regeneration permanent modifiziert?
            //Grundregeneration von 3W6
            regeneratedAu = DsaManager::getSingleton().rollD6()
                + DsaManager::getSingleton().rollD6() 
                + DsaManager::getSingleton().rollD6();
            //Addiere eventuelle Modifikatoren hinzu
            //regeneratedAu += getWert(WERT_MOD_REGENERATION_LE);
            //Bei gelungener KO Probe addiere 1
            if (RESULT_ERFOLG <= doEigenschaftsprobe("KO", 
                getWertStateSet(WERT_MOD_REGENERATION_LE)->getProbenModifier()))
            {
                modifyAu(6*factor);
            }
            
            if( lastSpielrunde >= Date::ONE_SPIELRUNDE ) // mehrere Runden auf einmal
            {
                //modifiziere die aktuellen AU
                time -= Date::ONE_SPIELRUNDE;
                modifyAu((regeneratedAu-modifier)*factor);
            }




            if( getAu() == getAuMax() )
            {
                lastSpielrunde = 0;
                return;
            }
        }

        // Restbetrag regenerieren:
        float regeneratedAuPerTime = float(regeneratedAu-modifier)/Date::ONE_SPIELRUNDE * Date::ONE_SECOND * time;
        modifyAu(regeneratedAuPerTime*factor);
    }

	void Creature::addEffect(Effect* effect)
	{
		effect->setOwner(this);
		mEffectManager->addEffect(effect);
	}
    
    void Creature::addEffectWithCheckTime(Effect* effect, RL_LONGLONG time)
    {
      addEffect(effect);
      mEffectManager->addTimeCheck(time, effect);
    }
    
    void Creature::addEffectWithCheckDate(Effect* effect, RL_LONGLONG date)
    {
      addEffect(effect);
      mEffectManager->addDateCheck(date, effect);
    }


	void Creature::checkEffects()
	{
		/// @todo Nur einmal pro Aktion ausfuehren
		mEffectManager->checkEffects();
	}

    void Creature::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        if (key == Creature::PROPERTY_BEHAVIOURS)
        {
            mBehaviours = value;
        }
        else
        {
            GameObject::setProperty(key, value);
        }
    }

    const Property Creature::getProperty(const Ogre::String &key) const
    {
        if (key == Creature::PROPERTY_BEHAVIOURS)
        {
            return mBehaviours;
        }
        //else if (key == Item::PROPERTY_SIZE)
        //{
        //    return Property(mSize);
        //}
        else
        {
            return GameObject::getProperty(key);
        }
    }

    PropertySet* Creature::getAllProperties() const
    {
        PropertySet* ps = GameObject::getAllProperties();
        ps->setProperty(Creature::PROPERTY_BEHAVIOURS, mBehaviours);

        return ps;
    }

    int Creature::getTaktischeBewegung(void) const
    {
        return mMovementType;
    }

    void Creature::setTaktischeBewegung(int type)
    {
        mMovementType = type;
    }


    bool Creature::canUseTaktischeBewegung(int movementType)
    {
        if( movementType == BEWEGUNG_NONE )
        {
            return true;
        }
        // kann sich die Kreatur ueberhaupt bewegen?
        ///@todo What other Stati prohibit creature  movement? Paralyzed? Incapacitated? Sleep? etc.
        ///@todo Crouching still possible, even if immobilized.
        if (getStatus() & Effect::STATUS_IMMOBILE)
        {
          //CoreSubsystem::getSingleton().getRubyInterpreter()->execute("p \"Immobile!\"");
          return false;
        }

        if( getAu() <= 1 )
            return false;



        if( movementType & BEWEGUNG_SCHLEICHEN )
        {
            // if( getragenes Gewicht > 2*KK ) return false
            if( movementType & 
                (BEWEGUNG_RENNEN | BEWEGUNG_LAUFEN | BEWEGUNG_JOGGEN |
                BEWEGUNG_HOCHSPRUNG | BEWEGUNG_WEITSPRUNG | BEWEGUNG_UMDREHEN) 
                )
            {
                return false;
            }

        }
        else if( movementType & BEWEGUNG_SEITWAERTS )
        {
            if( movementType & 
                (BEWEGUNG_RENNEN | BEWEGUNG_JOGGEN |
                BEWEGUNG_HOCHSPRUNG | BEWEGUNG_WEITSPRUNG | BEWEGUNG_UMDREHEN)
                )
            {
                return false;
            }
        }
        else if( (movementType & BEWEGUNG_HOCHSPRUNG) || (movementType & BEWEGUNG_WEITSPRUNG) )
        {
            // if( getragenes Gewicht > KK ) return false
        }



        if( (movementType & BEWEGUNG_DREHEN) ||
            (movementType & BEWEGUNG_UMDREHEN))
        {
        }
        else if( movementType & BEWEGUNG_RENNEN )
        {
            // if( getragenes Gewicht > KK ) return false
            // getCurrentBE() > ?
            if( getAu() < 6.0 || getAu() < getAuBasis()/3.0 )
                return false;
        }
        else if( movementType & BEWEGUNG_LAUFEN )
        {
            // if( getragenes Gewicht > 1.5*KK ) return false
            if( getAu() < 6.0 )
                return false;
        }
        else if( movementType & BEWEGUNG_JOGGEN )
        {
            // if( getragenes Gewicht > 2*KK ) return false
            if( getAu() < 6.0 )
                return false;
        }
        else if( movementType & BEWEGUNG_GEHEN )
        {
        }    

        return true;
    }




    float Creature::getTaktischeGeschwindigkeitsBasis(int movementType, bool modified, int modifikatoren)
    {
        static float factorJoggen = 2.0;
        // Bewegung nicht möglich!
        if( !canUseTaktischeBewegung(movementType) )
            return 0;


        int act_gs = getWert(Creature::WERT_GS);
        if( modified )
            act_gs -= getCurrentBe();
        if( act_gs < 1 )
            act_gs = 1;

        float velocity = 0;


        // drehen ist ein sonderfall! angabe der Rotationsgeschwindigkeit in Umdrehungen pro Sekunde
        if( (movementType & BEWEGUNG_DREHEN) ||
            (movementType & BEWEGUNG_UMDREHEN) )
        {
            if( movementType & BEWEGUNG_UMDREHEN )
                velocity = 0.5;
            else
                velocity = 0.3;
            //if( modified )
            //    velocity -= getCurrentBe()/getEigenschaft("GE");
            if( movementType & BEWEGUNG_SCHLEICHEN )
                velocity *= 0.5;
            return velocity;
        }




        if( movementType & BEWEGUNG_SCHLEICHEN )
        {
            if( !(movementType & BEWEGUNG_GEHEN) && !(movementType & BEWEGUNG_JOGGEN) &&
                !(movementType & BEWEGUNG_RENNEN) && !(movementType & BEWEGUNG_LAUFEN) )
            {
                return 0;
            }
            if( (movementType & BEWEGUNG_RUECKWAERTS) || (movementType & BEWEGUNG_SEITWAERTS) )
                return 1;
            if( modified && getCurrentBe() > 1 )
                return 1;
            
            return 2;
        }
        else if( movementType & BEWEGUNG_WEITSPRUNG )
        {
            float mod;
            if( movementType & BEWEGUNG_RENNEN )
                mod = 1;
            else if( movementType & BEWEGUNG_LAUFEN )
                mod = 0.6;
            else if( movementType & BEWEGUNG_JOGGEN )
                mod = 0.5;
            else
                mod = 0.3;
            velocity = mod*(getEigenschaft("GE") + getEigenschaft("KK")) / 5.0;

            if( modified )
            {
                // steht nicht in den Regeln aber finde ich sinnvoll
                // velocityBase *= (1 - getrageneLast/KK);
                // steht in den Regeln: pro Erschöpfung ein KK abziehen
                if( mErschoepfung > getEigenschaft("KO") )
                    velocity -= mod*(mErschoepfung - getEigenschaft("KO")) / 5.0;
                // steht nicht in den Regeln, aber finde ich sinnvoll
                if( getAu() < getAuBasis() / 3.0 )
                    velocity -= mod*(getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 5.0;
            }
            if( movementType & BEWEGUNG_SCHLEICHEN )
            {
                velocity *= 0.3;
            }
            return velocity;
        }
        else if( movementType & BEWEGUNG_HOCHSPRUNG )
        {
            velocity = (getEigenschaft("GE") + getEigenschaft("KK")) / 4.0 / 5.0;
            
            if( modified )
            {
                // steht nicht in den Regeln aber finde ich sinnvoll
                // velocityBase *= (1 - getrageneLast/KK);
                // steht in den Regeln: pro Erschöpfung ein KK abziehen
                if( mErschoepfung > getEigenschaft("KO") )
                    velocity -= (mErschoepfung - getEigenschaft("KO")) / 4.0 / 5.0;
                // steht nicht in den Regeln, aber finde ich sinnvoll
                if( getAu() < getAuBasis() / 3.0 )
                    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 4.0 / 5.0;
            }
            if( movementType & BEWEGUNG_SCHLEICHEN )
            {
                velocity *= 0.3;
            }
            return velocity;
        }


        if( movementType & BEWEGUNG_RUECKWAERTS )
        {
            if( (movementType & BEWEGUNG_RENNEN) ||
                (movementType & BEWEGUNG_LAUFEN) ||
                (movementType & BEWEGUNG_JOGGEN) )
            {
                velocity = 0.6 * act_gs / factorJoggen;
            }
            else if( (movementType & BEWEGUNG_GEHEN) )
            {
                velocity = 0.6 * act_gs / 3.6;
            }

            if( movementType & BEWEGUNG_SCHLEICHEN )
            {
                velocity = 0.3 * act_gs / 3.6;
            }
            return velocity;
        }
        else if( movementType & BEWEGUNG_SEITWAERTS )
        {
            if( (movementType & BEWEGUNG_RENNEN) || 
                (movementType & BEWEGUNG_LAUFEN) ||
                (movementType & BEWEGUNG_JOGGEN) )
            {
                velocity = 0.8 * act_gs / factorJoggen;
            }
            else if( movementType & BEWEGUNG_GEHEN )
            {
                velocity = 0.8 * act_gs / 3.6;
            }
            
            if( movementType & BEWEGUNG_SCHLEICHEN )
            {
                velocity = 0.4 * act_gs / 3.6;
            }            
            return velocity;
        }



        if( movementType & BEWEGUNG_SCHLEICHEN )
        {
            if( !(movementType & BEWEGUNG_GEHEN) && !(movementType & BEWEGUNG_JOGGEN) &&
                !(movementType & BEWEGUNG_RENNEN) && !(movementType & BEWEGUNG_LAUFEN) )
            {
                return 0;
            }
            if( modified && getCurrentBe() > 1 )
                return 1;
            
            return 2;
        }


        if( movementType & BEWEGUNG_RENNEN )
        {
            velocity = act_gs;
        }
        else if( movementType & BEWEGUNG_LAUFEN )
        {
            velocity = act_gs / 2.0;
        }
        else if( movementType & BEWEGUNG_JOGGEN )
        {
            velocity = act_gs / 3.0;
        }
        else if( movementType & BEWEGUNG_GEHEN )
        {
            velocity = act_gs / 3.6;
        }
        else
        {
            velocity = 0;
        }
        return velocity;
    }



    float Creature::doTaktischeBewegung(int movementType, Ogre::Real time, int& patzer, int probenErschwernis, int modifikatoren)
    {
        // damit bei einem Sprint immer nur eine Probe gemacht wird!
        static int lastProbeTaW = 0;
        static Ogre::Real lastProbeTime = 0;
        static int lastMovementType = 0;
        lastProbeTime -= time;
        bool movementTypeChanged = false;
        if( movementType != lastMovementType ) // das System lässt sich durch Zwischendurch nicht rennen austricksen!!!!
            movementTypeChanged = true;
        lastMovementType = movementType;
        
        const int regenerateAuModifier_Gehen = 2;
        const float regenerateAuFactor_Gehen = 0.5;
        const int regenerateAuModifier_Stehen = 1;
        const float regenerateAuFactor_Stehen = 0.75;




        float velocity = getTaktischeGeschwindigkeitsBasis(movementType, true, modifikatoren);


        setTaktischeBewegung(movementType);

        


        if( movementType & BEWEGUNG_DREHEN )
        {
            return velocity;
        }



        if( movementType & BEWEGUNG_SCHLEICHEN )
        {
//
/*
            try // schleichen wirklich hier machen?
            {
                patzer = doTalentprobe("Schleichen", probenErschwernis);
            }
            catch(OutOfRangeException err)
            {
                patzer = RESULT_MISSERFOLG;
            }
*/
            return velocity;
        }
        else if( movementType & BEWEGUNG_WEITSPRUNG )
        {
            try
            {
                patzer = doTalentprobe("Athletik", probenErschwernis);
                if( patzer > 0 )
                {
                    if( patzer == RESULT_SPEKT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") / 25.0;
                    }
                    else if( patzer == RESULT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") / 50.0;
                    }
                    else
                    {
                        velocity += patzer / 50.0;
                    }
                }
            }
            catch(OutOfRangeException err)
            {
                patzer = 0;
            }
            // Ausdauerverbrauch: eigentlich 1, aber ich denke das ist zu hoch
            damageAu(2./3);
            return velocity;
        }
        else if( movementType & BEWEGUNG_HOCHSPRUNG )
        {
            try
            {
                patzer = doTalentprobe("Athletik", probenErschwernis);
                if( patzer > 0 )
                {
                    if( patzer == RESULT_SPEKT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") / 12.5;
                    }
                    else if( patzer == RESULT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") / 25.0;
                    }
                    else
                    {
                        velocity += patzer / 25.0;
                    }
                }
            }
            catch(OutOfRangeException err)
            {
                patzer = 0;
            }
            // Ausdauerverbrauch:
            damageAu(2./3);
            return velocity;
        }


        if( movementType & BEWEGUNG_RUECKWAERTS )
        {
            if( (movementType & BEWEGUNG_RENNEN) || 
                (movementType & BEWEGUNG_LAUFEN) ||
                (movementType & BEWEGUNG_JOGGEN) )
            {
            }
            else if( movementType & BEWEGUNG_GEHEN )
            {
            }
            return velocity;
        }
        else if( movementType & BEWEGUNG_SEITWAERTS )
        {
            if( (movementType & BEWEGUNG_RENNEN) || 
                (movementType & BEWEGUNG_LAUFEN) ||
                (movementType & BEWEGUNG_JOGGEN) )
            {
            }
            else if( movementType & BEWEGUNG_GEHEN )
            {
            }
            
            return velocity;
        }



        if( movementType & BEWEGUNG_RENNEN )
        {
            // für Rennen Athletik-probe -> höhere Geschwindigkeit
            try
            {
                if( lastProbeTime <= 0 || movementTypeChanged)
                {
                	int lastProbeTaW = doTalentprobe("Athletik", probenErschwernis);
                    patzer = (int)lastProbeTaW;
                    lastProbeTime = getAuMax();
                }
                else
                {
                    // wird nur einmal angerechnet ?
                    patzer = 0;
                }
                if( lastProbeTaW > 0 )
                {
                    if( lastProbeTaW == RESULT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") * 0.2;
                    }
                    else if( lastProbeTaW == RESULT_SPEKT_AUTOERFOLG )
                    {
                        velocity += getTalent("Athletik") * 0.3;
                    }
                    else
                    {
                        velocity += lastProbeTaW;
                    }
                }
                damageAu(time/1.5);
            }
            catch(OutOfRangeException err)
            {
                patzer = 0;
            }
        }
        else if( movementType & BEWEGUNG_LAUFEN )
        {
            // für Laufen, Athletik-Probe weniger Ausdauer-Verbrauch
            float timePerAu = 180;
            try
            {
                if( lastProbeTime <= 0 || movementTypeChanged )
                {
                    lastProbeTaW = doTalentprobe("Athletik", probenErschwernis);
                    patzer = lastProbeTaW;
                    lastProbeTime = Date::ONE_SPIELRUNDE;
                }
                else
                {
                    // wird nur einmal angerechnet ?
                    patzer = 0;
                }
                if( lastProbeTaW > 0 )
                {
                    if( lastProbeTaW == RESULT_AUTOERFOLG )
                    {
                        timePerAu += getTalent("Athletik") * 5;
                    }
                    else if( lastProbeTime == RESULT_SPEKT_AUTOERFOLG )
                    {
                        timePerAu += getTalent("Athletik") * 10;
                    }
                    else
                    {
                        timePerAu += lastProbeTaW * 5;
                    }
                }
                damageAu(time/timePerAu);
            }
            catch(OutOfRangeException err)
            {
                patzer = 0;
            }
        }
        else if( movementType & BEWEGUNG_JOGGEN )
        {
        }
        else if( movementType & BEWEGUNG_GEHEN )
        {
            regenerateAu(regenerateAuModifier_Gehen, regenerateAuFactor_Gehen, time);
        }
        else
        {
            // rumstehen
            regenerateAu(regenerateAuModifier_Stehen, regenerateAuFactor_Stehen, time);
        }

        return velocity;
    }


}
