INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${RL_RULES_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIRS})

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
src/WayPointGraphManager.cpp)

ADD_LIBRARY(RlAi SHARED ${RlAi_LIB_SRCS})

TARGET_LINK_LIBRARIES(RlAi ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARIES})

SET_TARGET_PROPERTIES(RlAi PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}
	DEFINE_SYMBOL RLAI_EXPORTS)