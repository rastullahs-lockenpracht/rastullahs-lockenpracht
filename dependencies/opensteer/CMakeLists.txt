PROJECT(OpenSteer)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# Include required cmake macros
INCLUDE(FeatureSummary)

# === CPack packaging ===
  #Include packaging information
   INCLUDE(InstallRequiredSystemLibraries)

  #Enable absolute paths
   SET(CPACK_SET_DESTDIR "ON")

  # General package-information [mandatory]
   SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "opensteer info")
   SET(CPACK_PACKAGE_VENDOR "Team Pantheon")
   SET(CPACK_PACKAGE_CONTACT "contact@rastullahs-lockenpracht.de")

  # General package-information [optional]
   SET(CPACK_PACKAGE_NAME "opensteer")
   #SET(CPACK_PACKAGE_FILE_NAME "name-version-something-else-to-add")

  # Package version information [mandatory]
   SET(CPACK_PACKAGE_VERSION_MAJOR "0")
   SET(CPACK_PACKAGE_VERSION_MINOR "9")
   SET(CPACK_PACKAGE_VERSION_PATCH "0")

  # Architecture and Platform [mandatory]
   IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
     SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
   ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
     SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
   ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")

  # ReadMe/License/Copyright etc... [optional]
   #SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
   #SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")

# === CMake Main ===


# Version information
SET(OpenSteer_MAJOR_VERSION 0)
SET(OpenSteer_MINOR_VERSION 9)
SET(OpenSteer_PATCH_VERSION 0)

# Add directory with OpenSteer's own macro definitions
# (checked before ${CMAKE_ROOT}/Modules)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Check for required components
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(OGRE 1.4 REQUIRED)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${OGRE_INCLUDE_DIRS})

SET(OpenSteer_LIB_SRCS
src/lq.c
src/Clock.cpp
src/Color.cpp
src/Obstacle.cpp
src/OldPathway.cpp
src/Path.cpp
src/Pathway.cpp
src/PlugIn.cpp
src/PolylineSegmentedPath.cpp
src/PolylineSegmentedPathwaySingleRadius.cpp
src/SegmentedPath.cpp
src/SegmentedPathway.cpp
src/SimpleVehicle.cpp
src/Vec3.cpp
src/Vec3Utilities.cpp)

ADD_LIBRARY(OpenSteer SHARED ${OpenSteer_LIB_SRCS})

TARGET_LINK_LIBRARIES(OpenSteer ${OGRE_LIBRARIES})

SET_TARGET_PROPERTIES(OpenSteer PROPERTIES
	VERSION ${OpenSteer_MAJOR_VERSION}.${OpenSteer_MINOR_VERSION}.${OpenSteer_PATCH_VERSION}
	SOVERSION ${OpenSteer_MAJOR_VERSION}
	DEFINE_SYMBOL NOT_OPENSTEERDEMO) 

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/opensteer.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/opensteer.pc)

INSTALL(TARGETS OpenSteer
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

INSTALL(FILES include/OpenSteer/AbstractVehicle.h include/OpenSteer/Annotation.h
include/OpenSteer/Clock.h include/OpenSteer/Color.h include/OpenSteer/LocalSpace.h
include/OpenSteer/lq.h include/OpenSteer/Obstacle.h include/OpenSteer/OldPathway.h
include/OpenSteer/Path.h include/OpenSteer/Pathway.h include/OpenSteer/PlugIn.h
include/OpenSteer/PolylineSegmentedPath.h include/OpenSteer/PolylineSegmentedPathwaySegmentRadii.h
include/OpenSteer/PolylineSegmentedPathwaySingleRadius.h include/OpenSteer/Proximity.h
include/OpenSteer/QueryPathAlike.h include/OpenSteer/QueryPathAlikeBaseDataExtractionPolicies.h
include/OpenSteer/QueryPathAlikeMappings.h include/OpenSteer/QueryPathAlikeUtilities.h
include/OpenSteer/SegmentedPath.h include/OpenSteer/SegmentedPathAlikeUtilities.h
include/OpenSteer/SegmentedPathway.h include/OpenSteer/SharedPointer.h
include/OpenSteer/StandardTypes.h include/OpenSteer/SteerLibrary.h
include/OpenSteer/UnusedParameter.h include/OpenSteer/Utilities.h
include/OpenSteer/Vec3.h include/OpenSteer/Vec3Utilities.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/OpenSteer)

IF(NOT WIN32)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/opensteer.pc
	        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
ENDIF(NOT WIN32)

# === CPack package creation / installation ===

  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
  #SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "opensteer 0.9.0")
  IF(WIN32 AND NOT UNIX)
    # TEMPLATE - Not Tested!
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\opensteer_setup.exe")
    SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} opensteer")
    SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.rastullahs-lockenpracht.de")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.wiki.team-pantheon.de")
    SET(CPACK_NSIS_CONTACT "teall.hartmann@web.de")
    SET(CPACK_NSIS_MODIFY_PATH ON)
  ELSE(WIN32 AND NOT UNIX)
    SET(CPACK_STRIP_FILES "bin/opensteer")
    SET(CPACK_SOURCE_STRIP_FILES "")
  ENDIF(WIN32 AND NOT UNIX)

#SET(CPACK_PACKAGE_EXECUTABLES "opensteer" "opensteer")

INCLUDE(CPack)

PRINT_ENABLED_FEATURES()
