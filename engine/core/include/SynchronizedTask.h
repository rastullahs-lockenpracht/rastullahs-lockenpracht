#ifndef __SynchronizedTask_H__
#define __SynchronizedTask_H__

#include "CorePrerequisites.h"

namespace rl {

class _RlCoreExport SynchronizedTask
{
    public:
        SynchronizedTask();
        virtual ~SynchronizedTask( ) {};

        virtual void run( Real elapsedTime ) = 0;
        
        bool isPaused();
        void setPaused( bool isPaused );

    private:
        bool mPaused;
};

}
#endif
