#ifndef __DotSceneOctreeWorld_H__
#define __DotSceneOctreeWorld_H__

#include "World.h"

#include "CorePrerequisites.h"

namespace rl {

    class _RlCoreExport DotSceneOctreeWorld : public World
    {
    public:
        DotSceneOctreeWorld();
        ~DotSceneOctreeWorld();

        void clearScene();
        void loadScene( const String&  levelName );
        void initializeDefaultCamera();	

        virtual float* getVerticesPtr();
        virtual int* getIndexPtr();
        virtual int getVerticeCount();
        virtual int getIndexCount();
    };

}
#endif

