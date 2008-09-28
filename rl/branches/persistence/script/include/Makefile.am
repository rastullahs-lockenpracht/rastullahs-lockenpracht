noinst_HEADERS = \
	AbstractMapNodeProcessor.h \
	EntityNodeProcessor.h \
	EnvironmentProcessor.h \
	GameObjectNodeProcessor.h \
	LightNodeProcessor.h \
	MapLoader.h \
	ParticleSystemNodeProcessor.h \
	PlaneNodeProcessor.h \
	TriggerFactory.h \
	Scene.h \
	SceneLoader.h \
	SceneManager.h \
	ScriptObjectMarker.h \
	ScriptPrerequisites.h \
	ScriptSubsystem.h \
	SoundNodeProcessor.h \
	WaypointProcessor.h \
	UnifiedFactory.h \
	ZoneProcessor.h

CLEANFILES = stdinc.h.gch
all-local: stdinc.h.gch
stdinc.h.gch: $(noinst_HEADERS) stdinc.h
	@echo Creating GCH file
	g++ -I. stdinc.h
