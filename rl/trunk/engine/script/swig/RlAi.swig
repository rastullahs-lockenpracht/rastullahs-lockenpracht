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

%include std_vector.i

namespace std {
	%template(Ogrevec3Vector) vector<Ogre::Vector3 >;
};

namespace rl
{
	// Before first use
	%apply SWIGTYPE *DYNAMIC { rl::AStarHeuristic * };
	%apply SWIGTYPE *DYNAMIC { rl::AStarCosts * };
    %apply SWIGTYPE *DYNAMIC { rl::AgentState* };

	class SteeringVehicle
	{
	public:
        SteeringVehicle(rl::Creature* creature);
		void addForce(const Ogre::Vector3& force);
		Ogre::Vector3 calcWander(const float elapsedTime);
		Ogre::Vector3 calcSeek(const Ogre::Vector3& target);
		Ogre::Vector3 calcFlee(const Ogre::Vector3& target);
		Ogre::Vector3 calcPursuit(rl::Agent* agent);
		Ogre::Vector3 calcAvoidObstacles(const float minTimeToCollision);
		Ogre::Vector3 calcAvoidNeighbors(const float minTimeToCollision);
		Ogre::Vector3 calcSteerTargetSpeed(const float targetSpeed);
		float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);
		Ogre::Vector3 getPosition();
		bool isAhead(rl::Agent* agent, const float threshold);
		bool needAvoidance(const float minTimeToCollision);
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

	%feature("director") AgentState;
    class AgentState
    {
    private:
        AgentState(rl::Agent* agent);
    };


    class AgentSteeringState : public rl::AgentState
    {
    public:
        void addSteeringBehaviour(SteeringBehaviour* behaviour);
    };

    class AgentDialogState : public rl::AgentState
    {
    public:
        void addDialogPartner(rl::Agent* partner);
        void setDialog(rl::Dialog* dialog);
    private:
        AgentDialogState(rl::Agent* agent);
    };

    typedef enum {AST_STEERING, AST_COMBAT, AST_DIALOG} AgentStateType;

	class Agent
	{
	public:
		Agent(rl::Creature* character);
		virtual ~Agent();
        rl::Creature* getControlledCreature() const;
        rl::AgentState* getCurrentState() const;
        void pushState(rl::AgentStateType type);
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

        virtual rl::SteeringBehaviour* createBehaviour(const Ogre::String& classname) = 0;
    };

	class AgentManager
	{
	public:
		static AgentManager& getSingleton(void);
		rl::Agent* createAgent(rl::Creature* character);

        virtual void setBehaviourFactory(BehaviourFactory* factory);
	};

	class AStarHeuristic
	{
	public:
		AStarHeuristic();
		virtual ~AStarHeuristic();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const = 0;
	};

