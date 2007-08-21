INCLUDES = $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUIOGRE_CFLAGS) -I../include -I ../../core/include \
	-I ../../common/include -I ../../dialog/include -I ../../rules/include -I../include/predicates

lib_LTLIBRARIES = libRlAi.la
libRlAi_la_LDFLAGS = $(OGRE_LIBS) $(LIBS)
libRlAi_la_SOURCES = \
	Agent.cpp \
	AgentCombatState.cpp \
	AgentManager.cpp \
	AgentSteeringState.cpp \
	AiSubsystem.cpp \
	AiWorld.cpp \
	AStarCosts.cpp \
	AStar.cpp \
	AStarHeuristic.cpp \
	AStarNodePool.cpp \
	AStarWayPointNode.cpp \
	CreatureWalkPathJob.cpp \
	FuzzyState.cpp \
	FuzzyStateMachine.cpp \
	Landmark.cpp \
	LandmarkPath.cpp \
	PhysicalObstacle.cpp \
	SteeringVehicle.cpp \
	WayPointNode.cpp \
	WayPointGraph.cpp \
	WayPointGraphManager.cpp \
	AimlNodeImplRl.cpp \
	AimlParserImplRl.cpp \
	ContextConditionProcessor.cpp \
	ContextInterpreter.cpp \
	DialogCharacter.cpp \
	DialogOption.cpp \
	DialogResponse.cpp \
	DialogScriptProcessor.cpp \
	DialogSubsystem.cpp \
	ScriptProcessor.cpp \
	predicates/CreaturePredicates.cpp \
	predicates/EigenschaftsProbePredicates.cpp \
	predicates/EigenschaftsWertPredicates.cpp \
	predicates/GrundWertPredicates.cpp \
	predicates/QuestKnownPredicates.cpp \
	predicates/QuestPartsPredicates.cpp \
	predicates/QuestStatePredicates.cpp \
	predicates/ScriptPredicates.cpp \
	predicates/TalentProbePredicates.cpp \
	predicates/TalentWertPredicates.cpp	
