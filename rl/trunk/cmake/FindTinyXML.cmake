# Find TinyXML (A small XML DOM library)
#
# This module defines
# TINYXML_FOUND       - TinyXML was found
# TINYXML_INCLUDE_DIR - Directory containing TinyXML header files
# TINYXML_LIBRARY     - Library name of TinyXML library
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# On a new cmake run, we do not need to be verbose
IF(TINYXML_INCLUDE_DIR AND TINYXML_LIBRARY)
	SET(TINYXML_FOUND_QUIETLY TRUE)
ENDIF(TINYXML_INCLUDE_DIR AND TINYXML_LIBRARY)

IF(WIN32)

ELSE(WIN32)
	IF(NOT TINYXML_FOUND)
		SET(TINYXML_INCLUDE_SEARCH_DIRS
		    /usr/include
		    /usr/local/include
		    /opt/include
		    /opt/tinyxml/include)
		SET(TINYXML_LIBRARY_SEARCH_DIRS
		    /usr/lib
		    /usr/lib64
		    /usr/local/lib
		    /usr/local/lib64
		    /opt/tinyxml/lib
		    /opt/tinyxml/lib64)
		SET(TINYXML_INC_DIR_SUFFIXES PATH_SUFFIXES tinyxml)
		FIND_PATH(TINYXML_INCLUDE_DIR tinyxml.h
		          PATHS ${TINYXML_INCLUDE_SEARCH_DIRS}
		          PATH_SUFFIXES ${TINYXML_INC_DIR_SUFFIXES})
		SET(TINYXML_LIBRARY tinyxml)
		FIND_PATH(TINYXML_LIBRARY_DIRS libtinyxml.a
		             PATHS ${TINYXML_LIBRARY_SEARCH_DIRS}
		             PATH_SUFFIXES ${TINYXML_LIB_DIR_SUFFIXES}})
	ENDIF(NOT TINYXML_FOUND)
ENDIF(WIN32)

SET(TINYXML_INCLUDE_DIR ${TINYXML_INCLUDE_DIR} CACHE STRING "Directory containing TinyXML header files")
SET(TINYXML_LIBRARY ${TINYXML_LIBRARY} CACHE STRING "Library name of TinyXML library")

IF(TINYXML_INCLUDE_DIR AND TINYXML_LIBRARY_DIRS)
	SET(TINYXML_FOUND TRUE)
ENDIF(TINYXML_INCLUDE_DIR AND TINYXML_LIBRARY_DIRS)

IF(TINYXML_FOUND)
	IF(NOT TINYXML_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries: ${TINYXML_LIBRARY} from ${TINYXML_LIBRARY_DIRS}")
		MESSAGE(STATUS "  includes: ${TINYXML_INCLUDE_DIR}")
	ENDIF(NOT TINYXML_FIND_QUIETLY)
ELSE(TINYXML_FOUND)
	IF(TINYXML_FIND_REQUIRED)
		IF(NOT TINYXML_LIBRARY_DIRS)
			MESSAGE(SEND_ERROR, "TinyXML library could not be found.")
		ENDIF(NOT TINYXML_LIBRARY)
		IF(NOT TINYXML_INCLUDE_DIR)
			MESSAGE(SEND_ERROR "TinyXML include files could not be found.")
		ENDIF(NOT TINYXML_INCLUDE_DIR)
	ENDIF(TINYXML_FIND_REQUIRED)
ENDIF(TINYXML_FOUND)
