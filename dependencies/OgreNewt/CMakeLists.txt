PROJECT(OgreNewt)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)

# Package and version information
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ogre Bindings for the Newton Physics SDK")
SET(CPACK_PACKAGE_VENDOR "Team Pantheon")
SET(CPACK_PACKAGE_CONTACT "contact@rastullahs-lockenpracht.de")

SET(OgreNewt_MAJOR_VERSION 2)
SET(OgreNewt_MINOR_VERSION 0)
SET(OgreNewt_PATCH_VERSION 0)

SET(CPACK_PACKAGE_VERSION_MAJOR ${OgreNewt_MAJOR_VERSION})
SET(CPACK_PACKAGE_VERSION_MINOR ${OgreNewt_MINOR_VERSION})
SET(CPACK_PACKAGE_VERSION_PATCH ${OgreNewt_PATCH_VERSION})

IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
	SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")

SET(CPACK_SOURCE_GENERATOR "TBZ2;ZIP")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
"${CMAKE_PROJECT_NAME}-${OgreNewt_MAJOR_VERSION}.${OgreNewt_MINOR_VERSION}.${OgreNewt_PATCH_VERSION}")

IF(WIN32 AND NOT UNIX)
	# TEMPLATE - Not Tested!
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\OgreNewt2_setup.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} OgreNewt2")
	SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.rastullahs-lockenpracht.de")
	SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.wiki.team-pantheon.de")
	SET(CPACK_NSIS_CONTACT "contact@rastullahs-lockenpracht.de")
	SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
	SET(CPACK_STRIP_FILES "bin/OgrewNewt")
	SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)

#Enable absolute paths
SET(CPACK_SET_DESTDIR "ON")

# Include required cmake macros
INCLUDE(FeatureSummary)
INCLUDE(InstallRequiredSystemLibraries)
INCLUDE(CPack)

# Add directory with OgreNewt's own macro definitions
# (checked before ${CMAKE_ROOT}/Modules)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Check for required components
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(OGRE 1.4 REQUIRED)
FIND_PACKAGE(Newton 2.0 REQUIRED)

# Compile settings for build types
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-inline")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g3")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/inc
${OGRE_INCLUDE_DIRS}
${NEWTON_INCLUDE_DIR})

SET(OgreNewt_LIB_SRCS
src/OgreNewt_BasicJoints.cpp
src/OgreNewt_Body.cpp
src/OgreNewt_BodyInAABBIterator.cpp
src/OgreNewt_Collision.cpp
src/OgreNewt_CollisionPrimitives.cpp
src/OgreNewt_CollisionSerializer.cpp
src/OgreNewt_ContactCallback.cpp
src/OgreNewt_ContactJoint.cpp
src/OgreNewt_Debugger.cpp
src/OgreNewt_Joint.cpp
src/OgreNewt_MaterialID.cpp
src/OgreNewt_MaterialPair.cpp
src/OgreNewt_RayCast.cpp
src/OgreNewt_Tools.cpp
src/OgreNewt_Vehicle.cpp
src/OgreNewt_World.cpp
src/OgreNewt_PlayerController.cpp)

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

INSTALL(FILES inc/OgreNewt_BasicJoints.h
inc/OgreNewt_Body.h inc/OgreNewt_Collision.h
inc/OgreNewt_CollisionPrimitives.h inc/OgreNewt_CollisionSerializer.h
inc/OgreNewt_ContactCallback.h inc/OgreNewt_Debugger.h inc/OgreNewt.h
inc/OgreNewt_Joint.h inc/OgreNewt_MaterialID.h inc/OgreNewt_MaterialPair.h
inc/OgreNewt_Prerequisites.h inc/OgreNewt_RayCast.h inc/OgreNewt_Tools.h
inc/OgreNewt_Vehicle.h inc/OgreNewt_World.h inc/OgreNewt_PlayerController.h
inc/OgreNewt_ContactJoint.h inc/OgreNewt_BodyInAABBIterator.h
inc/OgreNewt_BasicFrameListener.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/OgreNewt)

IF(NOT WIN32)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/OgreNewt.pc
	        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
ENDIF(NOT WIN32)

PRINT_ENABLED_FEATURES()
