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

#include "Combat.h"

#include "MeshObject.h"

#include "CombatController.h"
#include "CombatLogger.h"
#include "Creature.h"
#include "Exception.h"
#include "Logger.h"
#include "MeshAnimation.h"

using namespace std;


namespace rl {

	Combat::Combat()
	{
	}

	Combat::~Combat()
	{
	}

	void Combat::setLogger(CombatLogger* logger)
	{
		mLogger = logger;
	}

	void Combat::add(Creature* creature, int group)
	{
		CreatureData* part = new CreatureData(creature, group);
		initialize(part);
		mCreatureData.insert(make_pair(creature, part));
	}

	void Combat::initialize(CreatureData* part)
	{
		//TODO: Aktuelle Waffe, INI wuerfeln
		part->initiative = part->creature->doInitiativeWurf();
	}

	Combat::CreatureData::CreatureData(Creature* creature, int group)
	{
		this->creature = creature;
		this->group = group;
		this->initiative = NO_INI;
	}

	vector<Creature*> Combat::getGroupMembers(int group)
	{
		vector<Creature*> members;

		for (CreatureDataMap::iterator partIter = mCreatureData.begin();
			partIter != mCreatureData.end(); partIter++)
		{
			CreatureData* part = (*partIter).second;
			if (part->group == group)
				members.push_back(part->creature);
		}

		return members;
	}

	Combat::CreatureData* Combat::getData(Creature* creature)
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

	int Combat::getGroupOf(Creature* creature)
	{
		return getData(creature)->group;
	}

	void Combat::addController(rl::CombatController *controller)
	{
		mControllers.push_back(controller);
	}

	void Combat::start()
	{
		//tick();
	}

	void Combat::tick()
	{
		if (isOver())
		{
			for (std::vector<CombatController*>::iterator 
				it = mControllers.begin();
				it != mControllers.end();
				it++)
			{
				(*it)->notifyCombatEnd();
			}
		}
		else
		{
			CreatureData* next = getNextActor();

			Logger::getSingleton().log(
				Logger::RULES,
				Ogre::LML_NORMAL,
				"Aktion von "+next->creature->getName());

			Logger::getSingleton().log(
				Logger::RULES,
				Ogre::LML_NORMAL,
				"Nächste Aktion: "+Ogre::StringConverter::toString(next->nextAction),
				"Combat");
			
			if (next->nextAction == Combat::ACTION_ATTACK)
			{
				CreatureData* targetData = getData(next->nextAttackTarget);
				
				int at = next->creature->doAttacke(
					next->creature->getActiveWeapon()->getKampftechnik(), 
					0);

				bool pareeActivated = 
					(targetData->nextPareeTarget == NULL) 
					|| (targetData->nextPareeTarget == next->creature);
				int pa = 
					targetData->creature->doParade(
						targetData->creature->getActiveWeapon()->getKampftechnik(), 
						0, 
						at == RESULT_GLUECKLICH);

				MeshObject* actorObj = 
					dynamic_cast<MeshObject*>(
						next->creature->getActor()->getControlledObject());
				MeshObject* targetObj = 
					dynamic_cast<MeshObject*>(
						targetData->creature->getActor()->getControlledObject());
				if (pareeActivated && pa > 0)
				{
					BaseAnimation* attackAnim = actorObj->startAnimation("Attack1", 1.0, 1);
					BaseAnimation* paradeAnim = targetObj->startAnimation("Parade", 1.0, 1);
					paradeAnim->setDelay(0.5);
					mLogger->logParee(next->creature, targetData->creature);
				}
				else
				{
					actorObj->startAnimation("Attack1", 1.0, 1);
					BaseAnimation* paradeAnim = targetObj->startAnimation("Treffer", 1.0, 1);
					paradeAnim->setDelay(0.5);
					int tp = 3; //FIXME: Schaden erwürfeln
					targetData->creature->applyDamage(tp, next->creature->getActiveWeapon());
					mLogger->logHit(next->creature, targetData->creature, tp);
				}			
			}

			mInitiative = next->initiative;
			notifyNextActor();
		}
	}

	bool Combat::isOver()
	{
		//FIXME: Mehr Gruppen
		for (int group = 1; group <= 2; group++)
		{
			bool over = true;
			vector<Creature*> members = getGroupMembers(group);
			for (vector<Creature*>::iterator it = members.begin(); it != members.end(); it++)
			{
				Creature* cur = *it;
				if (cur->getLe() > 0)
				{
					over = false;
				}
			}

			if (over) //FIXME: Geht nur fuer 2 Gruppen
				return true;
		}

		return false;
	}

	void Combat::notifyNextActor()
	{
		CreatureData* next = getNextActor();
		getController(next->group)->notifyActionStart();
	}

	CombatController* Combat::getController(int group)
	{
		for (vector<CombatController*>::iterator it = mControllers.begin();
			it != mControllers.end(); it++)
		{
			CombatController* cur = *it;
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

	void Combat::setActionOption(CombatController* controller, Creature* actor, ActionOption option)
	{
		CreatureData* cd = getData(actor);
		if (cd->group != controller->getGroup())
		{
			Throw(IllegalArgumentException, "This Controller must not control other group");
		}

		cd->nextAction = option;
	}

	void Combat::setAttackTarget(CombatController* controller, Creature* actor, Creature* target)
	{
		if (actor != NULL)
		{
			CreatureData* cd = getData(actor);
			if (cd->group != controller->getGroup())
			{
				Throw(IllegalArgumentException, "This Controller must not control other group");
			}

			cd->nextAttackTarget = target;	
		}
	}

	void Combat::setPareeTarget(CombatController* controller, Creature* actor, Creature* target)
	{
		if (actor != NULL)
		{
			CreatureData* cd = getData(actor);
			if (cd->group != controller->getGroup())
			{
				Throw(IllegalArgumentException, "This Controller must not control other group");
			}

			cd->nextPareeTarget = target;	
		}
	}

	Combat::CreatureData* Combat::getNextActor()
	{
		for (CreatureDataMap::iterator it = mCreatureData.begin(); it != mCreatureData.end(); ++it)
		{
			CreatureData* cur = (*it).second;
			if (cur->initiative < mInitiative)
				return cur;
		}

		mInitiative = INI_START;
		for (CreatureDataMap::iterator it = mCreatureData.begin(); it != mCreatureData.end(); ++it)
		{
			CreatureData* cur = (*it).second;
			if (cur->initiative < mInitiative)
				return cur;
		}

		return NULL;
	}
}
