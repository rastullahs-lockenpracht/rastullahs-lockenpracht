#ifndef __BSPWorld_H__
#define __BSPWorld_H__

#include "World.h"

#include "CorePrerequisites.h"


namespace rl {

class _RlCoreExport BSPWorld : public World
{
    private:
		static void removeAllLightmaps();
    public:
        BSPWorld();
        ~BSPWorld();
 
        void clearScene();
		void loadScene( const String&  levelName );
		void initializeDefaultCamera();	

        Quaternion getWorldAxis();
};

}
#endif

