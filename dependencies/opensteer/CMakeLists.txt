PROJECT(OpenSteer)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# Include required cmake macros
INCLUDE(FeatureSummary)

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

PRINT_ENABLED_FEATURES()
