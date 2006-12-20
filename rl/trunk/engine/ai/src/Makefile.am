INCLUDES = $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUIOGRE_CFLAGS) -I../include -I ../../core/include \
	-I ../../common/include -I ../../dialog/include -I ../../rules/include

lib_LTLIBRARIES = libRlAi.la
libRlAi_la_LDFLAGS = $(OGRE_LIBS) $(LIBS)
libRlAi_la_SOURCES = \
	Agent.cpp \
	AgentManager.cpp \
	AiSubsystem.cpp \
	AiWorld.cpp \
	FuzzyState.cpp \
	FuzzyStateMachine.cpp \
	PhysicalObstacle.cpp \
	PlayerVehicle.cpp \
	SteeringVehicle.cpp 
