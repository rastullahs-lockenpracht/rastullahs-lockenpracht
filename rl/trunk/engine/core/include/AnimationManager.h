#ifndef __AnimationManager_H__
#define __AnimationManager_H__

#include <list>
#include <OgreSingleton.h>

#include "SynchronizedTask.h"

#include "CorePrerequisites.h"

namespace rl {

class _RlCoreExport AnimationManager : public SynchronizedTask, protected Singleton<AnimationManager>
{
public:
    /** Default Constructor */
	AnimationManager( );
	/** Default Deconstructor */
    virtual ~AnimationManager();

    void addAnimation(AnimationState* newAnimState);
    void removeAnimation(AnimationState* oldAnimState);

    virtual void run(Real timePassed);

    static AnimationManager & getSingleton(void);
	static AnimationManager * getSingletonPtr(void);
private:
    std::list<AnimationState*> mAnimationList;
};
}
#endif
