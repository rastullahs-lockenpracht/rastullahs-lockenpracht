INCLUDES = $(CEGUI_CFLAGS) $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include -I../../sound/include -I../../dialog/include -I../../rules/include -I../../ui/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = $(CEGUI_LIBS) $(SDL_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)  \
	-Wl,--defsym -Wl,Init_libRlScript=Init_RlScript \
	-lRlCore -lRlCommon -lRlDialog -lRlUi -lRlRules -lRlSound \
	-L../../core/src  -L../../common/src -L../../dialog/src -L../../ui/src -L../../rules/src -L../../sound/src

libRlScript_la_SOURCES = \
  ScriptSubsystem.cpp \
  ScriptObjectRepository.cpp \
  TimerManager.cpp \
  TimerTask.cpp \
	../swig/RlExports.cxx 
