/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "Creature.h"

#include "Actor.h"
#include "Container.h"
#include "CreatureControllerManager.h"
#include "DsaManager.h"
#include "EffectFactory.h"
#include "EffectManager.h"
#include "Eigenschaft.h"
#include "Exception.h"
#include "Inventory.h"
#include "Kampftechnik.h"
#include "MeshObject.h"
#include "MessagePump.h"
#include "RulesMessages.h"
#include "Slot.h"
#include "StateSet.h"
#include "Talent.h"
#include "Weapon.h"

using namespace std;

namespace rl
{
    const Ogre::String Creature::CLASS_NAME = "Creature";

    const Ogre::String Creature::PROPERTY_AI = "ai";
    const Ogre::String Creature::PROPERTY_AI_BEHAVIOURS = "behaviours";
    const Ogre::String Creature::PROPERTY_INVENTORY_WINDOW_TYPE = "inventorywindowtype";
    const Ogre::String Creature::PROPERTY_CURRENT_LE = "current_le";
    const Ogre::String Creature::PROPERTY_CURRENT_AE = "current_ae";
    const Ogre::String Creature::PROPERTY_CURRENT_AU = "current_au";
    const Ogre::String Creature::PROPERTY_CURRENT_FATIGUE = "current_fatigue";
    const Ogre::String Creature::PROPERTY_EFFECTS = "effects";
    const Ogre::String Creature::PROPERTY_EIGENSCHAFTEN = "eigenschaften";
    const Ogre::String Creature::PROPERTY_TALENTE = "talente";
    const Ogre::String Creature::PROPERTY_KAMPFTECHNIKEN = "kampftechniken";
    const Ogre::String Creature::PROPERTY_KAMPFAKTIONEN = "kampfaktionen";
    const Ogre::String Creature::PROPERTY_VORTEILE = "vorteile";
    const Ogre::String Creature::PROPERTY_NACHTEILE = "nachteile";
    const Ogre::String Creature::PROPERTY_SF = "sonderfertigkeiten";
    const Ogre::String Creature::PROPERTY_WERTE = "werte";
    const Ogre::String Creature::PROPERTY_AP = "ap";
    const Ogre::String Creature::PROPERTY_INVENTORY = "inventory";
    const Ogre::String Creature::PROPERTY_ANIMATIONS = "animations";
    const Ogre::String Creature::PROPERTY_ANIMATIONSPEEDS = "animationspeeds";

    // some targets
    const std::string Creature::ALL_EIGENSCHAFTEN = "alle Eigenschaften";
    const std::string Creature::ALL_TALENTE = "alle Talente";
    const Creature::Wert Creature::WERT_MOD_AE = "ModAE";
    const Creature::Wert Creature::WERT_MOD_LE = "ModLE";
    const Creature::Wert Creature::WERT_MOD_AU = "ModAU";
    const Creature::Wert Creature::WERT_MOD_MR = "ModMR";
    const Creature::Wert Creature::WERT_MOD_AT = "ModAT";
    const Creature::Wert Creature::WERT_MOD_PA = "ModPA";
    const Creature::Wert Creature::WERT_MOD_FK = "ModFK";
    const Creature::Wert Creature::WERT_GS = "GS";
    const Creature::Wert Creature::WERT_SOZIALSTATUS = "SO";
    const Creature::Wert Creature::WERT_BE = "BE";
    const Creature::Wert Creature::WERT_RS = "RS";
    const Creature::Wert Creature::WERT_INI = "INI";
    const Creature::Wert Creature::WERT_KAMPFUNFAEHIGKEITSSCHWELLE = "Kampfunfaehigkeitsschwelle";
    const Creature::Wert Creature::WERT_REGENERATION = "Regeneration";

