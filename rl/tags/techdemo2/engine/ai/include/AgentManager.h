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
//#include "PhysicsController.h"
#include "GameTask.h"

namespace rl
{
	class Agent;
	class Creature;
	class SteeringVehicle;
	class DialogCharacter;

	class _RlAiExport AgentManager
		: protected Ogre::Singleton<AgentManager>,
		  public GameTask
  	//	  public PhysicsController,
		  
	{
	public:
		enum AgentType
		{
			AGENT_NONE = -1,
			AGENT_PLAYER = 0,
			AGENT_STD_NPC = 1,
			AGENT_FLOCKING = 2
		};
		static AgentManager& getSingleton(void);
        static AgentManager* getSingletonPtr(void);
		typedef std::vector<SteeringVehicle*> VehicleList;

		AgentManager(void);
		virtual ~AgentManager(void);
		/**
		 * Create an AI agent with an assigned Rl-Actor
		 * AiSubsystems is responsible for memory management of the Agent
		 */
		Agent* createAgent(AgentType type, Creature* character);
		Agent* createAgent(DialogCharacter* character);

		VehicleList getNeighbors(Agent* agent);
	//	void OnApplyForceAndTorque(PhysicalThing* thing);
		void run( Ogre::Real elapsedTime );
		void removeAllAgents();
	private:
		void addAgent(Agent* agent);
		typedef std::list<Agent*> AgentList;
		
		VehicleList mAllNeighbors;
		AgentList mAgents;
		Agent* mPlayer;
	};
}
#endif
