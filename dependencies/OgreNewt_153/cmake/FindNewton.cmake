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

SET(NEWTON_INCLUDE_DIR ${NEWTON_INCLUDE_DIR} CACHE STRING "Directory containing Newton header files")
SET(NEWTON_LIBRARY ${NEWTON_LIBRARY} CACHE STRING "Library name of Newton library")

IF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)
	SET(NEWTON_FOUND TRUE)
ENDIF(NEWTON_INCLUDE_DIR AND NEWTON_LIBRARY)

IF(NEWTON_INCLUDE_DIR)
	# Extract the library version from the header file
	FILE(READ "${NEWTON_INCLUDE_DIR}/Newton.h" _NEWTON_NEWTON_H_CONTENTS)
	# Check if header contains version information
	STRING(REGEX MATCH "NEWTON_MAJOR_VERSION"
	       NEWTON_VERSION_EXISTS "${_NEWTON_NEWTON_H_CONTENTS}")
	IF("${NEWTON_VERSION_EXISTS}" STREQUAL "NEWTON_MAJOR_VERSION")
		MESSAGE(STATUS "We should be here if NEWTON_VERSION_EXISTS is not empty")
		STRING(REGEX REPLACE ".*#define NEWTON_MAJOR_VERSION ([0-9]+).*" "\\1"
		       NEWTON_MAJOR_VERSION "${_NEWTON_NEWTON_H_CONTENTS}")
		STRING(REGEX REPLACE ".*#define NEWTON_MINOR_VERSION ([0-9]+).*" "\\1"
		       NEWTON_MINOR_VERSION "${_NEWTON_NEWTON_H_CONTENTS}")
		SET(NEWTON_VERSION "${NEWTON_MAJOR_VERSION}.${NEWTON_MINOR_VERSION}")
	ELSE("${NEWTON_VERSION_EXISTS}" STREQUAL "NEWTON_MAJOR_VERSION")
		# We assume Newton 1.53
		SET(NEWTON_VERSION "1.53")
	ENDIF("${NEWTON_VERSION_EXISTS}" STREQUAL "NEWTON_MAJOR_VERSION")

	# Compare installed and required version
	IF(Newton_FIND_VERSION)
		# Major and minor version are required
		IF(NOT DEFINED Newton_FIND_VERSION_MINOR)
			MESSAGE(SEND_ERROR "When requesting a specific version of Newton, you must provide at least the major and minor version numbers, e.g., 2.0")
		ENDIF(NOT DEFINED Newton_FIND_VERSION_MINOR)

		# Exact version match requested
		IF(Newton_FIND_VERSION_EXACT)
			IF(NOT "${NEWTON_VERSION}" VERSION_EQUAL "${Newton_FIND_VERSION}")
				SET(NEWTON_FOUND FALSE)
				MESSAGE(SEND_ERROR "Required version of Newton ${Newton_FIND_VERSION}, but you have Newton ${NEWTON_VERSION} installed.")
			ENDIF(NOT "${NEWTON_VERSION}" VERSION_EQUAL "${Newton_FIND_VERSION}")
		ELSE(Newton_FIND_VERSION_EXACT)
			IF("${NEWTON_VERSION}" VERSION_LESS "${Newton_FIND_VERSION}")
				SET(NEWTON_FOUND FALSE)
				MESSAGE(STATUS "Detected version of Newton ${NEWTON_VERSION} is too old. At least version ${Newton_FIND_VERSION} is required.")
			ENDIF("${NEWTON_VERSION}" VERSION_LESS "${Newton_FIND_VERSION}")
		ENDIF(Newton_FIND_VERSION_EXACT)
	ENDIF(Newton_FIND_VERSION)

ENDIF(NEWTON_INCLUDE_DIR)

IF(NEWTON_FOUND)
	IF(NOT NEWTON_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries: ${NEWTON_LIBRARY}")
		MESSAGE(STATUS "  includes: ${NEWTON_INCLUDE_DIR}")
		MESSAGE(STATUS "  version: ${NEWTON_VERSION}")
	ENDIF(NOT NEWTON_FIND_QUIETLY)
ELSE(NEWTON_FOUND)
	IF(NOT NEWTON_LIBRARY)
		MESSAGE(SEND_ERROR, "Newton library could not be found.")
	ENDIF(NOT NEWTON_LIBRARY)
	IF(NOT NEWTON_INCLUDE_DIR)
		MESSAGE(SEND_ERROR "Newton include files could not be found.")
	ENDIF(NOT NEWTON_INCLUDE_DIR)
ENDIF(NEWTON_FOUND)
