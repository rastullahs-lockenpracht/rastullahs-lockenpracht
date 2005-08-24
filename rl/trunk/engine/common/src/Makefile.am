INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUI_CFLAGS) -I../include

lib_LTLIBRARIES = libRlCommon.la
libRlCommon_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(LIBS)
libRlCommon_la_SOURCES = \
  CsvFile.cpp \
  EventObject.cpp \
  EventSource.cpp \
  Logger.cpp \
  XmlHelper.cpp \
  XmlResource.cpp \
  XmlResourceManager.cpp   
