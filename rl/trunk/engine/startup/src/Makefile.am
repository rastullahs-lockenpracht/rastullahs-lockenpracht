INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(SDL_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../../core/include -I../../rules/include -I../../sound/include -I../../ui/include -I../../dialog/include -I../../script/include

bin_PROGRAMS = rastullah
rastullah_SOURCES = RastullahMain.cpp
rastullah_LDFLAGS = -L../../script/src -L../../core/src  -L../../common/src -L../../dialog/src -L../../ui/src -L../../rules/src -L../../sound/src
rastullah_LDADD = $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(SDL_LIBS) $(OGRE_LIBS) \
	 -lRlCore -lRlScript -lRlCommon -lRlDialog -lRlUi -lRlRules -lRlSound -logg -lvorbisfile -lvorbis -lboost_thread



