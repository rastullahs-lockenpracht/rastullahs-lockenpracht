#include "SynchronizedTask.h"

namespace rl {

SynchronizedTask::SynchronizedTask()
{
    mPaused = false;
}
        
bool SynchronizedTask::isPaused()
{
    return mPaused;
}

void SynchronizedTask::setPaused( bool isPaused )
{
    mPaused = isPaused;
}

}
