# Find Newton
#
# This module defines
#  NEWTON_FOUND
#  NEWTON_INCLUDE_DIR
#  NEWTON_LIBRARY
#
# Copyright (C) 2003-2009 Team Pantheon. http://www.team-pantheon.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# On a new cmake run, we do not need to be verbose
IF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)
	SET(NEWTON_FIND_QUIETLY TRUE)
ENDIF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)

IF(WIN32)

ELSE(WIN32)
	SET(NEWTON_INCLUDE_SEARCH_DIRS
	    /usr/include
	    /usr/local/include
	    /opt/newton/include
	    /opt/include INTERNAL)
	SET(NEWTON_LIBRARY_SEARCH_DIRS
	    /usr/lib
	    /usr/lib64
	    /usr/local/lib
	    /usr/local/lib64
	    /opt/newton/lib
	    /opt/newton/lib64 INTERNAL)
	SET(NEWTON_INC_DIR_SUFFIXES PATH_SUFFIXES newton)
ENDIF(WIN32)

IF (NOT NEWTON_FIND_QUIETLY)
	MESSAGE(STATUS "Checking for Newton")
ENDIF(NOT NEWTON_FIND_QUIETLY)

# Search for header files and libraries
FIND_PATH(NEWTON_INCLUDE_DIR Newton.h
          PATHS ${NEWTON_INCLUDE_SEARCH_DIRS}
          PATH_SUFFIXES ${NEWTON_INC_DIR_SUFFIXES})

FIND_LIBRARY(NEWTON_LIBRARY Newton
             PATHS ${NEWTON_LIBRARY_SEARCH_DIRS}
             PATH_SUFFIXES ${NEWTON_LIB_DIR_SUFFIXES})

IF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)
	SET(NEWTON_FOUND TRUE)
ENDIF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)

IF(NEWTON_FOUND)
	IF(NOT NEWTON_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries: ${NEWTON_LIBRARY}")
		MESSAGE(STATUS "  includes: ${NEWTON_INCLUDE_DIR}")
	ENDIF(NOT NEWTON_FIND_QUIETLY)
ELSE(NEWTON_FOUND)
	IF(NOT NEWTON_LIBRARY)
		MESSAGE(SEND_ERROR, "Newton library could not be found.")
	ENDIF(NOT NEWTON_LIBRARY)
	IF(NOT NEWTON_INCLUDE_DIR)
		MESSAGE(SEND_ERROR "Newton include files could not be found.")
	ENDIF(NOT NEWTON_INCLUDE_DIR)
ENDIF(NEWTON_FOUND)