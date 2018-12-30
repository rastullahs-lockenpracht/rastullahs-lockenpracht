INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${TINYXML2_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS})

SET(RlCommon_LIB_SRCS
src/ConfigFile.cpp
src/EventObject.cpp
src/EventSource.cpp
src/GameTask.cpp
src/Logger.cpp
src/MathUtil.cpp
src/Properties.cpp
src/Property.cpp
src/ScriptWrapper.cpp
src/WriteableDataStream.cpp
src/XmlProcessor.cpp
src/XmlPropertyReader.cpp
src/XmlPropertyWriter.cpp
src/XmlResource.cpp
src/XmlResourceManager.cpp)

ADD_LIBRARY(RlCommon SHARED ${RlCommon_LIB_SRCS})

TARGET_LINK_LIBRARIES(RlCommon ${TINYXML2_LIBRARY} ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} Boost::boost Boost::filesystem)

SET_TARGET_PROPERTIES(RlCommon PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLCOMMON_EXPORTS)

INSTALL(TARGETS RlCommon LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
