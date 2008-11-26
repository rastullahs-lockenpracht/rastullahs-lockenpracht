INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${FMODEX_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${RL_UI_INCLUDE_DIR})

SET(RlFmod4Driver_LIB_SRCS
src/Fmod4ConfigComponent.cpp
src/Fmod4Driver.cpp
src/Fmod4DriverDllMain.cpp
src/Fmod4Listener.cpp
src/Fmod4Sound.cpp)

ADD_LIBRARY(RlFmod4Driver SHARED ${RlFmod4Driver_LIB_SRCS})