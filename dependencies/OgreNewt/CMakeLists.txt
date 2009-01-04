PROJECT(OgreNewt)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)

# Include required cmake macros
INCLUDE(FeatureSummary)

# Version information
SET(OgreNewt_MAJOR_VERSION 1)
SET(OgreNewt_MINOR_VERSION 5)
SET(OgreNewt_PATCH_VERSION 3)

# Add directory with OgreNewt's own macro definitions
# (checked before ${CMAKE_ROOT}/Modules)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Check for required components
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(OGRE 1.4 REQUIRED)
FIND_PACKAGE(Newton 1.53 EXACT REQUIRED)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include/OgreNewt
${OGRE_INCLUDE_DIRS}
${NEWTON_INCLUDE_DIR})

SET(OgreNewt_LIB_SRCS
src/OgreNewt_BasicJoints.cpp
src/OgreNewt_Body.cpp
src/OgreNewt_Collision.cpp
src/OgreNewt_CollisionPrimitives.cpp
src/OgreNewt_ContactCallback.cpp
src/OgreNewt_Debugger.cpp
src/OgreNewt_Joint.cpp
src/OgreNewt_MaterialID.cpp
src/OgreNewt_MaterialPair.cpp
src/OgreNewt_RayCast.cpp
src/OgreNewt_Tools.cpp
src/OgreNewt_TreeCollisionSerializer.cpp
src/OgreNewt_Vehicle.cpp
src/OgreNewt_World.cpp)

ADD_LIBRARY(OgreNewt SHARED ${OgreNewt_LIB_SRCS})

TARGET_LINK_LIBRARIES(OgreNewt ${OGRE_LIBRARIES} ${NEWTON_LIBRARY})

SET_TARGET_PROPERTIES(OgreNewt PROPERTIES
	VERSION ${OgreNewt_MAJOR_VERSION}.${OgreNewt_MINOR_VERSION}.${OgreNewt_PATCH_VERSION}
	SOVERSION ${OgreNewt_MAJOR_VERSION}
	DEFINE_SYMBOL _OGRENEWT_EXPORTS)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/OgreNewt.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/OgreNewt.pc)

INSTALL(TARGETS OgreNewt
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

INSTALL(FILES include/OgreNewt/FastDelegate.h include/OgreNewt/line3D.h
include/OgreNewt/OgreNewt_BasicJoints.h include/OgreNewt/OgreNewt_Body.h
include/OgreNewt/OgreNewt_BodyIterator.h include/OgreNewt/OgreNewt_Collision.h
include/OgreNewt/OgreNewt_CollisionPrimitives.h include/OgreNewt/OgreNewt_ContactCallback.h
include/OgreNewt/OgreNewt_Debugger.h include/OgreNewt/OgreNewt.h
include/OgreNewt/OgreNewt_Joint.h include/OgreNewt/OgreNewt_MaterialID.h
include/OgreNewt/OgreNewt_MaterialPair.h include/OgreNewt/OgreNewt_Prerequisites.h
include/OgreNewt/OgreNewt_RayCast.h include/OgreNewt/OgreNewt_Tools.h
include/OgreNewt/OgreNewt_TreeCollisionSerializer.h include/OgreNewt/OgreNewt_Vehicle.h
include/OgreNewt/OgreNewt_World.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/OgreNewt)

IF(NOT WIN32)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/OgreNewt.pc
	        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
ENDIF(NOT WIN32)

PRINT_ENABLED_FEATURES()
