PROJECT(RL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Cmake handles escaping of preprocessor directives
CMAKE_POLICY(SET CMP0005 NEW)

# Include required cmake macros
INCLUDE(FeatureSummary)

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
FIND_PACKAGE(OGRE 1.6 REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(Boost 1.34 REQUIRED filesystem)
FIND_PACKAGE(SWIG REQUIRED)
IF (NOT SWIG_DIR MATCHES -rl$)
	SET_FEATURE_INFO(SWIG "" "" "You use an unsupported version of SWIG.")
ENDIF(NOT SWIG_DIR MATCHES -rl$)

# Path options
SET(BINDIR "bin" CACHE STRING "Where to install binaries")
SET(DATAROOTDIR "${CMAKE_INSTALL_PREFIX}/share" CACHE STRING "Sets the root of data directories to a non-default location")
SET(DATADIRNAME "rastullah" CACHE STRING "change the name of the directory for the read-only architecture-independent game data")

# Game options
SET(ENABLE_FMOD4 "Enable FMOD 4.xx (FMODex) sound support" ON)
SET(ENABLE_OPENAL "Enable OpenAL sound support (unsupported)" OFF)

# Handle options
SET(DATADIR ${DATAROOTDIR}/${DATADIRNAME})
ADD_DEFINITIONS(-DDATADIR=${DATADIR})

# Include subdirectories
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(plugins)

PRINT_ENABLED_FEATURES()