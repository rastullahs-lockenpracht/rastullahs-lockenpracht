prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}
libdir=${CMAKE_INSTALL_PREFIX}/lib
includedir=${CMAKE_INSTALL_PREFIX}/include/OgreNewt

Name: OgreNewt
Description: Ogre Bindings for the Newton Physics SDK
Requires: OGRE >= 1.4
Version: ${OgreNewt_MAJOR_VERSION}.${OgreNewt_MINOR_VERSION}.${OgreNewt_PATCH_VERSION}
Libs: -L${CMAKE_INSTALL_PREFIX}/lib -lOgreNewt
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/OgreNewt
