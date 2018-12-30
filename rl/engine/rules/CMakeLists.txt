INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${RL_COMMON_INCLUDE_DIR}
${RL_CORE_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIR})

SET(RlRules_LIB_SRCS
src/Action.cpp
src/ActionManager.cpp
src/ApplyDamageJob.cpp
src/Armor.cpp
src/Combat.cpp
src/Combatant.cpp
src/CombatManager.cpp
src/Container.cpp
src/Creature.cpp
src/CreatureController.cpp
src/CreatureControllerManager.cpp
src/Date.cpp
src/DsaDataLoader.cpp
src/DsaManager.cpp
src/Effect.cpp
src/EffectFactory.cpp
src/EffectManagementTask.cpp
src/EffectManager.cpp
src/Eigenschaft.cpp
src/EigenschaftenStateSet.cpp
src/GameEventLog.cpp
src/GameObject.cpp
src/GameObjectManager.cpp
src/GameTimeSource.cpp
src/GlobalProperties.cpp
src/GoToJob.cpp
src/FetchItemJob.cpp
src/Item.cpp
src/Inventory.cpp
src/JournalEntry.cpp
src/Kampfaktion.cpp
src/Kampftechnik.cpp
src/ObjectStateChangeEvent.cpp
src/ObjectStateChangeEventSource.cpp
src/ObjectStateChangeListener.cpp
src/PartyManager.cpp
src/Quest.cpp
src/QuestBook.cpp
src/QuestEvent.cpp
src/QuestListener.cpp
src/RulesSubsystem.cpp
src/SelectionHelper.cpp
src/Selector.cpp
src/Slot.cpp
src/SonderfertigkeitenStateSet.cpp
src/StateSet.cpp
src/Talent.cpp
src/Talentgruppe.cpp
src/TalentStateSet.cpp
src/WaitJob.cpp
src/Weapon.cpp
src/ZauberStateSet.cpp
src/FallDownMovement.cpp
src/JogFastMovement.cpp
src/JogMovement.cpp
src/JumpHighMovement.cpp
src/JumpLongMovement.cpp
src/JumpToTargetMovement.cpp
src/LieDownMovement.cpp
src/RotateMovement.cpp
src/SneakMovement.cpp
src/SprintMovement.cpp
src/StandAroundMovement.cpp
src/StepRecognitionMovement.cpp
src/StrafeMovement.cpp
src/WalkMovement.cpp)

ADD_LIBRARY(RlRules SHARED ${RlRules_LIB_SRCS})

TARGET_LINK_LIBRARIES(RlRules ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARY} Boost::boost)

SET_TARGET_PROPERTIES(RlRules PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLRULES_EXPORTS)

INSTALL(TARGETS RlRules LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
