#ifndef __TerrainWorld_H__
#define __TerrainWorld_H__

#include "World.h"

#include "CorePrerequisites.h"

namespace rl {

class TerrainWorld : public World
{
public:
    TerrainWorld();
    ~TerrainWorld();

    void clearScene();
	void loadScene(const String& levelName);
	void initializeDefaultCamera();	

    Quaternion getWorldAxis();
};

}

#endif
