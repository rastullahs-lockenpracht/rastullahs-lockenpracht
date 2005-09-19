INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../multimedia/include

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_CFLAGS =
libRlCore_la_LDFLAGS = -L../../multimedia/src -lRlMultimedia $(OGRE_LIBS) $(CEGUI_LIBS) $(RUBY_LIBS) $(LIBS)
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
	DeletionPropagator.cpp \
	DotSceneLoader.cpp \
	DotSceneOctreeWorld.cpp \
	GameAreaEvent.cpp \
	GameAreaEventSource.cpp \
	GameAreaListener.cpp \
	GameAreaTypes.cpp \
	GameEventManager.cpp \
	GameLoop.cpp \
	LightObject.cpp \
	ListenerObject.cpp \
	MeshObject.cpp \
	ParticleSystemObject.cpp \
	PhysicalThing.cpp \
	PhysicsManager.cpp \
	Primitive.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	ScriptObjectRepository.cpp \
	SoundObject.cpp \
	TrackAnimation.cpp \
	World.cpp 		 

if WITH_FMOD
libRlCore_la_CFLAGS += -DWITH_FMOD
endif
if WITH_OAL
libRlCore_la_CFLAGS += -DWITH_OPENAL
endif
