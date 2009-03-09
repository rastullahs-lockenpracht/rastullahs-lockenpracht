/*
	Demo08_RagdollExample

	by Walaber
*/

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "OgreNewtonApplication.h"


#ifdef WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char**argv)
#endif
{
    // Create application object
    OgreNewtonApplication app;

    try {
        app.go();
    } catch( Ogre::Exception& e ) {
		printf( "An Error HAS OCCURED: %s", e.getFullDescription().c_str() );
    }

    return 0;
}

