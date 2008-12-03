# Find OgreNewt (Newton physic library bindings for OGRE)
#
# This module defines
# OGRENEWT_FOUND
# OGRENEWT_INCLUDE_DIR
# OGRENEWT_LIBRARY
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# On a new cmake run, we do not need to be verbose
IF(OGRENEWT_INCLUDE_DIRS AND OGRENEWT_LIBRARY)
	SET(OGRENEWT_FOUND_QUIETLY TRUE)
ENDIF(OGRENEWT_INCLUDE_DIRS AND OGRENEWT_LIBRARY)

IF(WIN32)

ELSE(WIN32)
	IF(PKG_CONFIG_FOUND)
		PKG_SEARCH_MODULE(OGRENEWT OgreNewt)
	ENDIF(PKG_CONFIG_FOUND)
ENDIF(WIN32)