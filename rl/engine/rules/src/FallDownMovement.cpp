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
#include "FallDownMovement.h"
#include "PhysicsManager.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    FallDownMovement::FallDownMovement(CreatureController* creature)
        : AbstractMovement(creature)
    {
        mAnim = mMovingCreature->getCreature()->getAnimation("fallen");
        // mAnim = mMovingCreature->getCreature()->getAnimation("fallen_anfang");
        // mAnim = mMovingCreature->getCreature()->getAnimation("fallen_landung_aufgefangen");
        // mAnim = mMovingCreature->getCreature()->getAnimation("fallen_landung_verletzt");
    }

    void FallDownMovement::activate()
    {
        mVel = 0;
    }

    void FallDownMovement::deactivate()
    {
        std::ostringstream oss;
        oss << "Fallen v: " << mVel
            << "   ermittelte Hoehe: " << mVel * mVel / (2 * fabs(PhysicsManager::getSingleton().getGravity().y));
        int h = int(mVel * mVel / (2 * fabs(PhysicsManager::getSingleton().getGravity().y)));
        oss << "    verwendete Hoehe: " << h << "m";

        if (h < 6 && h > 0) // nicht in den Regeln, aber angenehmer, bei gelunger GE-Probe noch aufgefangen
        {
            int probe = mMovingCreature->getCreature()->doEigenschaftsprobe("GE", 2 * h - 6);

            if (probe == RESULT_PATZER)
            {
                h++;
                oss << "    GE-Patzer!";
            }
            else if (probe >= 0)
            {
                h = 0;
                oss << "    Nochmal geschickt gelandet!";
            }
        }

        if (h > 0)
        {
            std::multiset<int> wuerfel;
            for (int i = 0; i < h; i++)
                wuerfel.insert(DsaManager::getSingleton().rollD6());

            int probenErschwernis = h;
            if (probenErschwernis > 10)
                probenErschwernis = 10;

            int taw = 0;
            if (mMovingCreature->getCreature()->hasTalent("K�rperbeherrschung"))
            {
                taw = mMovingCreature->getCreature()->doTalentprobe("K�rperbeherrschung", probenErschwernis);
            }

            int abgefangenerSchaden = 0;
            for (int i = 0; i < taw; i++)
            {
                if (wuerfel.size() <= 0)
                    break;
                abgefangenerSchaden += *(--(wuerfel.end()));
                wuerfel.erase(--(wuerfel.end()));
            }

            int sum = 0;
            for (std::multiset<int>::iterator it = wuerfel.begin(); it != wuerfel.end(); it++)
                sum += *it;

            mMovingCreature->getCreature()->damageLe(sum, Creature::LEDAMAGE_TP_A);

            oss << "    Schaden: " << sum << "    abgefangener Schaden: " << abgefangenerSchaden;
        }
        LOG_MESSAGE(Logger::RULES, oss.str());
    }

    bool FallDownMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = 0.0f;
        return isPossible();
    }

    bool FallDownMovement::isPossible() const
    {
        return mMovingCreature->getAbstractLocation() == CreatureController::AL_AIRBORNE
            && !(mMovingCreature->getCreature()->getLifeState() & Effect::LS_IMMOBILE);
    }

    void FallDownMovement::calculateForceAndTorque(Vector3& force, Vector3& torque, Real timestep)
    {
    }

    bool FallDownMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        mVel = max(-mMovingCreature->getVelocity().y, mVel);
        mMovingCreature->setAnimation(mAnim.first, mAnim.second, 0);
        return true;
    }

    void FallDownMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
    }

    bool FallDownMovement::isDirectionPossible(Ogre::Vector3& direction) const
    {
        direction = Vector3::ZERO;
        return false;
    }

    bool FallDownMovement::isRotationPossible(Ogre::Vector3& rotation) const
    {
        rotation = Vector3::ZERO;
        return false;
    }
}