    Creature::Creature(unsigned int id)
        : GameObject(id)
        , mCurrentLe(0)
        , mCurrentAu(0)
        , mCurrentAe(0)
        , mInventory(NULL)
        , mEigenschaften()
        , mWerte()
        , mTalente()
        , mKampftechniken()
        , mKampfaktionen()
        , mSonderfertigkeiten()
        , mErschoepfung(0)
        , mMovementType(0)
        , mAlignment(ALIGNMENT_NEUTRAL)
        , mTimeSinceLastRegeneration(0)
        , mLastCalculatedAuToRegenerate(DsaManager::getSingleton().rollD6() + DsaManager::getSingleton().rollD6()
              + DsaManager::getSingleton().rollD6())
    {
        mQueryFlags |= QUERYFLAG_CREATURE;

        setWert(WERT_MOD_AE, 0);
        setWert(WERT_MOD_LE, 0);
        setWert(WERT_MOD_AU, 0);
        setWert(WERT_MOD_MR, 0);
        setWert(WERT_GS, 8);
        setWert(WERT_RS, 0);
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

        for (SonderfertigkeitMap::iterator it = mSonderfertigkeiten.begin(); it != mSonderfertigkeiten.end(); it++)
            delete it->second;
        mSonderfertigkeiten.clear();
        CreatureControllerManager::getSingleton().detachController(this);
    }

    int Creature::getAttackeBasis() const
    {
        double es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getParadeBasis() const
    {
        double es = getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getFernkampfBasis() const
    {
        double es = getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_FINGERFERTIGKEIT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getInitiativeBasis() const
    {
        int es = 2 * getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getMrBasis() const
    {
        int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KLUGHEIT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 5.0 + 0.5);
    }

    int Creature::getLeBasis() const
    {
        int es = 2 * getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KOERPERKRAFT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAuBasis() const
    {
        int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_KONSTITUTION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_GEWANDTHEIT, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getAeBasis() const
    {
        int es = getEigenschaft(E_MUT, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_INTUITION, Effect::MODTAG_RECALCULATE)
            + getEigenschaft(E_CHARISMA, Effect::MODTAG_RECALCULATE);

        return static_cast<int>(es / 2.0 + 0.5);
    }

    int Creature::getWert(Wert wertId, bool getUnmodified) const
    {
        WertMap::const_iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            LOG_ERROR(Logger::RULES, (string(mName.c_str()) + ": Wert " + wertId.c_str() + " nicht gefunden.").c_str());
            return 0;
        }
        int rval = it->second;
        if (!getUnmodified)
        {
            rval += mEffectManager->getMod(wertId, Effect::MODTYPE_SUM);
            rval *= mEffectManager->getMod(wertId, Effect::MODTYPE_MULT);
        }
        return rval;
    }

    int Creature::getCurrentBe() const
    {
        pair<int, int> be = mInventory->getOverallBe();
        ///@todo Ruestungsgewoehnung?
        return be.first + be.second;
    }

    void Creature::setWert(Wert wertId, int value)
    {
        WertMap::iterator it = mWerte.find(wertId);
        if (it == mWerte.end())
        {
            mWerte.insert(make_pair(wertId, value));
        }
        else
        {
            it->second = value;
        }
    }

    void Creature::modifyLe(int mod, bool ignoreMax)
    {
        int oldLe = mCurrentLe;
        mCurrentLe += mod;
        if (!ignoreMax)
        {
            mCurrentLe = min(mCurrentLe, getLeMax());
        }

        if (mCurrentLe <= -getEigenschaft("KO") && oldLe > -getEigenschaft("KO"))
        {
            mEffectManager->addEffect(EffectFactoryManager::getSingleton().createEffect("DeadEffect"));
        }
        else if (mCurrentLe > -getEigenschaft("KO") && oldLe <= -getEigenschaft("KO"))
        {
            mEffectManager->removeEffect("DeadEffect");
        }
        else if (mCurrentLe <= 0 && oldLe > 0)
        {
            mEffectManager->addEffect(EffectFactoryManager::getSingleton().createEffect("UnconsciousEffect"));
        }
        else if (mCurrentLe > 0 && oldLe <= 0)
        {
            mEffectManager->removeEffect("UnconciousEffect");
        }
        else if (mCurrentLe <= getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE)
            && oldLe > getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE))
        {
            mEffectManager->addEffect(EffectFactoryManager::getSingleton().createEffect("IncapacitatedEffect"));
        }
        else if (mCurrentLe > getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE)
            && oldLe <= getWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE))
        {
            mEffectManager->removeEffect("IncapacitatedEffect");
        }

        fireObjectStateChangeEvent();
    }

    int Creature::getLe() const
    {
        return mCurrentLe;
    }

    int Creature::getLeMax() const
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

    int Creature::getAe() const
    {
        return mCurrentAe;
    }

