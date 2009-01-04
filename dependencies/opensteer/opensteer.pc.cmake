prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}
libdir=${CMAKE_INSTALL_PREFIX}/lib
includedir=${CMAKE_INSTALL_PREFIX}/include/OpenSteer

Name: OpenSteer
Description: C++ steering behavior library
Requires: OGRE >= 1.4
Version: ${OpenSteer_MAJOR_VERSION}.${OpenSteer_MINOR_VERSION}.${OpenSteer_PATCH_VERSION}
Libs: -L${CMAKE_INSTALL_PREFIX}/lib -lOpenSteer
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/OpenSteer