	class ManhattanDistance
		: public rl::AStarHeuristic
	{
	public:
		ManhattanDistance();
		virtual ~ManhattanDistance();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	class DiagonalDistance
		: public rl::AStarHeuristic
	{
	public:
		DiagonalDistance();
		virtual ~DiagonalDistance();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	class EuclideanDistance
		: public rl::AStarHeuristic
	{
	public:
		EuclideanDistance();
		virtual ~EuclideanDistance();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	class EuclideanDistanceSquared
		: public rl::AStarHeuristic
	{
	public:
		EuclideanDistanceSquared();
		virtual ~EuclideanDistanceSquared();
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	class AStarCosts
	{
	public:
		AStarCosts(const rl::AStarHeuristic* Heuristic, float TieBreakFactor);
		virtual ~AStarCosts();
		virtual float calcCost(const rl::WayPointGraph* WPGraph, const rl::AStarWayPointNode* wp1,
			const rl::AStarWayPointNode* wp2) const = 0;
	};

	class AStarCostsDefault : public AStarCosts
	{
	public:
		AStarCostsDefault(const rl::AStarHeuristic* Heuristic, float TieBreakFactor);
		~AStarCostsDefault();
		virtual float calcCost(const rl::WayPointGraph* WPGraph, const rl::AStarWayPointNode* wp1,
			const rl::AStarWayPointNode* wp2) const;
	};

	class AStar
	{
	public:
		AStar( const rl::AStarCosts* Costs, const rl::WayPointGraph* WPGraph);
		//AStar( const rl::AStarCosts* Costs, const rl::WayPointGraph* WPGraph,
		//	const Ogre::Vector3& StartPos, const Ogre::Vector3& EndPos );
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
		void load(const Ogre::String& filename);
		const rl::WayPointNode* getNearestWayPoint(const Ogre::Vector3& position) const;
		const rl::WayPointNode* getWayPointAt(unsigned int index) const;
	};

	class WayPointGraphManager
	{
	public:
		static WayPointGraphManager& getSingleton(void);
		rl::WayPointGraph* createWayPointGraph();
	};

	class Landmark
	{
	public:
		Landmark(const Ogre::String& name, const Ogre::Vector3& position);
		const Ogre::String& getName() const;
		const Ogre::Vector3& getPosition() const;
	};

	class LandmarkPath
	{
	public:
        typedef std::deque<rl::Landmark*> LandmarkList;
		LandmarkPath(const Ogre::String& name);

		void addPoint(const Ogre::String& name, const Ogre::Vector3& position);
		void addPoint(rl::Landmark* lm);
        rl::LandmarkPath::LandmarkList getPoints();
	};

    class WalkPathBehaviour : public rl::SteeringBehaviour
	{
	public:
	    WalkPathBehaviour();
		virtual ~WalkPathBehaviour();
		void calcPathToLandmark(rl::Landmark* lm, const rl::WayPointGraph* wps);
		void setLandmarkPath(LandmarkPath* lmp);
		virtual rl::CeGuiString getType();
		virtual void init();
		virtual void activate();
		virtual void deactivate();
		virtual void update(const float elapsedTime);

		virtual float calculateActivation();
	};

	class CreatureWalkPathJob : public Job
    {
    public:
        CreatureWalkPathJob(const Ogre::String& name, rl::Creature* movingCreature, rl::Landmark* startLandmark);

        void addLandmark(const Ogre::String& name, const Ogre::Vector3& position);
        void addLandmark(rl::Landmark* lm);
        void setWayPoints(const rl::WayPointGraph* wps);
    };

    class Dialog
    {
    private:
        Dialog(rl::Creature* npc, rl::Creature* pc);
    };

    class DialogManager
    {
    public:
        static rl::DialogManager& getSingleton();
        rl::Dialog* createDialog(const Ogre::String& name, rl::Creature* npc);
        rl::Dialog* createDialog(const Ogre::String& name, rl::CreatureList& npcs);
    private:
        DialogManager();
    };
} // Namespace

%{
static swig_type_info *AStarHeuristic_dynamic_cast(void **ptr)
{
    rl::AStarHeuristic *pAStarHeuristic = static_cast<rl::AStarHeuristic*>(*ptr);

    rl::ManhattanDistance *pManhattanDistance=dynamic_cast<rl::ManhattanDistance*>(pAStarHeuristic);
    if (pManhattanDistance)
    {
        *ptr=pManhattanDistance;
        return SWIGTYPE_p_rl__ManhattanDistance;
    }
    rl::DiagonalDistance *pDiagonalDistance=dynamic_cast<rl::DiagonalDistance*>(pAStarHeuristic);
    if (pDiagonalDistance)
    {
        *ptr=pDiagonalDistance;
        return SWIGTYPE_p_rl__DiagonalDistance;
    }
    rl::EuclideanDistance *pEuclideanDistance=dynamic_cast<rl::EuclideanDistance*>(pAStarHeuristic);
    if (pEuclideanDistance)
    {
        *ptr=pEuclideanDistance;
        return SWIGTYPE_p_rl__EuclideanDistance;
    }
    rl::EuclideanDistanceSquared *pEuclideanDistanceSquared=dynamic_cast<rl::EuclideanDistanceSquared*>(pAStarHeuristic);
    if (pEuclideanDistanceSquared)
    {
        *ptr=pEuclideanDistanceSquared;
        return SWIGTYPE_p_rl__EuclideanDistanceSquared;
    }

    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__AStarHeuristic, AStarHeuristic_dynamic_cast);


%{
static swig_type_info *AStarCosts_dynamic_cast(void **ptr)
{
    rl::AStarCosts *pAStarCosts = static_cast<rl::AStarCosts*>(*ptr);

    rl::AStarCostsDefault *pAStarCostsDefault=dynamic_cast<rl::AStarCostsDefault*>(pAStarCosts);
    if (pAStarCostsDefault)
    {
        *ptr=pAStarCostsDefault;
        return SWIGTYPE_p_rl__AStarCostsDefault;
    }

    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__AStarCosts, AStarCosts_dynamic_cast);


%{
static swig_type_info *AgentState_dynamic_cast(void **ptr)
{
    rl::AgentState *pAgentState = static_cast<rl::AgentState*>(*ptr);

    rl::AgentDialogState *pAgentDialogState=dynamic_cast<rl::AgentDialogState*>(pAgentState);
    if (pAgentDialogState)
    {
        *ptr=pAgentDialogState;
        return SWIGTYPE_p_rl__AgentDialogState;
    }

    rl::AgentSteeringState *pAgentSteeringState = dynamic_cast<rl::AgentSteeringState*>(pAgentState);
    if (pAgentSteeringState)
    {
        *ptr=pAgentSteeringState;
        return SWIGTYPE_p_rl__AgentSteeringState;
    }

    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__AgentState, AgentState_dynamic_cast);


