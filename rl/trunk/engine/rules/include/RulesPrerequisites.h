#ifndef __RulesPrerequisites_H__
#define __RulesPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLRULES_EXPORTS )
#       define _RlRulesExport __declspec( dllexport )
#   else
#       define _RlRulesExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlRulesExport
#endif

#include <map>
#include <vector>

#define _t(T) std::string(T)

#include <OgreLogManager.h>

#define log_error(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_CRITICAL);
#define log_info(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_NORMAL);
#define log_debug(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_TRIVIAL);

namespace rl
{
    class _RlRulesExport Action;
    class _RlRulesExport GameObject;

    typedef std::map<std::string, Action*> ActionMap;
    typedef std::vector<std::string> StringVector;
}
#endif
