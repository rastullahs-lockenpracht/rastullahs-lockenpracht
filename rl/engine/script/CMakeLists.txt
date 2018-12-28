# Include required cmake macros
INCLUDE(UseSWIG)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${RL_RULES_INCLUDE_DIR}
${RL_AI_INCLUDE_DIR}
${RL_UI_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIR}
${OIS_INCLUDE_DIRS}
${RUBY_INCLUDE_PATH})

SET_SOURCE_FILES_PROPERTIES(swig/RlExports.i PROPERTIES 
	CPLUSPLUS ON
	SWIG_FLAGS -minherit)

SWIG_ADD_MODULE(RlScript RUBY
swig/RlExports.i
src/AbstractMapNodeProcessor.cpp
src/EntityNodeProcessor.cpp
src/EnvironmentProcessor.cpp
src/GameObjectNodeProcessor.cpp
src/LightNodeProcessor.cpp
src/MapLoader.cpp
src/ParticleSystemNodeProcessor.cpp
src/PlaneNodeProcessor.cpp
src/Scene.cpp
src/SceneLoader.cpp
src/SceneManager.cpp
src/ScriptSubsystem.cpp
src/ScriptObjectMarker.cpp
src/SoundNodeProcessor.cpp
src/TriggerFactory.cpp
src/WaypointProcessor.cpp
src/ZoneProcessor.cpp)

SWIG_LINK_LIBRARIES(RlScript ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARY} ${OIS_LIBRARIES} ${RUBY_LIBRARY} Boost::boost)

SET_TARGET_PROPERTIES(RlScript PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLSCRIPT_EXPORTS)

IF(NOT WIN32)
	SET_TARGET_PROPERTIES(RlScript PROPERTIES
		LINK_FLAGS "-Wl,--defsym -Wl,Init_libRlScript=Init_RlScript")
ENDIF(NOT WIN32)

INSTALL(TARGETS RlScript LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
