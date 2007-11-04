INCLUDES = $(OGRE_CFLAGS) $(OgreNewt_CFLAGS) $(STLPORT_CFLAGS) $(CEGUIOGRE_CFLAGS) $(BOOST_CPPFLAGS) $(OpenSteer_CFLAGS) \
	-I../include -I ../../core/include -I ../../common/include -I ../../dialog/include -I ../../rules/include -I../include/predicates

lib_LTLIBRARIES = libRlAi.la
libRlAi_la_LDFLAGS = $(OgreNewt_LIBS) $(OpenSteer_LIBS) $(LIBS)
libRlAi_la_SOURCES = \
	Agent.cpp \
	AgentCombatState.cpp \
	AgentDialogState.cpp \
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
	Dialog.cpp \
	DialogCondition.cpp \
	DialogElement.cpp \
	DialogImplication.cpp \
	DialogManager.cpp \
	DialogOption.cpp \
	DialogResponse.cpp \
	DialogVariable.cpp \
	FuzzyState.cpp \
	FuzzyStateMachine.cpp \
	Landmark.cpp \
	LandmarkPath.cpp \
	PhysicalObstacle.cpp \
	SteeringVehicle.cpp \
	WayPointNode.cpp \
	WayPointGraph.cpp \
	WayPointGraphManager.cpp