#include "Logger.h"

#include <OgreLogManager.h>


using Ogre::LogManager;
using Ogre::Singleton;

template<> rl::Logger* Singleton<rl::Logger>::ms_Singleton = 0;

namespace rl
{
Logger& Logger::getSingleton(void)
{
    return Singleton<Logger>::getSingleton();
}

Logger* Logger::getSingletonPtr(void)
{
    return Singleton<Logger>::getSingletonPtr();
}

Logger::Logger(const Ogre::String& logPath, const Ogre::String& ogreLogPath)
{
    if (LogManager::getSingletonPtr() == 0)
    {
	   new LogManager();
    }

	//Log für Ogre
	LogManager::getSingleton().setDefaultLog(LogManager::getSingleton().createLog(ogreLogPath));

	//Log für RL
	mLog = LogManager::getSingleton().createLog(logPath);
}

Logger::~Logger()
{
}

void Logger::log(const Ogre::LogMessageLevel level, const Ogre::String& component, 
			const Ogre::String& message, const Ogre::String& ident)
{
	if (ident.length() == 0)
		log(level, "[" + component + "] " + message);
	else
		log(level, "[" + component + "] (" + ident + ") " + message);
}

void Logger::log(const Ogre::LogMessageLevel level, const Ogre::String& msg )
{
    mLog->logMessage(msg, level);
}

void Logger::setLogDetail(const Ogre::LoggingLevel level)
{
	mLogLevel = level;
	mLog->setLogDetail(level);
}

const CEGUI::LoggingLevel Logger::getCeGuiLogDetail()
{
	if (mLogLevel == Ogre::LL_LOW)
		return CEGUI::Errors;
	else if (mLogLevel == Ogre::LL_NORMAL)
		return CEGUI::Standard;
	else if (mLogLevel == Ogre::LL_BOREME)
		return CEGUI::Insane;

	return CEGUI::Errors;
}



}
