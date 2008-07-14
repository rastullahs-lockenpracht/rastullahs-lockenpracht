#ifndef __EffectManagementTask__
#define __EffectManagementTask__

#include "GameTask.h"

namespace rl
{
    class EffectManagementTask :
        public GameTask
    {
    public:
        EffectManagementTask();
        ~EffectManagementTask();

        virtual void run(Ogre::Real elapsedTime);
        virtual const Ogre::String& getName() const;
        virtual void setTimeFactor(Ogre::Real timeFactor);

    private:
        Ogre::Real mElapsedTime;
        Ogre::Real mTimeFactor;
    };

}

#endif
