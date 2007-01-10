
SUBDIRS = include

INCLUDES = -I./include/Dsa4Wz 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = Dsa4Wz.pc


doc:
	doxygen
	
lib_LTLIBRARIES = libDsa4Wz.la
libDsa4Wz_la_LIBADD = $(XERCESC_LIBS) $(STLPORT_LIBS)
libDsa4Wz_la_LDFLAGS = $(XERCESC_LDFLAGS) $(STLPORT_LDFLAGS)
libDsa4Wz_la_SOURCES = \
	src/detconfig.cpp \
	src/LaderXerces.cpp \
	src/Log.cpp \
	src/PersistenzManagerXerces.cpp \
	src/Update.cpp \
	src/AbgeleiteterWert.cpp \
	src/Charakteristikum.cpp \
	src/Definition.cpp \
	src/Eigenschaft.cpp \
	src/Hole.cpp \
	src/Kampftechnik.cpp \
	src/Kultur.cpp \
	src/Modifikator.cpp \
	src/NDSABasis.cpp \
	src/NDSAUtil.cpp \
	src/PersistenzManager.cpp \
	src/Person.cpp \
	src/PersonenKomponente.cpp \
	src/PersonHandler.cpp \
	src/PersonHandlerGenerator.cpp \
	src/Profession.cpp \
	src/Rasse.cpp \
	src/Regeln.cpp \
	src/Sonderfertigkeit.cpp \
	src/SpracheSchrift.cpp \
	src/Talent.cpp \
	src/Texte.cpp \
	src/Voraussetzung.cpp \
	src/Wuerfel.cpp \
	src/Zauber.cpp 
	
