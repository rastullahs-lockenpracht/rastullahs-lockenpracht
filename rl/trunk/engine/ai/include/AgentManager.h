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
#ifndef __RlAI_AgentManager_H__
#define __RlAI_AgentManager_H__

#include <OgreSingleton.h>
#include "AiPrerequisites.h"
#include "PhysicsController.h"
#include "GameTask.h"

namespace rl
{
	class Agent;
	class Creature;
	class SteeringVehicle;

	class _RlAiExport AgentManager
		: public GameTask,
		  public PhysicsController,
		  protected Ogre::Singleton<AgentManager>
	{
	public:
		static AgentManager& getSingleton(void);
        static AgentManager* getSingletonPtr(void);
		typedef std::vector<SteeringVehicle*> VehicleList;

		AgentManager(void);
		virtual ~AgentManager(void);
		/**
		 * Create an AI agent with an assigned Rl-Actor
		 * AiSubsystems is responsible for memory management of the Agent
		 */
		Agent* createAgent(Creature* character);

		VehicleList getNeighbors(Agent* agent);
		void OnApplyForceAndTorque(PhysicalThing* thing);
		void run( Ogre::Real elapsedTime );
	private:
		typedef std::list<Agent*> AgentList;
		
		VehicleList mAllNeighbors;
		AgentList mAgents;
	};
}
#endif
