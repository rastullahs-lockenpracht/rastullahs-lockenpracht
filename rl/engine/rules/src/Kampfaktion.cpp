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
#include "stdinc.h"

#include "Kampfaktion.h"

#include "Combatant.h"
#include "CreatureController.h"
#include "Exception.h"
#include "Inventory.h"
#include "Weapon.h"

#include <limits>

using namespace Ogre;

namespace rl
{
    Kampfaktion::Kampfaktion(Type type, const CeGuiString& name, const CeGuiString& description)
        : mType(type)
        , mName(name)
        , mDescription(description)
    {
    }

    Kampfaktion::~Kampfaktion()
    {
    }

    CeGuiString Kampfaktion::getName() const
    {
        return mName;
    }

    CeGuiString Kampfaktion::getDescription() const
    {
        return mDescription;
    }

    AttackeAktion::AttackeAktion()
        : Kampfaktion(AKTION, "Attacke", "Greife Gegner an.")
    {
    }

    AttackeAktion::~AttackeAktion()
    {
    }

    bool AttackeAktion::hasTarget() const
    {
        return true;
    }

    Real AttackeAktion::getMaximumTargetDistance(Combatant* actor) const
    {
        std::vector<Weapon*> weapons = actor->getCreature()->getInventory()->getReadiedWeapons();
        Real distance = 0.0f;
        for (size_t i = 0; i < weapons.size(); ++i)
        {
            distance = std::max(weapons[i]->getMaximumDistance(), distance);
        }
        return distance;
    }

    bool AttackeAktion::canDoKampfaktion(Combatant* actor, Combatant* target) const
    {
        // Does the actor have a weapon readied?
        if (actor->getCreature()->getInventory()->getReadiedWeapons().empty())
        {
            return false;
        }
        // Is target in weapon range?
        if (target->getPosition().distance(actor->getPosition()) > getMaximumTargetDistance(actor))
        {
            return false;
        }
        return true;
    }

    void AttackeAktion::doKampfaktion(Combatant* actor, Combatant* target)
    {
    }

    ParadeAktion::ParadeAktion()
        : Kampfaktion(REAKTION, "Parade", "Pariere Attacken des Gegners.")
    {
    }

    ParadeAktion::~ParadeAktion()
    {
    }

    bool ParadeAktion::hasTarget() const
    {
        return true;
    }

    Real ParadeAktion::getMaximumTargetDistance(Combatant* actor) const
    {
        return std::numeric_limits<Ogre::Real>::max();
    }

    bool ParadeAktion::canDoKampfaktion(Combatant* actor, Combatant* target) const
    {
        // Check, whether actor's weapon can be parried with our weapon/side weapon/shield.
        return true;
    }

    void ParadeAktion::doKampfaktion(Combatant* actor, Combatant* target)
    {
    }

    AusweichenAktion::AusweichenAktion()
        : Kampfaktion(REAKTION, "Ausweichen", "Weiche der Attacke des Gegners aus.")
    {
    }

    AusweichenAktion::~AusweichenAktion()
    {
    }

    bool AusweichenAktion::hasTarget() const
    {
        return true;
    }

    Ogre::Real AusweichenAktion::getMaximumTargetDistance(Combatant* actor) const
    {
        return std::numeric_limits<Ogre::Real>::max();
    }

    bool AusweichenAktion::canDoKampfaktion(Combatant* actor, Combatant* target) const
    {
        // Always possible.
        return true;
    }

    void AusweichenAktion::doKampfaktion(Combatant* actor, Combatant* target)
    {
    }

    BewegenAktion::BewegenAktion()
        : Kampfaktion(AKTION, "Bewegen", "Gehe zu einem Zielpunkt.")
    {
    }

    BewegenAktion::~BewegenAktion()
    {
    }

    bool BewegenAktion::hasTarget() const
    {
        return false;
    }

    Ogre::Real BewegenAktion::getMaximumTargetDistance(Combatant* actor) const
    {
        return std::numeric_limits<Ogre::Real>::max();
    }

    bool BewegenAktion::canDoKampfaktion(Combatant* actor, Combatant* target) const
    {
        return false;
    }

    void BewegenAktion::doKampfaktion(Combatant* actor, Combatant* target)
    {
        Throw(rl::OperationNotSupportedException, "BewegenAktion does not support Combatant as target.");
    }

    void BewegenAktion::doKampfaktion(Combatant* actor, const Ogre::Vector3& target)
    {
    }

    FolgenAktion::FolgenAktion()
        : Kampfaktion(AKTION, "Folgen", "Gehe zu einem Gegner.")
    {
    }

    FolgenAktion::~FolgenAktion()
    {
    }

    bool FolgenAktion::hasTarget() const
    {
        return true;
    }

    Ogre::Real FolgenAktion::getMaximumTargetDistance(Combatant* actor) const
    {
        return std::numeric_limits<Ogre::Real>::max();
    }

    bool FolgenAktion::canDoKampfaktion(Combatant* actor, Combatant* target) const
    {
        return true;
    }

    void FolgenAktion::doKampfaktion(Combatant* actor, Combatant* target)
    {
    }
}
