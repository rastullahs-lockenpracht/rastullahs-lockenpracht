PROJECT(OgreNewt_demos)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)

# Include required cmake macros
INCLUDE(FeatureSummary)

# Package and version information
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ogre Bindings for the Newton Physics SDK - Demos")
SET(CPACK_PACKAGE_VENDOR "Team Pantheon")
SET(CPACK_PACKAGE_CONTACT "contact@rastullahs-lockenpracht.de")

# Add directory with OgreNewt's own macro definitions
# (checked before ${CMAKE_ROOT}/Modules)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Check for required components
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(OGRE 1.6 REQUIRED)
FIND_PACKAGE(OgreNewt 2.0 REQUIRED)
FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(CEGUIOGRE 1.6 REQUIRED)

# Compile settings for build types
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-inline -D_DEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g3 -D_DEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
SET(CMAKE_CXX_FLAGS "-DTIXML_USE_STL")

# We need the ogre-samples directory
SET(OGRE_SAMPLES_DIR ${CMAKE_SOURCE_DIR}/../../ogrenew/Samples CACHE STRING "The OGRE-Samples directory")
FIND_PATH(OGRE_SAMPLES_INCLUDE_DIR ExampleApplication.h ${OGRE_SAMPLES_DIR}/Common/include)
IF (OGRE_SAMPLES_INCLUDE_DIR)
    MESSAGE(STATUS " Ogre-Samples directory: ${OGRE_SAMPLES_DIR}")
ELSE (OGRE_SAMPLES_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "Ogre-Samples directory not found. Please specify the correct location of the Ogre-Samples directory with -D OGRE_SAMPLES_DIR=\"<dir>\".")
ENDIF (OGRE_SAMPLES_INCLUDE_DIR)

INCLUDE_DIRECTORIES(
${OGRE_INCLUDE_DIRS}
${NEWTON_INCLUDE_DIR}
${OIS_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIRS}
${OGRE_SAMPLES_INCLUDE_DIR}
${CEGUI_INCLUDE_DIRS}
${CEGUIOGRE_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/tinyxml)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(TINYXML_SRCS
../tinyxml/tinystr.cpp
../tinyxml/tinyxml.cpp
../tinyxml/tinyxmlerror.cpp
../tinyxml/tinyxmlparser.cpp)

ADD_SUBDIRECTORY(Demo01_TheBasics)
ADD_SUBDIRECTORY(Demo02_Joints)
ADD_SUBDIRECTORY(Demo03_CollisionCallbacks)
ADD_SUBDIRECTORY(Demo04_Raycasting)
#ADD_SUBDIRECTORY(Demo05_SimpleVehicle)
ADD_SUBDIRECTORY(Demo06_SimpleBuoyancy)
ADD_SUBDIRECTORY(Demo07_CustomJoints)
ADD_SUBDIRECTORY(Demo08_RagdollExample)

PRINT_ENABLED_FEATURES()
