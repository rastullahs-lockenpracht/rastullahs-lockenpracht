INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include

lib_LTLIBRARIES = libRlRules.la
libRlRules_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(RUBY_LIBS) $(OGRE_LIBS) $(SDL_LIBS) $(LIBS) -Wl,--defsym -Wl,Init_libRlRules=Init_RlRules 
libRlRules_la_SOURCES = \
	Action.cpp \
	ActionManager.cpp \
	Creature.cpp \
	Date.cpp \
	DsaManager.cpp \
	Eigenschaft.cpp \
	GameObject.cpp \
	Item.cpp \
	Kampftechnik.cpp \
	RulesSubsystem.cpp \
	Talent.cpp \
	Talentgruppe.cpp \
	../swig/RlRules.cxx \
	TalentCsvIterator.cpp \
	KampftechnikCsvIterator.cpp
	
