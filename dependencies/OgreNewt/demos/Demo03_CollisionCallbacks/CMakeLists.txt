ADD_EXECUTABLE(Demo03_CollisionCallbacks demo03.cpp OgreNewtonApplication.cpp OgreNewtonFrameListener.cpp ConveyorBelt.cpp ConveyorMatCallback.cpp)

TARGET_LINK_LIBRARIES(Demo03_CollisionCallbacks ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARIES} ${OIS_LIBRARIES} ${CEGUIOGRE_LIBRARIES})

#INSTALL(TARGETS Demo03_CollisionCallbacks RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
