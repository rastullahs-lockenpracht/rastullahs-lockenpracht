cmake_minimum_required (VERSION 3.6)
project(mesh_combiner)

find_package(OGRE 1.10 REQUIRED)

add_executable(mesh_combiner MergeMesh.cpp MergeSkeleton.cpp MeshCombiner.cpp MeshCombinerMain.cpp)

target_include_directories(mesh_combiner
    PRIVATE ../include
    PUBLIC ${OGRE_INCLUDE_DIRS})
target_link_libraries(mesh_combiner OgreMain)
