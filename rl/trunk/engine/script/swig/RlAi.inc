/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

namespace rl 
{
	class SteeringVehicle
	{
	public:
		SteeringVehicle(rl::Agent* agent, rl::Actor* character);
		
		void addForce(const Ogre::Vector3& force);

		Ogre::Vector3 calcWander(const float elapsedTime);

		Ogre::Vector3 calcSeek(const Ogre::Vector3& target);

		Ogre::Vector3 calcFlee(const Ogre::Vector3& target);

		Ogre::Vector3 calcAvoidObstacles(const float minTimeToCollision);

		Ogre::Vector3 calcAvoidNeighbors(const float minTimeToCollision);
		
		Ogre::Vector3 calcSteerTargetSpeed(const float targetSpeed);
		
		bool isDialogActive();
		
		float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);
		
		Ogre::Vector3 getPosition();
		
		bool needAvoidance(const float minTimeToCollision);
		
		void setAnimation(rl::CeGuiString& name);
	};
	
	%feature("director") SteeringBehaviour;
	class SteeringBehaviour
	{
	public:
		SteeringBehaviour();
		virtual ~SteeringBehaviour();
		virtual rl::CeGuiString getType()=0;
		virtual void init()=0;
		virtual void activate()=0;
		virtual void deactivate()=0;
		virtual void update(const float elapsedTime)=0;

		virtual float calculateActivation()=0;
		
		float getActivationLevel();
		void setActivationLevel(float activationLevel);

		SteeringBehaviour* getParent();
		SteeringVehicle* getController();
	};
	
	class Agent
	{
	public:
		Agent(rl::Creature* character);
		virtual ~Agent();
		void addSteeringBehaviour(SteeringBehaviour* behaviour);
		bool isDialogActive();
	};
	class AiSubsystem 
	{
	public:
		static AiSubsystem& getSingleton(void);
	};
	
	class AgentManager
	{
	public:
		static AgentManager& getSingleton(void);
		rl::Agent* createAgent(rl::Creature* character);
		rl::Agent* createAgent(rl::DialogCharacter* character);
	};
}
