#include "MeshCombiner.h"

/*#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#else
	//#include "SDL.h"
#endif
*/


int main(int argc, char **argv)
{
	rl::MeshCombiner* meshCombiner = new rl::MeshCombiner();
	delete meshCombiner;
}

