# Default values, if configuration file is missing (hard coded)
AM_CPPFLAGS = -DRL_MODULEDIR=\"$(RL_MODULEDIR)\" -DOGRE_PLUGINDIR=\"$(OGRE_PLUGINDIR)\"

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
	BoxPrimitive.cpp \
	CameraObject.cpp \
	ConfigurationManager.cpp \
	ContentModule.cpp \
	CoreEvents.cpp \
	CoreSubsystem.cpp \
	DebugVisualisable.cpp \
	DebugVisualsManager.cpp \
	DotSceneLoader.cpp \
	DotSceneOctreeWorld.cpp \
	FadeAnimation.cpp \
	GameAreaEvent.cpp \
	GameAreaEventSource.cpp \
	GameAreaListener.cpp \
	GameAreaTypes.cpp \
	GameEventManager.cpp \
	GameLoop.cpp \
	Job.cpp \
	JobScheduler.cpp \
	LightObject.cpp \
	LightFadeJob.cpp \
	LineSetPrimitive.cpp \
	ListenerObject.cpp \
	ListenerMovable.cpp \
	MeshAnimation.cpp \
	MeshObject.cpp \
	MovableText.cpp \
	ParticleSystemObject.cpp \
	PhysicsContactListener.cpp \
	PhysicalThing.cpp \
	PhysicsGenericContactCallback.cpp \
	PhysicsManager.cpp \
	PhysicsMaterialRaycast.cpp \
	PlayAnimationJob.cpp \
	PlaySoundJob.cpp \
	PrimitiveObject.cpp \
	RubyInterpreter.cpp \
	SceneQuery.cpp \
	SoundManager.cpp \
	SoundObject.cpp \
	SoundDriver.cpp \
	Sound.cpp \
	SoundEvents.cpp \
	SoundResource.cpp \
	TrackAnimation.cpp \
	World.cpp \
	Zone.cpp \
	ZoneManager.cpp \
	nulldriver/NullDriver.cpp \
	nulldriver/NullListener.cpp \
	nulldriver/NullSound.cpp
