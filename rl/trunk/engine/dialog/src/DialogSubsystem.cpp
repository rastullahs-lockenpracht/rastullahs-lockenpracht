#include "AimlProcessorManager.h"
#include "DialogSubsystem.h"

#include "OgreLogManager.h"

template<> rl::DialogSubsystem* Singleton<rl::DialogSubsystem>::ms_Singleton = 0;

namespace rl 
{
	DialogSubsystem& DialogSubsystem::getSingleton(void)
	{
		return Singleton<DialogSubsystem>::getSingleton();
	}

	DialogSubsystem* DialogSubsystem::getSingletonPtr(void)
	{
		return Singleton<DialogSubsystem>::getSingletonPtr();
	}

	DialogSubsystem::DialogSubsystem()
	{
		Log* log = LogManager::getSingleton().createLog( "logs/rlDialog.log" );
        log->setLogDetail( LL_BOREME );
		log->logMessage("Test");
		AimlProcessorManager::addStandardProcessors();
//		Log* log = LogManager::getSingleton().createLog( "rlDialog.log" );
//      log->setLogDetail( LL_BOREME );
//		log->logMessage("Test");
	}

    DialogSubsystem::~DialogSubsystem() 
    {  

    }
}
