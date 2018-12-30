# - Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIRS
#  CEGUI_LIBRARIES, the libraries to link against to use CEGUI.
#  CEGUI_LIBRARY_DIRS, the location of the libraries
#  CEGUI_FOUND, If false, do not try to use CEGUI
#
# Copyright © 2007-2008, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIRS)
	SET(CEGUI_FIND_QUIETLY TRUE)
ENDIF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIRS)

#SET(OGRESDK $ENV{OGRE_HOME})
#STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
FILE(TO_CMAKE_PATH "$ENV{OGRE_HOME}" OGRESDK)
SET(CEGUI_INCLUDE_SEARCH_DIRS
	${INCLUDE_SEARCH_DIRS}
	${CMAKE_LIBRARY_PATH}
	/usr/include
	/usr/local/include
	/usr/local/include/cegui-0
	/usr/local/include/cegui-0/CEGUI
	/usr/local/include
	/usr/include/CEGUI
	/usr/local/include/CEGUI
	/opt/include/CEGUI
	/opt/CEGUI/include
	$ENV{OGRE_HOME}/samples/CEGUI
	$ENV{OGRE_HOME}/Dependencies/include
	$ENV{OGRE_HOME}/Dependencies/include/CEGUI
	CACHE STRING ""
)

SET(CEGUI_LIBRARY_SEARCH_DIRS
	${LIBRARY_SEARCH_DIRS}
	${CMAKE_LIBRARY_PATH}
	/usr/lib
	/usr/lib64
	/usr/local/lib
	/usr/local/lib64
	/opt/lib
	/opt/lib64
	/opt/CEGUI/lib
	/opt/CEGUI/lib64
	$ENV{OGRE_HOME}/lib
	$ENV{OGRE_HOME}/lib64
	CACHE STRING ""
)

IF (WIN32) #Windows
	MESSAGE(STATUS "Looking for CEGUI")
	FIND_PATH(CEGUI_INCLUDE_DIRS CEGUI.h ${CEGUI_INCLUDE_SEARCH_DIRS})
	FIND_LIBRARY(CEGUI_LIBRARIES debug CEGUIBase_d optimized CEGUIBase PATHS ${CEGUI_LIBRARY_SEARCH_DIRS})

# 	SET(OGRESOURCE $ENV{OGRE_NEW})
# 	IF (OGRESDK)
# 		MESSAGE(STATUS "Using CEGUI in OGRE SDK")
# 		SET(OGRESDK $ENV{OGRE_HOME})
# 		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}" )
# 		SET(CEGUI_INCLUDE_DIRS ${OGRESDK}/include/CEGUI)
# 		SET(CEGUI_LIBRARY_DIRS ${OGRESDK}/lib)
# 		SET(CEGUI_LIBRARIES debug CEGUIBase_d optimized CEGUIBase)
# 	ELSEIF (OGRESOURCE)
# 		MESSAGE(STATUS "Using CEGUI in OGRE dependencies")
# 		SET(CEGUI_INCLUDE_DIRS C:/ogrenew/Dependencies/include C:/ogrenew/Dependencies/include/CEGUI)
# 		SET(CEGUI_LIBRARY_DIRS C:/ogrenew/Dependencies/lib/Release C:/ogrenew/Dependencies/lib/Debug)
# 		SET(CEGUI_LIBRARIES debug CEGUIBase_d optimized CEGUIBase)
# 	ELSE (OGRESDK)
# 		IF (NOT DEPENDENCIES_PATH)
# 			MESSAGE(STATUS "Please set DEPENDENCIES_PATH (the directory containing 'common' and 'dependencies')")
# 		ELSE (NOT DEPENDENCIES_PATH)
# 			SET(CEGUI_INCLUDE_DIRS ${DEPENDENCIES_PATH}/dependencies/Ogre/CEGUI)
# 			SET(CEGUI_LIBRARIES optimized CEGUIBase debug CEGUIBase_d)
# 			SET(CEGUI_LIBRARY_DIRS optimized ${DEPENDENCIES_PATH}/dependencies/lib/release debug ${DEPENDENCIES_PATH}/dependencies/lib/debug)
# 		ENDIF (NOT DEPENDENCIES_PATH)
# 	ENDIF (OGRESDK)
ELSE (WIN32) #Unix
	SET(CEGUI_INCLUDE_DIRS ${CEGUI_INCLUDE_DIRSS})
	SET(CEGUI_LIBRARY_DIRS ${CEGUI_LIBDIR})
	SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES})

	FIND_PACKAGE(PkgConfig)
	IF(PKG_CONFIG_FOUND)
		PKG_SEARCH_MODULE(CEGUI CEGUI)
		SET(CEGUI_INCLUDE_DIRS ${CEGUI_INCLUDE_DIRS})
		SET(CEGUI_LIBRARY_DIRS ${CEGUI_LIBDIR})
		SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CACHE STRING "")
	ENDIF(PKG_CONFIG_FOUND)
	IF(NOT CEGUI_FOUND) #If PkgConfig couldn't find OGRE...
		IF (NOT CEGUI_FIND_QUIETLY)
			MESSAGE(STATUS " Manually searching for CEGUI")
			ENDIF (NOT CEGUI_FIND_QUIETLY)
		FIND_PATH(CEGUI_INCLUDE_DIRS Ogre.h ${CEGUI_INCLUDE_SEARCH_DIRS})
		FIND_LIBRARY(CEGUI_LIBRARIES CEGUIBase PATHS ${CEGUI_LIBRARY_SEARCH_DIRS})
		find_path(CEGUI_INCLUDE_DIRS
				CEGUI/CEGUI.h
				${CEGUI_INCLUDE_SEARCH_DIRS})
		find_library(CEGUI_LIBS_MANUAL
				NAMES CEGUIBase CEGUIBase-0
				PATHS ${CEGUI_LIBRARY_SEARCH_DIRS})
		SET(CEGUI_LIBRARIES ${CEGUI_LIBS_MANUAL})
	ENDIF(NOT CEGUI_FOUND)
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(CEGUI_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(CEGUI_LIBRARIES)

SET(CEGUI_INCLUDE_DIRS ${CEGUI_INCLUDE_DIRS})
SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES})
SET(CEGUI_LIBRARY_DIRS ${CEGUI_LIBRARY_DIRS})

MARK_AS_ADVANCED(CEGUI_INCLUDE_DIRS CEGUI_LIBRARIES CEGUI_LIBRARY_DIRS)

IF (CEGUI_INCLUDE_DIRS AND CEGUI_LIBRARIES)
	SET(CEGUI_FOUND TRUE)
ENDIF (CEGUI_INCLUDE_DIRS AND CEGUI_LIBRARIES)

IF (CEGUI_FOUND)
	IF (NOT CEGUI_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${CEGUI_LIBRARIES} from ${CEGUI_LIBRARY_DIRS}")
		MESSAGE(STATUS "  includes  : ${CEGUI_INCLUDE_DIRS}")
	ENDIF (NOT CEGUI_FIND_QUIETLY)
ELSE (CEGUI_FOUND)
	IF (CEGUI_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find CEGUI. Make sure you have the CEGUI development headers are installed. Otherwise, try setting LIBRARY_SEARCH_DIRS and INCLUDE_SEARCH_DIRS to the place CEGUI was installed with -DLIBRARY_SEARCH_DIRS=<path/to/lib> -DINCLUDE_SEARCH_DIRS=<path/to/include>")
	ENDIF (CEGUI_FIND_REQUIRED)
ENDIF (CEGUI_FOUND)
