INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../sound/include

lib_LTLIBRARIES = libRlCore.la
libRlCore_la_LDFLAGS = -L../../sound/src -lRlSound $(SDL_LIBS) $(SWIG_RUBY_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)
libRlCore_la_SOURCES = \
	Actor.cpp \
	ActorControlledObject.cpp \
	ActorManager.cpp \
    AnimationListener.cpp \
	AnimationManager.cpp \
	CameraObject.cpp \
	CoreSubsystem.cpp \
	DotSceneOctreeWorld.cpp \
	GameLoop.cpp \
	GameTask.cpp \
	LightObject.cpp \
	MeshObject.cpp \
	PhysicalThing.cpp \
	PhysicsManager.cpp \
	RlAnimation.cpp \
	RlTrackAnimation.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	World.cpp 		 

