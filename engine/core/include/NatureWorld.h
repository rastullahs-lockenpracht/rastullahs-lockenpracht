#ifndef __NatureWorld_H__
#define __NatureWorld_H__

#include "World.h"

#include "CorePrerequisites.h"

namespace rl {

class NatureWorld : public World
{
public:
    NatureWorld();
    ~NatureWorld();

    void clearScene();
	void loadScene(const String& levelName);
	void initializeDefaultCamera();	

    Quaternion getWorldAxis();
};

}

#endif
