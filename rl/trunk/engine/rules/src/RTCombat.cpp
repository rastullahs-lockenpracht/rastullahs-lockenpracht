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

#include "MeshAnimation.h"
#include "CombatLogger.h"
#include "Creature.h"
#include "Date.h"
#include "DsaManager.h"
#include "Exception.h"
#include "GameLoop.h"
#include "MeshObject.h"
#include "RTCombat.h"
#include "RTCombatController.h"
#include "ScriptWrapper.h"
#include "Weapon.h"

using namespace std;

namespace rl {
	RTCombat::RTCombat()
	{
		mTimeKR = 99999;
	}

	void RTCombat::run(Ogre::Real elapsedTime)
	{
		 mTimeKR += elapsedTime;
		 if (mTimeKR > Date::ONE_KAMPFRUNDE)
		 {
			 initKR();
			 mTimeKR -= Date::ONE_KAMPFRUNDE;
		 }
	}

	void RTCombat::add(Creature* creature, int group)
	{
		ScriptWrapper::getSingleton().owned(creature);
		CreatureData* part = new CreatureData(creature, group);
		mCreatureData.insert(make_pair(creature, part));
	}

	void RTCombat::release()
	{
		GameLoopManager::getSingleton().removeAsynchronousTask(this);

		for (CreatureDataMap::iterator iter = mCreatureData.begin();
			iter != mCreatureData.end();
			iter++)
		{
			ScriptWrapper::getSingleton().disowned((*iter).first);
		}
	}
		
	void RTCombat::attack(RTCombatController* controller, Creature* attacker, Creature* target)
	{
		if (attacker != NULL)
		{
			CreatureData* cdAt = getData(attacker);
			if (cdAt->group != controller->getGroup())
			{
				Throw(IllegalArgumentException, "This Controller must not control other group");
			}

			CreatureData* cdPa = getData(target);

			if (isAttackPossible(cdAt, cdPa))
			{
				int at = cdAt->creature->doAttacke(
					cdAt->creature->getActiveWeapon()->getKampftechnik(), 
					0);
				int pa;

				cdPa->nextAttackTime = getCurrentTime() + Date::ONE_KAMPFRUNDE;

				bool paree = false;
				if (isPareeActivated(cdPa, cdAt))
				{
					cdPa->numParees -= 1;
					pa = cdPa->creature->doParade(
						cdPa->creature->getActiveWeapon()->getKampftechnik(), 
						0, 
						at == RESULT_GLUECKLICH);
				}
				else
				{
					pa = -1;
				}

				if (at < 0 || pa >= 0)
				{
					displayAtPa(cdAt->creature, cdPa->creature);
					mLogger->logParee(cdAt->creature, cdPa->creature);
				}
				else
				{
					displayAtHit(cdAt->creature, cdPa->creature);
					int tp = 3; //FIXME: Schaden erwürfeln
					cdPa->creature->applyDamage(tp, cdAt->creature->getActiveWeapon());
					mLogger->logHit(cdAt->creature, cdPa->creature, tp);
				}			
			}
		}
	}

	void RTCombat::displayAtPa(Creature* attacker, Creature* defender)
	{
		MeshObject* attackerObj = 
			dynamic_cast<MeshObject*>(
				attacker->getActor()->getControlledObject());
		MeshObject* targetObj = 
			dynamic_cast<MeshObject*>(
				defender->getActor()->getControlledObject());

		BaseAnimation* attackAnim = attackerObj->startAnimation("Attack1", 1.0, 1);
		BaseAnimation* paradeAnim = targetObj->startAnimation("Parade", 1.0, 1);
		paradeAnim->setDelay(0.5);
	}

	void RTCombat::displayAtHit(Creature* attacker, Creature* defender)
	{
		MeshObject* attackerObj = 
			dynamic_cast<MeshObject*>(
				attacker->getActor()->getControlledObject());
		MeshObject* targetObj = 
			dynamic_cast<MeshObject*>(
				defender->getActor()->getControlledObject());

		attackerObj->startAnimation("Attack1", 1.0, 1);
		BaseAnimation* paradeAnim = targetObj->startAnimation("Treffer", 1.0, 1);
		paradeAnim->setDelay(0.5);
	}

	void RTCombat::initKR()
	{
		for (CreatureDataMap::iterator iter = mCreatureData.begin();
			iter != mCreatureData.end();
			iter++)
		{
			CreatureData* cd = (*iter).second;
			cd->numParees = cd->convertAt ? 1+1 : 1;
		}
	}

	void RTCombat::paree(RTCombatController* controller, Creature* defender, Creature* target)
	{
		if (defender != NULL)
		{
			CreatureData* cd = getData(defender);
			if (cd->group != controller->getGroup())
			{
				Throw(IllegalArgumentException, "This Controller must not control other group");
			}
			cd->nextPareeTarget = target;
		}
	}

	void RTCombat::start()
	{
		GameLoopManager::getSingleton().addAsynchronousTask(this);
	}

	void RTCombat::setLogger(CombatLogger* logger)
	{
		mLogger = logger;
	}

	bool RTCombat::isAttackPossible(RTCombat::CreatureData *attacker, RTCombat::CreatureData *defender)
	{
		if (attacker->nextAttackTime > getCurrentTime())
		{
			return false;
		}

		Weapon* atWeapon = attacker->creature->getActiveWeapon();
		if (atWeapon == NULL)
		{
			return false;
		}

		Weapon::Distanzklasse dk = atWeapon->getDk();
		Ogre::Real distance = 
			(attacker->creature->getActor()->getPosition()
			- defender->creature->getActor()->getPosition()).length();

		if (DsaManager::getSingleton().isDkDistance(dk, distance))
		{
			return true;
		}

		return false;
	}

	bool RTCombat::isPareeActivated(RTCombat::CreatureData *defender, RTCombat::CreatureData *attacker)
	{
		if (defender->numParees == 0)
			return false;

		//TODO: Angriffsposition (von hinten nicht parierbar)

		if (defender->nextPareeTarget != NULL
			&& defender->nextPareeTarget != attacker->creature)
		{
			//TODO: Mehrere Paraden
			//TODO: Wenn Paradeziel in dieser KR nicht mehr angreifen kann, dann auch parieren
			return false;
		}

		return true;
	}

	RTCombat::CreatureData::CreatureData(Creature *creature, int group)
	{
		this->creature = creature;
		this->group = group;
		this->convertAt = false;
		this->convertPa = false;
	}

	RTCombatController* RTCombat::getController(int group)
	{
		for (vector<RTCombatController*>::iterator it = mControllers.begin();
			it != mControllers.end(); it++)
		{
			RTCombatController* cur = *it;
			if (cur->getGroup() == group)
			{
				return cur;
			}
		}

		Throw(
			rl::IllegalArgumentException, 
			"No controller for group"
			+ Ogre::StringConverter::toString(group));
		return NULL;
	}

	void RTCombat::addController(RTCombatController* controller)
	{
		mControllers.push_back(controller);
	}

	const Time RTCombat::getCurrentTime() const
	{
		return DsaManager::getSingleton().getTimestamp();
	}

	RTCombat::CreatureData* RTCombat::getData(Creature* creature)
	{
		vector<Creature*> members;

		for (CreatureDataMap::iterator partIter = mCreatureData.begin();
			partIter != mCreatureData.end(); partIter++)
		{
			CreatureData* part = (*partIter).second;
			if (part->creature == creature)
				return part;
		}

		Throw(IllegalArgumentException, (creature->getName()+" nimmt nicht am Kampf teil.").c_str());
	}

}
