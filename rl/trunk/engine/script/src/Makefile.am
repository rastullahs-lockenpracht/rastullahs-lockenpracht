INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(SDL_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) -Wl,--defsym -Wl,Init_libRlScript=Init_RlScript
libRlScript_la_SOURCES = \
	../swig/RlExports.cxx 
