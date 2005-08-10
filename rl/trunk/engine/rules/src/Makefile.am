INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include

lib_LTLIBRARIES = libRlRules.la
libRlRules_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(SDL_LIBS) $(LIBS)
libRlRules_la_SOURCES = \
	Action.cpp \
	ActionManager.cpp \
	Container.cpp \
	Creature.cpp \
	Date.cpp \
	DsaDataLoader.cpp \
	DsaManager.cpp \
	Eigenschaft.cpp \
	GameObject.cpp \
	Item.cpp \
	Kampftechnik.cpp \
	KampftechnikCsvIterator.cpp \
	ObjectStateChangeEvent.cpp \
	ObjectStateChangeEventSource.cpp \
	ObjectStateChangeListener.cpp \
	Person.cpp \
	RulesSubsystem.cpp \
	Talent.cpp \
	TalentCsvIterator.cpp \
	Talentgruppe.cpp 
	
