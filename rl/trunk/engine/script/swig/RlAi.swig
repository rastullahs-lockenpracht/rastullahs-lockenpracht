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
	// Before first use of GameObject or subclasses!
	%apply SWIGTYPE *DYNAMIC { rl::AStarHeuristic * };
	%apply SWIGTYPE *DYNAMIC { rl::AStarCosts * };

	class SteeringVehicle
	{
	public:
		SteeringVehicle(rl::Agent* agent, rl::Actor* character);
		
		void addForce(const Ogre::Vector3& force);

		Ogre::Vector3 calcWander(const float elapsedTime);

		Ogre::Vector3 calcSeek(const Ogre::Vector3& target);

		Ogre::Vector3 calcFlee(const Ogre::Vector3& target);
		
		Ogre::Vector3 calcPursuit(rl::Agent* agent);

		Ogre::Vector3 calcAvoidObstacles(const float minTimeToCollision);

		Ogre::Vector3 calcAvoidNeighbors(const float minTimeToCollision);
		
		Ogre::Vector3 calcSteerTargetSpeed(const float targetSpeed);
		
		bool isDialogActive();
		
		float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);
		
		Ogre::Vector3 getPosition();
		
		bool isAhead(rl::Agent* agent, const float threshold);
		
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

		rl::SteeringBehaviour* getParent();
		rl::SteeringVehicle* getController();
	};
	
	class Agent
	{
	public:
		Agent(rl::Creature* character);
		virtual ~Agent();
		void addSteeringBehaviour(rl::SteeringBehaviour* behaviour);
		void clearSteeringBehaviours();
		bool isDialogActive();
	};
	class AiSubsystem 
	{
	public:
		static AiSubsystem& getSingleton(void);
	};
	
	%feature("director") BehaviourFactory;
    class BehaviourFactory
    {
    public:
        BehaviourFactory();
        virtual ~BehaviourFactory();

        virtual SteeringBehaviour* createBehaviour(const Ogre::String& classname) = 0;
    };
    
	class AgentManager
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
		rl::Agent* createAgent(rl::AgentManager::AgentType type, rl::Creature* character);
		rl::Agent* createAgent(rl::DialogCharacter* character);
		
        virtual void setBehaviourFactory(BehaviourFactory* factory);
	};
	
	class AStarHeuristic
	{
		AStarHeuristic();
		virtual ~AStarHeuristic();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const = 0;
	};
	
	class ManhattenDistance
		: public AStarHeuristic
	{
	public:
		ManhattenDistance();
		virtual ~ManhattenDistance();
	};

	class DiagonalDistance
		: public AStarHeuristic
	{
	public:
		DiagonalDistance();
		virtual ~DiagonalDistance();
	};

	class EuclideanDistance
		: public AStarHeuristic
	{
	public:
		EuclideanDistance();
		virtual ~EuclideanDistance();
	};

	class EuclideanDistanceSquared
		: public AStarHeuristic
	{
	public:
		EuclideanDistanceSquared();
		virtual ~EuclideanDistanceSquared();
	}; 
	
	class AStarCosts
	{
		AStarCosts(const AStarHeuristic* Heuristic);	
		virtual ~AStarCosts();
	};
	
	class AStarCostsDefault
	{
		AStarCostsDefault(const AStarHeuristic* Heuristic);	
		~AStarCostsDefault();
	};
	
	class AStar
	{
		AStar( const AStarCosts* Costs, const WayPointGraph* WPGraph,
			const Ogre::Vector3& StartPos, const Ogre::Vector3& EndPos );
		virtual ~AStar();
		
		void search(std::vector<Ogre::Vector3>& resultPath);
		void searchFromTo(std::vector<Ogre::Vector3>& resultPath,
			const Ogre::Vector3& StartPos, const Ogre::Vector3& EndPos );
		void reset();
	};
	
    class PhysicalObstacle {
        
        public:
            PhysicalObstacle(rl::PhysicalThing *thing);
            virtual ~PhysicalObstacle();
            
            OpenSteer::Obstacle *getObstacle() const;
            
            rl::PhysicalThing *getPhysicalThing() const;
            void setPhysicalThing(rl::PhysicalThing *thing);
    };
    
    class WayPointNode
	{
	public:
		enum WayPointNodeType 
		{
			WP_UNKNOWN = 0,
			WP_EXTERIOR = 1,
			WP_INTERIOR = 2
		};
		
		WayPointNode(const Ogre::Vector3& pos, const rl::WayPointNode::WayPointNodeType type);
		Ogre::Vector3 getPosition() const;
	};
	
	class WayPointGraph 
	{
	public:
		WayPointNode* addWayPoint(const Ogre::Vector3& position, const WayPointNode::WayPointNodeType type);
		void addConnection(rl::WayPointNode* wp1, rl::WayPointNode* wp2);
		void addDirectedConnection(rl::WayPointNode* wp1, const rl::WayPointNode* wp2);
		void load (const Ogre::String& filename);
		void save (const Ogre::String& filename) const;
		const rl::WayPointNode* getNearestWayPoint(const Ogre::Vector3& position) const;
		const rl::WayPointNode* getWayPointAt(unsigned int index) const;
	};
	
	class WayPointGraphManager 
	{
	public:
		static WayPointGraphManager& getSingleton(void);
		rl::WayPointGraph* createWayPointGraph();
	};
	
} // Namespace

%{
static swig_type_info *AStarHeuristic_dynamic_cast(void **ptr)
{
    rl::AStarHeuristic **ppAStarHeuristic = reinterpret_cast<rl::AStarHeuristic**>(ptr);

    rl::ManhattenDistance *pManhattenDistance=dynamic_cast<rl::ManhattenDistance*>(*ppAStarHeuristic);
    if( pManhattenDistance )
    {
        *ptr=reinterpret_cast<void*>(pManhattenDistance);
        return SWIGTYPE_p_rl__ManhattenDistance;
    }
    rl::DiagonalDistance *pDiagonalDistance=dynamic_cast<rl::DiagonalDistance*>(*ppAStarHeuristic);
    if( pDiagonalDistance )
    {
        *ptr=reinterpret_cast<void*>(pDiagonalDistance);
        return SWIGTYPE_p_rl__DiagonalDistance;
    }
    rl::EuclideanDistance *pEuclideanDistance=dynamic_cast<rl::EuclideanDistance*>(*ppAStarHeuristic);
    if( pEuclideanDistance )
    {
        *ptr=reinterpret_cast<void*>(pEuclideanDistance);
        return SWIGTYPE_p_rl__EuclideanDistance;
    }
    rl::EuclideanDistanceSquared *pEuclideanDistanceSquared=dynamic_cast<rl::EuclideanDistanceSquared*>(*ppAStarHeuristic);
    if( pEuclideanDistanceSquared )
    {
        *ptr=reinterpret_cast<void*>(pEuclideanDistanceSquared);
        return SWIGTYPE_p_rl__EuclideanDistanceSquared;
    }
   
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__AStarHeuristic, AStarHeuristic_dynamic_cast);


%{
static swig_type_info *AStarCosts_dynamic_cast(void **ptr)
{
    rl::AStarCosts **ppAStarCosts = reinterpret_cast<rl::AStarCosts**>(ptr);

    rl::AStarCostsDefault *pAStarCostsDefault=dynamic_cast<rl::AStarCostsDefault*>(*ppAStarCosts);
    if( pAStarCostsDefault )
    {
        *ptr=reinterpret_cast<void*>(pAStarCostsDefault);
        return SWIGTYPE_p_rl__AStarCostsDefault;
    }
   
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__AStarCosts, AStarCosts_dynamic_cast);


