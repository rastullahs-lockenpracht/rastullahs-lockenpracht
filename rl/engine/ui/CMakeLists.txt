INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${RL_RULES_INCLUDE_DIR}
${RL_AI_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${OIS_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${CEGUIOGRE_INCLUDE_DIR}
${OGRENEWT_INCLUDE_DIR}
${RUBY_INCLUDE_PATH})

SET(RlUi_LIB_SRCS
src/AboutWindow.cpp
src/ActionChoiceWindow.cpp
src/AbstractWindow.cpp
src/CharacterSelectionWindow.cpp
src/CharacterSheetWindow.cpp
src/CharacterStateWindow.cpp
src/CloseConfirmationWindow.cpp
src/CombatControlState.cpp
src/CombatGui.cpp
src/CombatWindow.cpp
src/CommandMapper.cpp
src/ConfigComponent.cpp
src/ContainerContentWindow.cpp
src/ControlState.cpp
src/Console.cpp
src/CutsceneControlState.cpp
src/DebugWindow.cpp
src/DialogController.cpp
src/DialogControlState.cpp
src/DialogWindow.cpp
src/FreeflightControlState.cpp
src/GameLoggerWindow.cpp
src/GameObjectInfoWindow.cpp
src/GameOverWindow.cpp
src/GameSaveLoadWindow.cpp
src/GameSettings.cpp
src/InfoPopup.cpp
src/InGameMenuWindow.cpp
src/InputManager.cpp
src/InventoryWindow.cpp
src/ItemDragContainer.cpp
src/ItemDescriptionDragContainer.cpp
src/ItemIconDragContainer.cpp
src/JournalWindow.cpp
src/LogWindow.cpp
src/MainMenuWindow.cpp
src/MainMenuEngineWindow.cpp
src/MainMenuLoadWindow.cpp
src/MessageWindow.cpp
src/MovementControlState.cpp
src/ObjectDescriptionWindow.cpp
src/PlaylistWindow.cpp
src/ProgressWindow.cpp
src/PropertiesWindow.cpp
src/RenderSystemConfigComponent.cpp
src/SoundConfig.cpp
src/SoundDriverConfigComponent.cpp
src/SubtitleWindow.cpp
src/UiSubsystem.cpp
src/WindowFactory.cpp
src/WindowFadeJob.cpp
src/WindowManager.cpp)

ADD_LIBRARY(RlUi SHARED ${RlUi_LIB_SRCS})

TARGET_LINK_LIBRARIES(RlUi ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUIOGRE_LIBRARY} ${OGRENEWT_LIBRARY} ${RUBY_LIBRARY} Boost::boost)

SET_TARGET_PROPERTIES(RlUi PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLUI_EXPORTS)

INSTALL(TARGETS RlUi LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
