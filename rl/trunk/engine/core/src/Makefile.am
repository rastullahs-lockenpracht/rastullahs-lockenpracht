# Default values, if configuration file is missing (hard coded)
AM_CPPFLAGS = -DRL_MODULEDIR=\"$(RL_MODULEDIR)\" -DOGRE_PLUGINDIR=\"$(OGRE_PLUGINDIR)\"

INCLUDES = -I../include $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) $(BOOST_CPPFLAGS) $(OgreNewt_CFLAGS) \
           -I../../common/include -I../include/nulldriver $(meshmagick_CFLAGS)

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = $(OGRE_LIBS) $(CEGUI_LIBS) $(RUBY_LIBS) $(LIBS) $(meshmagick_LIBS) $(OgreNewt_LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	ActorControlledObject.cpp \
	ActorManager.cpp \
	AnimationManager.cpp \
	BaseAnimation.cpp \
	BoxPrimitive.cpp \
	CameraObject.cpp \
	ConfigurationManager.cpp \
	ContentLoader.cpp \
	ContentModule.cpp \
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
	JobQueue.cpp \
	JobScheduler.cpp \
	LightObject.cpp \
	LightFadeJob.cpp \
	LineSetPrimitive.cpp \
	ListenerObject.cpp \
	ListenerMovable.cpp \
	LinearSoundFadeFunctor.cpp \
	MergeableMeshObject.cpp \
	MeshAnimation.cpp \
	MeshObject.cpp \
	MessagePump.cpp \
	MovableText.cpp \
	ParticleSystemObject.cpp \
	PhysicsContactListener.cpp \
	PhysicalThing.cpp \
	PhysicsGenericContactCallback.cpp \
	PhysicsManager.cpp \
	PhysicsMaterialRaycast.cpp \
	PlayAnimationJob.cpp \
	PlaySoundJob.cpp \
	PolynomicSoundFadeFunctor.cpp \
	PrimitiveObject.cpp \
	RubyInterpreter.cpp \
	SaveAble.cpp \
	SaveAbleCollection.cpp \
	SaveAbleFactory.cpp \
	SaveAbleManager.cpp \
	SaveGameFile.cpp \
	SaveGameFileReader.cpp \
	SaveGameFileWriter.cpp \
	SaveGameManager.cpp \
	SceneQuery.cpp \
	SoundManager.cpp \
	SoundObject.cpp \
	SoundDriver.cpp \
	Sound.cpp \
	SoundEvents.cpp \
	SoundResource.cpp \
	SoundFadeJob.cpp \
	SoundFadeFunctor.cpp \
	SoundStitching.cpp \
	SoundStitchingObject.cpp \
	StartAnimationJob.cpp \
	TimeSource.cpp \
	Trigger.cpp \
	TrackAnimation.cpp \
	World.cpp \
	Zone.cpp \
	ZoneManager.cpp \
	nulldriver/NullDriver.cpp \
	nulldriver/NullListener.cpp \
	nulldriver/NullSound.cpp \
	nulldriver/NullSoundStitching.cpp
