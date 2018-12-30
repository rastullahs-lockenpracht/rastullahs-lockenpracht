INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/nulldriver
${RL_COMMON_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIR}
${OGRE_INCLUDE_DIRS}
${CEGUI_INCLUDE_DIRS}
${OGRENEWT_INCLUDE_DIR}
${MESHMAGICK_INCLUDE_DIR}
${RUBY_INCLUDE_PATH})

SET(RlCore_LIB_SRCS
src/Actor.cpp
src/ActorControlledObject.cpp
src/ActorManager.cpp
src/AnimationManager.cpp
src/BaseAnimation.cpp
src/BoxPrimitive.cpp
src/CameraObject.cpp
src/ConfigurationManager.cpp
src/ContentLoader.cpp
src/ContentModule.cpp
src/CoreSubsystem.cpp
src/DebugVisualisable.cpp
src/DebugVisualsManager.cpp
src/FadeAnimation.cpp
src/GameAreaEvent.cpp
src/GameAreaEventSource.cpp
src/GameAreaListener.cpp
src/GameAreaTypes.cpp
src/GameEventManager.cpp
src/GameLoop.cpp
src/GenericWorld.cpp
src/Job.cpp
src/JobQueue.cpp
src/JobScheduler.cpp
src/LightObject.cpp
src/LightFadeJob.cpp
src/LineSetPrimitive.cpp
src/ListenerObject.cpp
src/ListenerMovable.cpp
src/LinearSoundFadeFunctor.cpp
src/MergeableMeshObject.cpp
src/MeshAnimation.cpp
src/MeshObject.cpp
src/MessagePump.cpp
src/MovableText.cpp
src/ParticleSystemObject.cpp
src/PhysicsContactListener.cpp
src/PhysicsCollisionFactory.cpp
src/PhysicalThing.cpp
src/PhysicsGenericContactCallback.cpp
src/PhysicsManager.cpp
src/PhysicsMaterialRaycast.cpp
src/PhysicsRagDoll.cpp
src/PlayAnimationJob.cpp
src/PlaySoundJob.cpp
src/PolynomicSoundFadeFunctor.cpp
src/PrimitiveObject.cpp
src/RubyInterpreter.cpp
src/SaveAble.cpp
src/SaveAbleCollection.cpp
src/SaveAbleFactory.cpp
src/SaveAbleManager.cpp
src/SaveGameFile.cpp
src/SaveGameFileReader.cpp
src/SaveGameFileWriter.cpp
src/SaveGameManager.cpp
src/SceneQuery.cpp
src/SoundManager.cpp
src/SoundObject.cpp
src/SoundDriver.cpp
src/Sound.cpp
src/SoundEvents.cpp
src/SoundResource.cpp
src/SoundFadeJob.cpp
src/SoundFadeFunctor.cpp
src/SoundStitching.cpp
src/SoundStitchingObject.cpp
src/StartAnimationJob.cpp
src/TimeSource.cpp
src/Trigger.cpp
src/TrackAnimation.cpp
src/World.cpp
src/Zone.cpp
src/ZoneManager.cpp
src/nulldriver/NullDriver.cpp
src/nulldriver/NullListener.cpp
src/nulldriver/NullSound.cpp
src/nulldriver/NullSoundStitching.cpp)

ADD_DEFINITIONS(-DRL_MODULEDIR="${DATAROOTDIR}/${DATADIRNAME}")

ADD_LIBRARY(RlCore SHARED ${RlCore_LIB_SRCS})

find_package(OGRE COMPONENTS OgreOverlay)

TARGET_LINK_LIBRARIES(RlCore ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARY} ${MESHMAGICK_LIBRARY} ${OIS_LIBRARIES} ${RUBY_LIBRARY} Boost::boost OgreOverlay)

SET_TARGET_PROPERTIES(RlCore PROPERTIES
	VERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}.${RL_PATCH_VERSION}
	SOVERSION ${RL_MAJOR_VERSION}.${RL_MINOR_VERSION}
	DEFINE_SYMBOL RLCORE_EXPORTS)

INSTALL(TARGETS RlCore LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
