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

#include "Combatant.h"

#include "Combat.h"
#include "CreatureController.h"
#include "GoToJob.h"
#include "Inventory.h"
#include "Job.h"
#include "JobQueue.h"
#include "PlayAnimationJob.h"
#include "Weapon.h"

using namespace Ogre;

namespace rl
{
    Combatant::Combatant(Combat* combat, CreatureController* controller)
        : mCombat(combat), mController(controller)
    {
		mController->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);
    }

    Combatant::~Combatant()
    {
    }

    CreatureController* Combatant::getCreatureController() const
    {
        return mController;
    }
    
	Creature* Combatant::getCreature() const
	{
		return mController->getCreature();
	}

	CeGuiString Combatant::getName() const
    {
        return mController->getCreature()->getName();
    }

	Ogre::Vector3 Combatant::getPosition() const
	{
		return getCreature()->getPosition();
	}

	Weapon* Combatant::getActiveWeapon() const
	{
		std::vector<Weapon*> weapons = getCreature()->getInventory()->getReadiedWeapons();
		if (weapons.empty())
		{
			return NULL;
		}
		else
		{
			return weapons[0];
		}
	}

	int Combatant::rollAttacke()
	{
		Weapon* weapon = getActiveWeapon();
		return getCreature()->doAttacke(weapon->getKampftechnik(), 0);
	}

	int Combatant::rollParade(bool gluecklich)
	{
		Weapon* weapon = getActiveWeapon();
		return getCreature()->doParade(weapon->getKampftechnik(), 0, gluecklich);
	}

	int Combatant::rollTrefferpunkte() const
	{
		return getCreature()->doTrefferpunkteWurf(getActiveWeapon());
	}

	int Combatant::applyTrefferpunkte(int tp)
	{
		int oldLe = getCreature()->getLe();
		getCreature()->damageLe(tp);
		int newLe = getCreature()->getLe();
		return oldLe - newLe;
	}

	void Combatant::doAttacke(JobSet* jobSet, Combatant* target, int attackeResult, bool parade,
		int paradeResult)
	{
		// For the start, we just turn this to face target manually here.
		Vector3 pos = getCreature()->getPosition();
		pos.y = 0;
		Vector3 targetPos = target->getCreature()->getPosition();
		targetPos.y = 0;

		getCreature()->setOrientation(Vector3::NEGATIVE_UNIT_Z.getRotationTo(targetPos - pos));

		// Determine animation to be played according to outcome.
		Ogre::String ani;
		if (attackeResult >= RESULT_ERFOLG)
		{
			if (parade)
			{
				if (paradeResult >= RESULT_ERFOLG)
				{
					ani = "kampf_attacke_pariert";
				}
				else
				{
					ani = "kampf_attacke_treffer_nicht_pariert";
				}
			}
			else
			{
				ani = "kampf_attacke_treffer";
			}
		}
		else
		{
			ani = "kampf_attacke_misserfolg";
		}

		// Get real animation name
		Creature::AnimationSpeedPair asp = getCreature()->getAnimation(ani);
		PlayAnimationJob* job = new PlayAnimationJob(getCreature()->getActor(), asp.first);
        job->setHoldOnEnd(false);
        jobSet->add(job);
	}

	void Combatant::doParade(JobSet* jobSet, Combatant* target, int paradeResult)
	{
		// For the start, we just turn this to face target manually here.
		Vector3 pos = getCreature()->getPosition();
		pos.y = 0;
		Vector3 targetPos = target->getCreature()->getPosition();
		targetPos.y = 0;

		getCreature()->setOrientation(Vector3::NEGATIVE_UNIT_Z.getRotationTo(targetPos - pos));

		// Determine animation to be played according to outcome.
		Ogre::String ani;
		if (paradeResult >= RESULT_ERFOLG)
		{
			ani = "kampf_parade_erfolg";
		}
		else
		{
			ani = "kampf_parade_misserfolg";
		}

		// Get real animation name
		Creature::AnimationSpeedPair asp = getCreature()->getAnimation(ani);
		PlayAnimationJob* job = new PlayAnimationJob(getCreature()->getActor(), asp.first);
        job->setHoldOnEnd(false);
        jobSet->add(job);
	}

	void Combatant::doGetroffen(JobSet* jobSet)
	{
		// Get real animation name
		Creature::AnimationSpeedPair asp = getCreature()->getAnimation("kampf_getroffen");
		PlayAnimationJob* job = new PlayAnimationJob(getCreature()->getActor(), asp.first);
        job->setHoldOnEnd(false);
		jobSet->add(job);
	}

	void Combatant::doBewegen(JobSet* jobSet, const Ogre::Vector3& targetPos)
	{
		Job* job = new GoToJob(getCreature(), targetPos, 0.0f, 3.0f);
		jobSet->add(job);
	}

	void Combatant::doFolgen(JobSet* jobSet, Combatant* target)
	{
		Job* job = new GoToJob(getCreature(), target->getCreature(),
			getActiveWeapon()->getMaximumDistance(), 3.0f);
		jobSet->add(job);
	}

	Ogre::String Combatant::getMeshAnimationName(const Ogre::String animationName)
	{
		///@todo implement
		return animationName;
	}
}
