INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include

lib_LTLIBRARIES = libRlRules.la
libRlRules_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(SDL_LIBS) $(LIBS)
libRlRules_la_SOURCES = \
	Action.cpp \
	ActionManager.cpp \
	Creature.cpp \
	Person.cpp \
	Date.cpp \
	DsaDataLoader.cpp \
	DsaManager.cpp \
	Eigenschaft.cpp \
	GameObject.cpp \
	Item.cpp \
	Kampftechnik.cpp \
	RulesSubsystem.cpp \
	Talent.cpp \
	Talentgruppe.cpp \
	TalentCsvIterator.cpp \
	KampftechnikCsvIterator.cpp
	
