#ifndef __GameLoop_H__
#define __GameLoop_H__

#include <list>
#include <OgreSingleton.h>

#include "CorePrerequisites.h"

namespace rl {

class SynchronizedTask;

class _RlCoreExport GameLoop : public FrameListener, protected Singleton<GameLoop>
{

public:
	GameLoop();
	virtual ~GameLoop();

    void addSynchronizedTask(SynchronizedTask* newTask);
    void removeSynchronizedTask(SynchronizedTask* oldTask);

	void loop( Real timeSinceLastCall );
    
    bool frameStarted(const Ogre::FrameEvent & evt);
	bool frameEnded(const Ogre::FrameEvent & evt);

    bool isPaused();
    void setPaused(bool pause);

    static GameLoop & getSingleton(void);
	static GameLoop * getSingletonPtr(void);

private:
    bool mPaused;

    std::list<SynchronizedTask*> mSynchronizedTaskList;
};


}
#endif

