noinst_HEADERS =
	Agent.h \
	AgentManager.h \
	AgentCombatState.h \
	AgentDialogState.h \
	AgentState.h \
	AgentSteeringState.h \
	AiPrerequisites.h \
	AiSubsystem.h \
	AiWorld.h \
	AStarCosts.h \
	AStar.h \
	AStarHeuristic.h \
	AStarNodePool.h \
	AStarWayPointNode.h \
	CreatureWalkPathJob.h \
	Dialog.h \
	DialogCondition.h \
	DialogElement.h \
	DialogElementSelection.h \
	DialogImplication.h \
	DialogManager.h \
	DialogOption.h \
	DialogResponse.h \
	DialogVariable.h \
	FuzzyState.h \
	FuzzyStateMachine.h \
	Landmark.h \
	LandmarkPath.h \
	PhysicalObstacle.h \
	SteeringMachine.h \
	SteeringVehicle.h \
	WalkPathBehaviour.h \
	WayPointNode.h \
	WayPointGraph.h \
	WayPointGraphManager.h

CLEANFILES = stdinc.h.gch
all-local: stdinc.h.gch
stdinc.h.gch: $(noinst_HEADERS) stdinc.h
	@echo Creating GCH file
	g++ -I. stdinc.h
