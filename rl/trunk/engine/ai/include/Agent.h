/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#ifndef __RlAI_Agent_H__
#define __RlAI_Agent_H__

#include "PhysicsController.h"
#include "FuzzyState.h"
#include "AgentManager.h"

namespace rl
{
	class SteeringVehicle;
	typedef FuzzyState<SteeringVehicle> SteeringBehaviour;

	class SteeringMachine;
	class DialogCharacter;
	class Creature;
	class Actor;
//	class PerceptionPool;

	class _RlAiExport Agent 
		: public PhysicsController
	{
	public:
		Agent(Creature* character);
		Agent(Creature* character, SteeringVehicle* vehicle);
		Agent(DialogCharacter* character);
		virtual ~Agent(void);

		void initialize();

		AgentManager::AgentType getType();
		void setType(AgentManager::AgentType type);

		void update(const float elapsedTime);
		void addSteeringBehaviour(SteeringBehaviour* behaviour);
        void clearSteeringBehaviours();

		SteeringVehicle* getVehicle();
//		PerceptionPool* getPerceptionPool();
		bool isDialogActive();
		virtual void OnApplyForceAndTorque(PhysicalThing* thing);

	protected:
		AgentManager::AgentType mType;
		SteeringMachine* mBehaviour;
		SteeringVehicle* mVehicle;
		DialogCharacter* mDialogBot;
		Creature* mCreature;
//		PerceptionPool* mPerceptionPool;
	};

	inline AgentManager::AgentType Agent::getType()
	{
		return mType;
	}

	inline SteeringVehicle* Agent::getVehicle()
	{
		return mVehicle;
	}

	inline void Agent::setType(AgentManager::AgentType type)
	{
		mType = type;
	}

/*	inline PerceptionPool* Agent::getPerceptionPool()
	{
		return mPerceptionPool;
	}
*/
}
#endif
