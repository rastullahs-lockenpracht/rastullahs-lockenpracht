#ifndef __RULESSUBSYSTEM_H__
#define __RULESSUBSYSTEM_H__

#include "RulesPrerequisites.h"
using namespace Ogre;

namespace rl
{
    class _RlRulesExport RulesSubsystem : public Singleton<RulesSubsystem>
    {
    public:
        RulesSubsystem();
        ~RulesSubsystem();

        static RulesSubsystem& getSingleton(void);
        static RulesSubsystem* getSingletonPtr(void);
    };
}

#endif
