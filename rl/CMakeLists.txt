PROJECT(RL)

CMAKE_MINIMUM_REQUIRED(VERSION 3.3.0 FATAL_ERROR)

# Include required cmake macros
INCLUDE(FeatureSummary)

# Version information
SET(RL_MAJOR_VERSION 0)
SET(RL_MINOR_VERSION 3)
SET(RL_PATCH_VERSION 0)

# Add directory with RL's own macro definitions
# (checked before ${CMAKE_ROOT}/Modules)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set convenience variables for Rastullah's own include directories
SET(RL_AI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/ai/include INTERNAL)
SET(RL_COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/common/include INTERNAL)
SET(RL_CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/core/include INTERNAL)
SET(RL_RULES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/rules/include INTERNAL)
SET(RL_SCRIPT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/script/include INTERNAL)
SET(RL_UI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/engine/ui/include INTERNAL)

# Check for required components
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(tinyxml2 REQUIRED)
FIND_PACKAGE(OGRE 1.6 REQUIRED)
FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(CEGUIOGRE 1.6 REQUIRED)
FIND_PACKAGE(OgreNewt 2.0 REQUIRED)
FIND_PACKAGE(Newton 2.0 REQUIRED)
FIND_PACKAGE(MeshMagick 0.5 REQUIRED)
FIND_PACKAGE(Ruby REQUIRED)
FIND_PACKAGE(Boost 1.34 REQUIRED filesystem)
FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
IF (NOT SWIG_DIR MATCHES -rl$)
	SET_FEATURE_INFO(SWIG "" "" "You use an unsupported version of SWIG.")
ENDIF(NOT SWIG_DIR MATCHES -rl$)

# Compile settings for build types
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -D_DEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -D_DEBUG")

# Path options
SET(BINDIR "bin" CACHE STRING "Where to install binaries")
SET(DATAROOTDIR "${CMAKE_INSTALL_PREFIX}/share" CACHE STRING "Sets the root of data directories to a non-default location")
SET(DATADIRNAME "rastullah" CACHE STRING "change the name of the directory for the read-only architecture-independent game data")

# Game options

# Include subdirectories
ADD_SUBDIRECTORY(engine)
#ADD_SUBDIRECTORY(plugins)

PRINT_ENABLED_FEATURES()
