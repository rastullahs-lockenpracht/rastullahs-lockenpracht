INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../sound/include

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = -L../../sound/src -Wl,--defsym -Wl,Init_libRlCore=Init_RlCore -lRlSound $(SDL_LIBS) $(SWIG_RUBY_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	AudibleActor.cpp \
	BSPWorld.cpp \
	CameraActor.cpp \
	GameActor.cpp \
	GameLoop.cpp \
	LightActor.cpp \
	ParticleSystemActor.cpp \
	../swig/RlCore.cxx \
	MeshActor.cpp \
	ListenerActor.cpp \
	AnimationManager.cpp \
	CoreSubsystem.cpp \
	World.cpp \
	PhysicalThing.cpp \
	PhysicsManager.cpp \
	SynchronizedTask.cpp \
	ActorManager.cpp
		 

