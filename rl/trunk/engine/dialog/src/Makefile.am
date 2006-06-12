INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../include/predicates -I../../common/include -I../../ui/include -I../../core/include -I../../rules/include 

lib_LTLIBRARIES = libRlDialog.la
libRlDialog_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(LIBS) $(CEGUI_LIBS) 
libRlDialog_la_SOURCES = \
	AimlNodeImplRl.cpp \
    AimlParserImplRl.cpp \
    ContextConditionProcessor.cpp \
    ContextInterpreter.cpp \
	DialogCharacter.cpp \
	DialogOption.cpp \
	DialogResponse.cpp \
	DialogSubsystem.cpp \
	ScriptProcessor.cpp \
	predicates/CreaturePredicates.cpp \
	predicates/EigenschaftsProbePredicates.cpp \
	predicates/EigenschaftsWertPredicates.cpp \
	predicates/QuestPredicates.cpp \
	predicates/ScriptPredicates.cpp \
	predicates/TalentProbePredicates.cpp \
	predicates/TalentWertPredicates.cpp
	
