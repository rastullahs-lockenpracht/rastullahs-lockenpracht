INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../ui/include -I../../core/include

lib_LTLIBRARIES = libRlDialog.la
libRlDialog_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(SDL_LIBS) $(LIBS) 
libRlDialog_la_SOURCES = \
	AimlParser.cpp \
	AimlProcessorManager.cpp \
	DialogSubsystem.cpp \
	Graphmaster.cpp \
	Match.cpp \
	NaturalLanguageProcessor.cpp \
	Nodemaster.cpp \
	Predicates.cpp \
	StringTokenizer.cpp \
	Utils.cpp \
	processors/ConditionProcessor.cpp 
