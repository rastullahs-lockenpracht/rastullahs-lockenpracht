INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include

lib_LTLIBRARIES = libRlUiComponents.la
libRlUiComponents_la_LDFLAGS = $(RUBY_LIBS) $(LIBS) \
	$(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS)
libRlUiComponents_la_SOURCES = \
	ListboxImageItem.cpp \
	ListboxWrappedTextItem.cpp
	