INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${RL_RULES_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIRS}
${OPENSTEER_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIR})

SET(RlAi_LIB_SRCS
src/Agent.cpp
src/AgentCombatState.cpp
src/AgentDialogState.cpp
src/AgentManager.cpp
src/AgentSteeringState.cpp
src/AiSubsystem.cpp
src/AiWorld.cpp
src/AStarCosts.cpp
src/AStar.cpp
src/AStarHeuristic.cpp
src/AStarNodePool.cpp
src/AStarWayPointNode.cpp
src/CreatureWalkPathJob.cpp
src/Dialog.cpp
src/DialogCondition.cpp
src/DialogElement.cpp
src/DialogImplication.cpp
src/DialogLoader.cpp
src/DialogLoaderImpl.cpp
src/DialogManager.cpp
src/DialogOption.cpp
src/DialogParagraph.cpp
src/DialogResponse.cpp
src/DialogVariable.cpp
src/FuzzyState.cpp
src/FuzzyStateMachine.cpp
src/Landmark.cpp
src/LandmarkPath.cpp
src/PhysicalObstacle.cpp
src/SimpleVehicle.cpp
src/SteeringVehicle.cpp
src/WalkPathBehaviour.cpp
src/WayPointNode.cpp
src/WayPointGraph.cpp
src/WayPointGraphManager.cpp
src/OpenSteer/Color.cpp
src/OpenSteer/lq.c
src/OpenSteer/Obstacle.cpp
src/OpenSteer/Path.cpp
src/OpenSteer/Pathway.cpp
src/OpenSteer/PolylineSegmentedPath.cpp
src/OpenSteer/PolylineSegmentedPathwaySingleRadius.cpp
src/OpenSteer/SegmentedPath.cpp
src/OpenSteer/SegmentedPathway.cpp
src/OpenSteer/Vec3.cpp)

ADD_LIBRARY(RlAi SHARED ${RlAi_LIB_SRCS})

TARGET_LINK_LIBRARIES(RlAi ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARIES} ${OPENSTEER_LIBRARY} Boost::boost)

SET_TARGET_PROPERTIES(RlAi PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLAI_EXPORTS)

INSTALL(TARGETS RlAi LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
