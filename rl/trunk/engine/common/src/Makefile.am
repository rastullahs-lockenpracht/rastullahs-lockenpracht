INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUI_CFLAGS) -I../include 

lib_LTLIBRARIES = libRlCommon.la
libRlCommon_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(RUBY_LIBS) $(OGRE_LIBS) $(SDL_LIBS) $(CEGUI_LIBS) $(LIBS)
libRlCommon_la_SOURCES = \
  CsvFile.cpp \
  XmlHelper.cpp \
  EventSource.cpp \
  EventObject.cpp \
  EventListener.cpp \
  EventCaster.cpp 