INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../include/nulldriver

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = $(OGRE_LIBS) $(CEGUI_LIBS) $(RUBY_LIBS) $(LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	ActorControlledObject.cpp \
	ActorManager.cpp \
	AnimationListener.cpp \
	AnimationManager.cpp \
	BaseAnimation.cpp \
	CameraObject.cpp \
	ConfigurationManager.cpp \
	ConfigurationManagerLinux.cpp \
	ConfigurationManagerWin32.cpp \
	ConfigurationManagerMac.cpp \
	ContentModule.cpp \
	CoreEvents.cpp \
	CoreSubsystem.cpp \
	DotSceneLoader.cpp \
	DotSceneOctreeWorld.cpp \
	FadeAnimation.cpp \
	GameAreaEvent.cpp \
	GameAreaEventSource.cpp \
	GameAreaListener.cpp \
	GameAreaTypes.cpp \
	GameEventManager.cpp \
	GameLoop.cpp \
	LightObject.cpp \
	ListenerObject.cpp \
	MeshAnimation.cpp \
	MeshObject.cpp \
	MovableText.cpp \
	ParticleSystemObject.cpp \
	PhysicsContactListener.cpp \
	PhysicalThing.cpp \
	PhysicsGenericContactCallback.cpp \
	PhysicsManager.cpp \
	PhysicsMaterialRaycast.cpp \
	Primitive.cpp \
	RubyInterpreter.cpp \
	SoundManager.cpp \
	SoundObject.cpp \
	SoundUpdateTask.cpp \
	SoundDriver.cpp \
	ListenerMovable.cpp \
	Sound.cpp \
	SoundEvents.cpp \
	SoundResource.cpp \
	TrackAnimation.cpp \
	World.cpp \
	nulldriver/NullDriver.cpp \
	nulldriver/NullListener.cpp \
	nulldriver/NullSound.cpp
