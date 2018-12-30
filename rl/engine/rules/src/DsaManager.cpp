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

#include "DsaManager.h"

#include "Creature.h"
#include "Date.h"
#include "DsaDataLoader.h"
#include "Eigenschaft.h"
#include "Kampftechnik.h"
#include "RulesSubsystem.h"
#include "Talent.h"

#include "Exception.h"
#include "GameTimeSource.h"

#include <cstdlib>

template <> rl::DsaManager* Ogre::Singleton<rl::DsaManager>::msSingleton = 0;

using namespace std;
using namespace Ogre;
using CEGUI::utf8;

namespace rl
{
    DsaManager::DsaManager()
        : mLastGameTime(0)
        , mLastClock(0)
        , mTimeScale(24.0f)
        , // 5 min == 2 hours, 1 hour == 1 day
        mEigenschaften()
        , mTalente()
        , mKampftechniken()
        , mCreatures()
    {
        // Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        initializeEigenschaften();
        initializeSkt();

        initializeTalente();
        initializeKampftechniken();
    }

    DsaManager::~DsaManager()
    {
        for (CreatureMap::iterator it = mCreatures.begin(); it != mCreatures.end(); ++it)
        {
            delete it->second;
        }
        for (KampftechnikMap::iterator it = mKampftechniken.begin(); it != mKampftechniken.end(); ++it)
        {
            delete it->second;
        }
        for (TalentMap::iterator it = mTalente.begin(); it != mTalente.end(); ++it)
        {
            delete it->second;
        }
        for (EigenschaftMap::iterator it = mEigenschaften.begin(); it != mEigenschaften.end(); ++it)
        {
            delete it->second;
        }
    }

    void DsaManager::initializeEigenschaften()
    {
        /// @warning So nicht lokalisierbar
        mEigenschaften[E_MUT] = new Eigenschaft("Mut", "MU", "");
        mEigenschaften[E_KLUGHEIT] = new Eigenschaft("Klugheit", "KL", "");
        mEigenschaften[E_INTUITION] = new Eigenschaft("Intuition", "IN", "");
        mEigenschaften[E_CHARISMA] = new Eigenschaft("Charisma", "CH", "");
        mEigenschaften[E_FINGERFERTIGKEIT] = new Eigenschaft("Fingerfertigkeit", "FF", "");
        mEigenschaften[E_GEWANDTHEIT] = new Eigenschaft("Gewandtheit", "GE", "");
        mEigenschaften[E_KONSTITUTION] = new Eigenschaft("Konstitution", "KO", "");
        mEigenschaften[E_KOERPERKRAFT] = new Eigenschaft("Koerperkraft", "KK", "");
    }

    void DsaManager::initializeTalente()
    {
        mTalente.clear();
    }

    void DsaManager::_addTalent(Talent* talent)
    {
        mTalente.insert(make_pair(talent->getName(), talent));
    }

    void DsaManager::_addCreature(Creature* creature)
    {
        mCreatures.insert(make_pair(creature->getId(), creature));
    }

    void DsaManager::_addKampftechnik(Kampftechnik* kampftechnik)
    {
        mKampftechniken.insert(make_pair(kampftechnik->getName(), kampftechnik));
    }

    void DsaManager::initializeKampftechniken()
    {
        mKampftechniken.clear();
    }

    bool DsaManager::isRuleActive(DsaManager::Rule rule) const
    {
        return false;
    }

    Time DsaManager::getTimestamp() const
    {
        TimeSource* ts = TimeSourceManager::getSingleton().getTimeSource(TimeSource::GAMETIME);

        if (ts)
        {
            return ts->getClock();
        }
        else
        {
            return 0; ///@todo better throw exception?
        }
    }

    Date DsaManager::getCurrentDate() const
    {
        GameTimeSource* ts
            = dynamic_cast<GameTimeSource*>(TimeSourceManager::getSingleton().getTimeSource(TimeSource::GAMETIME));

        if (ts)
        {
            return ts->getDate();
        }
        else
        {
            return Date(); ///@todo better throw exception?
        }
    }

    void DsaManager::setCurrentDate(const Date& date)
    {
        GameTimeSource* ts
            = dynamic_cast<GameTimeSource*>(TimeSourceManager::getSingleton().getTimeSource(TimeSource::GAMETIME));

        if (ts)
        {
            ts->setDate(date);
        }
        // else ///@todo better throw exception?
    }

    int DsaManager::rollD20() const
    {
        double d = std::rand();
        return static_cast<int>(d * 20.0 / RAND_MAX) + 1;
    }

    Tripel<int> DsaManager::roll3D20() const
    {
        return Tripel<int>(rollD20(), rollD20(), rollD20());
    }

    int DsaManager::rollD6() const
    {
        double d = std::rand();
        return static_cast<int>(d * 6.0 / RAND_MAX) + 1;
    }

    int DsaManager::roll(int d6, int d20) const
    {
        int sum = 0;

        for (int d = 0; d < d6; d++)
            sum += rollD6();
        for (int d = 0; d < d20; d++)
            sum += rollD20();

        return sum;
    }

    Talent* DsaManager::getTalent(const CeGuiString talentName) const
    {
        TalentMap::const_iterator it = mTalente.find(talentName);
        if (it != mTalente.end())
        {
            return (*it).second;
        }
        else
        {
            CeGuiString errorMessage = "Talent " + talentName + " nicht gefunden.";
            Throw(IllegalArgumentException, errorMessage.c_str());
        }
    }

    Kampftechnik* DsaManager::getKampftechnik(const CeGuiString kampftechnikName) const
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

    Eigenschaft* DsaManager::getEigenschaft(const CeGuiString eigenschaftName) const
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
            return 2.5 <= distance && distance < 4;
        default:
            return false;
        }
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

        for (int i = from; i < to; i++)
            sum += getSteigerKosten(column, i);

        return sum;
    }

    Creature* DsaManager::getCreature(int id) const
    {
        CreatureMap::const_iterator it = mCreatures.find(id);
        if (it != mCreatures.end())
        {
            return (*it).second;
        }
        else
        {
            Throw(IllegalArgumentException, "Creature not found.");
        }
    }
}
