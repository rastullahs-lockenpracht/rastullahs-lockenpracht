INCLUDES = $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include

lib_LTLIBRARIES = libRlAi.la
libRlCommon_la_LDFLAGS = $(OGRE_LIBS) $(OPENSTEER_LIBS) $(LIBS)
libRlCommon_la_SOURCES = \
	Agent.cpp \
	AgentManager.cpp \
	AiSubsystem.cpp \
	FuzzyState.cpp \
	FuzzyStateMachine.cpp \
	SteeringMachine.cpp \
	SteeringVehicle.cpp