    int Creature::getAeMax() const
    {
        return isMagic() ? getAeBasis() + getWert(WERT_MOD_AE) : 0;
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

    float Creature::getAu() const
    {
        return mCurrentAu;
    }

    int Creature::getAuMax() const
    {
        return getAuBasis() + getWert(WERT_MOD_AU);
    }

    void Creature::modifyAp(int modifier)
    {
        mAp.total += modifier;
    }

    int Creature::getAp() const
    {
        return mAp.total;
    }

    void Creature::modifyUsedAp(int modifier)
    {
        mAp.used += modifier;
    }

    int Creature::getUsedAp() const
    {
        return mAp.used;
    }

    int Creature::getEigenschaft(const CeGuiString eigenschaftName, Effect::ModTag tag) const
    {
        EigenschaftMap::const_iterator it = mEigenschaften.find(eigenschaftName);
        if (it == mEigenschaften.end())
        {
            Throw(IllegalArgumentException, "Eigenschaft nicht gefunden.");
        }
        int result = it->second;
        result += mEffectManager->getMod(eigenschaftName, Effect::MODTYPE_SUM, tag);
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
        mEigenschaften[eigenschaftName] += mod;
        fireObjectStateChangeEvent();
    }

    bool Creature::hasTalent(const CeGuiString talentName, bool ausweich) const
    {
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
            return true;
        /*
        if( ausweich )
        {
            Talent::AusweichTalente ausweichTalente;
            ausweichTalente =
                DsaManager::getSingleton().getTalent(talentName)->getAusweichTalente();
            Talent::AusweichTalente::const_iterator ausweichIt = ausweichTalente.begin();
            if (ausweichIt != ausweichTalente.end())
            {
                return true;
            }
        }
        */

        return false;
    }

    int Creature::getTalent(const CeGuiString talentName) const
    {
        LOG_DEBUG(Logger::RULES, "Using talent " + talentName);
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it == mTalente.end())
        {
            Talent::AusweichTalente ausweichTalente;
            ausweichTalente = DsaManager::getSingleton().getTalent(talentName)->getAusweichTalente();
            Talent::AusweichTalente::const_iterator ausweichIt = ausweichTalente.begin();
            if (ausweichIt == ausweichTalente.end())
            {
                Throw(IllegalArgumentException, "Talent nicht gefunden.");
            }
            int rval = getTalent(ausweichIt->first) - ausweichIt->second;
            for (ausweichIt++; ausweichIt != ausweichTalente.end(); ausweichIt++)
            {
                int tempAusweichTaw = getTalent(ausweichIt->first) - ausweichIt->second;
                if (tempAusweichTaw > rval)
                    rval = tempAusweichTaw;
            }
            return rval;
        } // if (it == mTalente.end())
        return it->second;
    }

