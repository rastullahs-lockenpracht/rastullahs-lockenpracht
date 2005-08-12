INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../sound/include

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = -L../../sound/src -lRlSound $(SDL_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	ActorControlledObject.cpp \
	ActorManager.cpp \
	Animation.cpp \
	AnimationListener.cpp \
	AnimationManager.cpp \
	CameraObject.cpp \
	ConfigurationManager.cpp \
	ConfigurationManagerLinux.cpp \
	ConfigurationManagerWin32.cpp \
	ConfigurationManagerMac.cpp \
	CoreSubsystem.cpp \
	DotSceneLoader.cpp \
	DotSceneOctreeWorld.cpp \
	GameAreaEvent.cpp \
	GameAreaEventSource.cpp \
	GameAreaListener.cpp \
	GameAreaTypes.cpp \
	GameEventManager.cpp \
	GameLoop.cpp \
	GameTask.cpp \
	LightObject.cpp \
	ListenerObject.cpp \
	MeshObject.cpp \
	ParticleSystemObject.cpp \
	PhysicalThing.cpp \
	PhysicsManager.cpp \
	Primitive.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	SoundObject.cpp \
	SoundUpdateTask.cpp \
	TrackAnimation.cpp \
	World.cpp 		 

