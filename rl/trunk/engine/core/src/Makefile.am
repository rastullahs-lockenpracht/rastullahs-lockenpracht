INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../sound/include

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = -L../../sound/src -Wl,--defsym -Wl,Init_libRlCore=Init_RlCore -lRlSound $(SDL_LIBS) $(SWIG_RUBY_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	ActorManager.cpp \
	AnimationManager.cpp \
	AudibleActor.cpp \
	BSPWorld.cpp \
	CameraActor.cpp \
	CoreSubsystem.cpp \
	GameActor.cpp \
	GameLoop.cpp \
	LightActor.cpp \
	ListenerActor.cpp \
	MeshActor.cpp \
	NatureWorld.cpp \
	ParticleSystemActor.cpp \
	PhysicalThing.cpp \
	PhysicsManager.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	SynchronizedTask.cpp \
	TerrainWorld.cpp \
	World.cpp \
	../swig/RlCore.cxx 
		 