    void Creature::addTalent(const CeGuiString talentName, int value)
    {
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
            Throw(IllegalArgumentException, "Talent schon in mTalente enthalten.");
        }
        DsaManager::getSingleton().getTalent(talentName); // ueberpruefe ob es das Talent ueberhaupt gibt
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
        it->second += mod;
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
        // it->second->setSe( true );
        /// @todo Implement SEs
    }

    void Creature::addKampftechnik(const CeGuiString kampftechnikName, const pair<int, int>& value)
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
        if (it != mKampftechniken.end())
        {
            Throw(IllegalArgumentException, "Kampftechnik schon in mKampftechniken enthalten.");
        }
        // ueberpruefe ob es die Kampftechnik ueberhaupt gibt
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

    void Creature::addKampfaktion(const CeGuiString& kampfaktionName)
    {
        mKampfaktionen.insert(kampfaktionName);
    }

    bool Creature::hasKampfaktion(const CeGuiString& kampfaktionName) const
    {
        return mKampfaktionen.find(kampfaktionName) != mKampfaktionen.end();
    }

    const std::set<CeGuiString>& Creature::getAllKampfaktionen() const
    {
        return mKampfaktionen;
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

    bool Creature::hasVorteil(const CeGuiString vorteilName) const
    {
        VorteilMap::const_iterator it = mVorteile.find(vorteilName);
        if (it != mVorteile.end())
        {
            return true;
        }
        else
            return false;
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

    bool Creature::hasNachteil(const CeGuiString nachteilName) const
    {
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it != mNachteile.end())
        {
            return true;
        }
        else
            return false;
    }

    int Creature::getSchlechteEigenschaft(const CeGuiString nachteilName) const
    {
        NachteilMap::const_iterator it = mNachteile.find(nachteilName);
        if (it == mNachteile.end())
        {
            Throw(IllegalArgumentException, "Nachteil nicht gefunden.");
        }
        else
            return it->second + mEffectManager->getMod(nachteilName, Effect::MODTYPE_SUM);
    }

    int Creature::getSf(const CeGuiString sfName) const
    {
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
        // ueberpruefe ob es die Sonderfertigkeit ueberhaupt gib
        /// @todo
        // DsaManager::getSingleton().getSf(sfName);
        mSonderfertigkeiten[sfName] = new SonderfertigkeitenStateSet();
        mSonderfertigkeiten[sfName]->setOriginalValue(value);
        fireObjectStateChangeEvent();
    }

    void Creature::setSf(const CeGuiString sfName, SfStatus value)
    {
        SonderfertigkeitMap::iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(IllegalArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        it->second->setOriginalValue(value);
        fireObjectStateChangeEvent();
    }

    SonderfertigkeitenStateSet* Creature::getSonderfertigkeitenStateSet(const CeGuiString sfName) const
    {
        SonderfertigkeitMap::const_iterator it = mSonderfertigkeiten.find(sfName);
        if (it == mSonderfertigkeiten.end())
        {
            Throw(IllegalArgumentException, "Sonderfertigkeit nicht gefunden.");
        }
        return it->second;
    }

    Effect::LifeState Creature::getLifeState() const
    {
        return mEffectManager->getLifeState();
    }

    bool Creature::isMagic() const
    {
        return getWert(WERT_MOD_AE) > 0;
    }

    void Creature::setStatus(int& statusVariable, bool value, const Ogre::String& errorMessage)
    {
        if (value == true)
        {
            statusVariable++;
        }
        else // value == false
        {
            if (statusVariable > 0)
            {
                statusVariable--;
            }
            else
            {
                LOG_MESSAGE(Logger::RULES, errorMessage);
            }
        } // value == false
    }

    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, Effect::ModTag spezialisierung, int modifier,
        CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        if (((talent->getArt() == TALENT_ART_SPEZIAL) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_SPEZIAL))
            || ((talent->getArt() == TALENT_ART_BERUF) && (getTalent(talentName) < TALENT_MIN_TAW_FOR_BERUF)))
        {
            /// @todo Find proper return value. Prompt an error message?
            // Throw(OutOfRangeException, "TaW zu niedrig");
            return -1;
        }
        EigenschaftTripel et(eigenschaft1Name, eigenschaft2Name, eigenschaft3Name);

        // Der Probenwurf
        Tripel<int> probe(DsaManager::getSingleton().roll3D20());

        // Glueckliche
        if ((probe.first == 1) && (probe.second == 1) && (probe.third == 1))
        {
            // BasisBoxS. 72 Der Wert wird sofort um 1 angehoben
            modifyTalent(talentName, +1);
            return RESULT_SPEKT_AUTOERFOLG;
        }
        if (((probe.first == 1) && (probe.second == 1)) || ((probe.first == 1) && (probe.third == 1))
            || ((probe.second == 1) && (probe.third == 1)))
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
        if (((probe.first == 20) && (probe.second == 20)) || ((probe.first == 20) && (probe.third == 20))
            || ((probe.second == 20) && (probe.third == 20)))
        {
            addSe(talentName);
            return RESULT_AUTOMISSERFOLG;
        }

        // Vor dem Vergleich hat man den Talentwert uebrig.
        int eBe = DsaManager::getSingleton()
                      .getTalent(talentName)
                      ->calculateEbe(mEffectManager->getMod(WERT_BE, Effect::MODTYPE_SUM));
        int taW = getTalent(talentName);
        int rval = taW - modifier - mEffectManager->getMod(talentName, Effect::MODTYPE_PROBENMOD, spezialisierung)
            - mEffectManager->getMod(ALL_TALENTE, Effect::MODTYPE_PROBENMOD, spezialisierung) - eBe;
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

        // TaP* niemals groesser als TaW (MFF14)
        if (rval > taW)
            rval = taW;
        return rval;
    }

    int Creature::doAlternativeTalentprobe(const CeGuiString talentName, int modifier, CeGuiString eigenschaft1Name,
        CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name)
    {
        return doAlternativeTalentprobe(
            talentName, Effect::MODTAG_NONE, modifier, eigenschaft1Name, eigenschaft2Name, eigenschaft3Name);
    }

    int Creature::doTalentprobe(const CeGuiString talentName, Effect::ModTag spezialisierung, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
        return doAlternativeTalentprobe(talentName, spezialisierung, modifier, et.first, et.second, et.third);
    }

    int Creature::doTalentprobe(const CeGuiString talentName, int modifier)
    {
        Talent* talent = DsaManager::getSingleton().getTalent(talentName);
        EigenschaftTripel et(talent->getEigenschaften());
        return doAlternativeTalentprobe(talentName, Effect::MODTAG_NONE, modifier, et.first, et.second, et.third);
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
            rval = getEigenschaft(eigenschaftName)
                - (probe + modifier + mEffectManager->getMod(eigenschaftName, Effect::MODTYPE_PROBENMOD, tag)
                      + mEffectManager->getMod(ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD, tag));
        }
        return rval;
    }

    Inventory* Creature::getInventory() const
    {
        return mInventory;
    }

    const Ogre::String& Creature::getInventoryWindowType() const
    {
        return mInventoryWindowType;
    }

    int Creature::doAttacke(const CeGuiString kampftechnikName, int modifier)
    {
        KampftechnikMap::const_iterator it = mKampftechniken.find(kampftechnikName);
        if (it == mKampftechniken.end())
        {
            Throw(IllegalArgumentException, "kampftechnikName nicht in mKampftechniken gefunden");
        }
        int rval;
        int eBe = (int)floor(
            float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

        int probe = DsaManager::getSingleton().rollD20();
        if (probe == 1) /// @todo Bestaetigen
        {
            rval = RESULT_GLUECKLICH;
        }
        else if (probe == 20) /// @todo Bestaetigen
        {
            rval = RESULT_PATZER;
        }
        else
        {
            rval = getAttackeBasis() + (*it).second.first
                - (probe + modifier + mEffectManager->getMod(ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD) + eBe);
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
        int eBe = (int)ceil(
            float(DsaManager::getSingleton().getKampftechnik(kampftechnikName)->calculateEbe(getWert(WERT_BE))) / 2.0);

        int probe = DsaManager::getSingleton().rollD20();
        if (probe == 1) /// @todo Bestaetigen
        {
            rval = RESULT_GLUECKLICH;
        }
        else if (probe == 20)
        {
            rval = RESULT_PATZER; /// @todo Bestaetigen
        }
        else
        {
            /// @todo Gute Parade
            rval = getParadeBasis() + (*it).second.second
                - (probe + modifier + mEffectManager->getMod(ALL_EIGENSCHAFTEN, Effect::MODTYPE_PROBENMOD) + eBe);
        }
        if (rval < 0)
            return RESULT_MISSERFOLG;
        else
            return RESULT_ERFOLG;
    }

    int Creature::doInitiativeWurf(bool getMaxInitiave)
    {
        int rval = getInitiativeBasis();
        rval += mEffectManager->getMod(WERT_INI, Effect::MODTYPE_SUM);
        rval -= mEffectManager->getMod(WERT_BE, Effect::MODTYPE_SUM);
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

    int Creature::doTrefferpunkteWurf(Weapon* weapon, DamageStrength damage) const
    {
        if (damage == DMG_NONE)
        {
            return 0;
        }

        // Roll dice
        const Tripel<int>& weaponTp = weapon->getTp();
        int rval = DsaManager::getSingleton().roll(weaponTp.first, weaponTp.second);
        rval += weaponTp.third;

        // Apply TP/KK

        int kk = getEigenschaft(E_KOERPERKRAFT);
        std::pair<int, int> tpkk = weapon->getTpKk();

        int bonus = (kk - tpkk.first) / tpkk.second;
        rval += bonus;

        if (damage == DMG_HALF)
        {
            rval = (rval + 1) / 2;
        }
        else if (damage == DMG_DOUBLE)
        {
            rval *= 2; ///@TODO check if correct
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
        if (damageType & LEDAMAGE_FIRE)
        {
            LOG_ERROR("Creature", "Fire damage not handled!"); ///@todo implement
        }
        if (damageType & LEDAMAGE_WATER)
        {
            LOG_ERROR("Creature", "Water damage not handled!"); ///@todo implement
        }
        if (damageType & LEDAMAGE_DEMONIC)
        {
            LOG_ERROR("Creature", "Demonic damage not handled!"); ///@todo implement
        }

        if (damageType & LEDAMAGE_TP_A)
        {
            damageAu(tp, AUDAMAGE_NORMAL);
            tp = (int)floor(tp / 2.);
        }

        int sp = -tp;
        if (!(damageType & LEDAMAGE_SP))
        {
            int rs = getWert(WERT_RS);
            sp += rs;
        }
        modifyLe(sp);

        if (sp >= getEigenschaft("KO"))
        {
            LOG_ERROR("Creature", "TODO: Add a wound."); ///@todo implement
        }
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
        // bei ueberanstrengung, kostet alles doppelte Au!!!!!
        if (mErschoepfung > getEigenschaft("KO"))
            aup *= 2;
        modifyAu(-aup);
    }

    void Creature::regenerateLe(int modifier)
    {
        // Grundregeneration von 1W6
        int regeneratedLe = DsaManager::getSingleton().rollD6();
        // Addiere eventuelle Modifikatoren hinzu
        regeneratedLe += mEffectManager->getMod(WERT_REGENERATION, Effect::MODTYPE_SUM, Effect::MODTAG_REGENERATION_LE);
        // Bei gelungener KO Probe addiere 1
        if (RESULT_ERFOLG <= doEigenschaftsprobe("KO", 0, Effect::MODTAG_REGENERATION_LE))
        {
            regeneratedLe++;
        }
        // modifiziere die aktuellen LE
        modifyLe(regeneratedLe);
    }

    void Creature::regenerateAe(int modifier)
    {
        // Grundregeneration von 1W6
        int regeneratedAe = DsaManager::getSingleton().rollD6();
        // Addiere eventuelle Modifikatoren hinzu
        regeneratedAe += mEffectManager->getMod(WERT_REGENERATION, Effect::MODTYPE_SUM, Effect::MODTAG_REGENERATION_AE);
        // Bei gelungener KO Probe addiere 1
        if (RESULT_ERFOLG <= doEigenschaftsprobe("IN", 0, Effect::MODTAG_REGENERATION_AE))
        {
            regeneratedAe++;
        }
        // modifiziere die aktuellen AE
        modifyAe(regeneratedAe);
    }

    void Creature::regenerateAu(int modifier, float factor, float time)
    {
        // das Ganze nur jede Spielrunde machen
        mTimeSinceLastRegeneration += time;

        if (getAu() == getAuMax())
        {
            mTimeSinceLastRegeneration = 0;
            return;
        }

        while (mTimeSinceLastRegeneration >= Date::ONE_SPIELRUNDE)
        {
            mTimeSinceLastRegeneration -= Date::ONE_SPIELRUNDE;

            ///@todo Gibt es etwas das die Regeneration permanent modifiziert?
            // Grundregeneration von 3W6
            mLastCalculatedAuToRegenerate = DsaManager::getSingleton().rollD6() + DsaManager::getSingleton().rollD6()
                + DsaManager::getSingleton().rollD6();
            // Addiere eventuelle Modifikatoren hinzu
            // regeneratedAu += getWert(WERT_MOD_REGENERATION_LE);
            // Bei gelungener KO Probe addiere 1
            if (RESULT_ERFOLG
                <= doEigenschaftsprobe("KO", 0,
                       mEffectManager->getMod(WERT_REGENERATION, Effect::MODTYPE_SUM, Effect::MODTAG_REGENERATION_AU)))
            {
                modifyAu(6 * factor);
            }

            if (mTimeSinceLastRegeneration >= Date::ONE_SPIELRUNDE) // mehrere Runden auf einmal
            {
                // modifiziere die aktuellen AU
                time -= Date::ONE_SPIELRUNDE;
                modifyAu((mLastCalculatedAuToRegenerate - modifier) * factor);
            }

            if (getAu() == getAuMax())
            {
                mTimeSinceLastRegeneration = 0;
                return;
            }
        }

        // Restbetrag regenerieren:
        float regeneratedAuPerTime
            = float(mLastCalculatedAuToRegenerate - modifier) / Date::ONE_SPIELRUNDE * Date::ONE_SECOND * time;
        modifyAu(regeneratedAuPerTime * factor);
    }

    void Creature::setAlignment(Creature::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Creature::Alignment Creature::getAlignment() const
    {
        return mAlignment;
    }

    void Creature::setProperty(const CeGuiString& key, const Property& value)
    {
        if (key == Creature::PROPERTY_AI)
        {
            mAiProperties = value;
        }
        else if (key == Creature::PROPERTY_INVENTORY_WINDOW_TYPE)
        {
            mInventoryWindowType = value.toString().c_str();
        }
        else if (key == Creature::PROPERTY_CURRENT_LE)
        {
            mCurrentLe = value.toInt();
        }
        else if (key == Creature::PROPERTY_CURRENT_AE)
        {
            mCurrentAe = value.toInt();
        }
        else if (key == Creature::PROPERTY_CURRENT_AU)
        {
            mCurrentAu = value.toReal();
        }
        else if (key == Creature::PROPERTY_CURRENT_FATIGUE)
        {
            mErschoepfung = value.toInt();
        }
        else if (key == Creature::PROPERTY_EFFECTS)
        {
            mEffectManager->setProperty(Creature::PROPERTY_EFFECTS, value);
        }
        else if (key == Creature::PROPERTY_EIGENSCHAFTEN)
        {
            mEigenschaften.clear();
            convertToMap(value.toMap(), mEigenschaften);
        }
        else if (key == Creature::PROPERTY_TALENTE)
        {
            mTalente.clear();
            convertToMap(value.toMap(), mTalente);
        }
        else if (key == Creature::PROPERTY_KAMPFTECHNIKEN)
        {
            mKampftechniken.clear();
            convertToMap(value.toMap(), mKampftechniken);
        }
        else if (key == Creature::PROPERTY_KAMPFAKTIONEN)
        {
            mKampfaktionen.clear();
            convertToSet(value.toArray(), mKampfaktionen);
        }
        else if (key == Creature::PROPERTY_VORTEILE)
        {
            mVorteile.clear();
            convertToMap(value.toMap(), mVorteile);
        }
        else if (key == Creature::PROPERTY_NACHTEILE)
        {
            mNachteile.clear();
            convertToMap(value.toMap(), mNachteile);
        }
        //        else if (key == Creature::PROPERTY_SF)
        //        {
        ///@todo Sonderfertigkeiten
        //        }
        else if (key == Creature::PROPERTY_WERTE)
        {
            mWerte.clear();
            convertToMap(value.toMap(), mWerte);
        }
        else if (key == Creature::PROPERTY_AP)
        {
            IntPair ip = value.toIntPair();
            mAp.total = ip.first;
            mAp.used = ip.second;
        }
        else if (key == Creature::PROPERTY_INVENTORY)
        {
            mInventory->setProperties(value.toMap());
        }
        else if (key == Creature::PROPERTY_ANIMATIONS)
        {
            mAnimations.clear();
            convertToMap(value.toMap(), mAnimations);
        }
        else if (key == Creature::PROPERTY_ANIMATIONSPEEDS)
        {
            mAnimationSpeeds.clear();
            convertToMap(value.toMap(), mAnimationSpeeds);
        }
        else
        {
            GameObject::setProperty(key, value);
        }
    }

    const Property Creature::getProperty(const CeGuiString& key) const
    {
        if (key == Creature::PROPERTY_AI)
        {
            return mAiProperties;
        }
        else if (key == Creature::PROPERTY_INVENTORY_WINDOW_TYPE)
        {
            return Property(mInventoryWindowType);
        }
        else if (key == Creature::PROPERTY_CURRENT_LE)
        {
            return Property(mCurrentLe);
        }
        else if (key == Creature::PROPERTY_CURRENT_AE)
        {
            return Property(mCurrentAe);
        }
        else if (key == Creature::PROPERTY_CURRENT_AU)
        {
            return Property(mCurrentAu);
        }
        else if (key == Creature::PROPERTY_CURRENT_FATIGUE)
        {
            return Property(mErschoepfung);
        }
        else if (key == Creature::PROPERTY_EFFECTS)
        {
            return mEffectManager->getProperty(Creature::PROPERTY_EFFECTS);
        }
        else if (key == Creature::PROPERTY_EIGENSCHAFTEN)
        {
            PropertyMap map = rl::convertToPropertyMap(mEigenschaften);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_TALENTE)
        {
            PropertyMap map = rl::convertToPropertyMap(mTalente);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_KAMPFTECHNIKEN)
        {
            PropertyMap map = rl::convertToPropertyMap(mKampftechniken);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_KAMPFAKTIONEN)
        {
            PropertyArray vec = rl::convertToPropertyArray(mKampfaktionen);
            return Property(vec);
        }
        else if (key == Creature::PROPERTY_VORTEILE)
        {
            PropertyMap map = rl::convertToPropertyMap(mVorteile);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_NACHTEILE)
        {
            PropertyMap map = rl::convertToPropertyMap(mNachteile);
            return Property(map);
        }
        //        else if (key == Creature::PROPERTY_SF)
        //        {
        ///@todo Sonderfertigkeiten
        //        }
        else if (key == Creature::PROPERTY_WERTE)
        {
            PropertyMap map = rl::convertToPropertyMap(mWerte);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_AP)
        {
            return Property(make_pair(mAp.total, mAp.used));
        }
        else if (key == Creature::PROPERTY_INVENTORY)
        {
            return mInventory->getAllProperties()->toPropertyMap();
        }
        else if (key == Creature::PROPERTY_ANIMATIONS)
        {
            PropertyMap map = rl::convertToPropertyMap(mAnimations);
            return Property(map);
        }
        else if (key == Creature::PROPERTY_ANIMATIONSPEEDS)
        {
            PropertyMap map = rl::convertToPropertyMap(mAnimationSpeeds);
            return Property(map);
        }
        else
        {
            return GameObject::getProperty(key);
        }
    }

    PropertyKeys Creature::getAllPropertyKeys() const
    {
        PropertyKeys keys(GameObject::getAllPropertyKeys());
        keys.insert(Creature::PROPERTY_AI);
        keys.insert(Creature::PROPERTY_INVENTORY_WINDOW_TYPE);
        keys.insert(Creature::PROPERTY_CURRENT_LE);
        keys.insert(Creature::PROPERTY_CURRENT_AE);
        keys.insert(Creature::PROPERTY_CURRENT_AU);
        keys.insert(Creature::PROPERTY_CURRENT_FATIGUE);
        keys.insert(Creature::PROPERTY_EFFECTS);
        keys.insert(Creature::PROPERTY_EIGENSCHAFTEN);
        keys.insert(Creature::PROPERTY_TALENTE);
        keys.insert(Creature::PROPERTY_KAMPFTECHNIKEN);
        keys.insert(Creature::PROPERTY_VORTEILE);
        keys.insert(Creature::PROPERTY_NACHTEILE);
        // keys.insert(Creature::PROPERTY_SF);
        keys.insert(Creature::PROPERTY_WERTE);
        keys.insert(Creature::PROPERTY_AP);
        keys.insert(Creature::PROPERTY_INVENTORY);
        keys.insert(Creature::PROPERTY_ANIMATIONS);
        keys.insert(Creature::PROPERTY_ANIMATIONSPEEDS);

        return keys;
    }

    Creature::AnimationSpeedPair Creature::getAnimation(const CeGuiString& name) const
    {
        AnimationSpeedPair ret;
        AnimationsMap::const_iterator itName = mAnimations.find(name);
        if (itName != mAnimations.end())
            ret.first = itName->second.c_str();
        else
            ret.first = name.c_str();

        AnimationSpeedsMap::const_iterator itSpeed = mAnimationSpeeds.find(name);
        if (itSpeed != mAnimationSpeeds.end())
            ret.second = itSpeed->second;
        else
            ret.second = 1;

        return ret;
    }

    bool Creature::canReachItem(const Item* item) const
    {
        return (item->getPosition() - getPosition()).length() <= 3.0f;
    }

    void Creature::doPlaceIntoScene()
    {
        GameObject::doPlaceIntoScene();

        // check items in inventory
        Inventory::SlotMap slots = mInventory->getAllSlots();
        Inventory::SlotMap::iterator it = slots.begin();
        for (; it != slots.end(); it++)
        {
            it->second->update();
        }
    }

    void Creature::doRemoveFromScene()
    {
        GameObject::doRemoveFromScene();

        ///@todo check if this is really necessary
        // check items in inventory
        // Inventory::SlotMap slots = mInventory->getAllSlots();
        // Inventory::SlotMap::iterator it = slots.begin();
        // for( ; it != slots.end(); it++ )
        //{
        //    it->second->update();
        //}
    }
}
