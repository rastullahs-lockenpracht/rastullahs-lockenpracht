INCLUDES = $(RUBY_CFLAGS) $(SDL_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../core/include -I../../rules/include -I../../sound/include -I../../ui/include 

bin_PROGRAMS = rastullah
rastullah_SOURCES = RastullahMain.cpp
rastullah_LDFLAGS = -L../../core/src  -L../../common/src -L../../dialog/src -L../../ui/src -L../../rules/src -L../../sound/src 
rastullah_LDADD = $(LIBS) $(CEGUI_LIBS) $(SWIG_RUBY_LIBS) $(SDL_LIBS) $(OGRE_LIBS) $(OGRE_PLUGINDIR)/Plugin_GuiElements.so \
	 -lruby -lRlCore -lRlCommon -lRlDialog -lRlUi -lRlRules -lRlSound -logg -lvorbisfile -lvorbis


