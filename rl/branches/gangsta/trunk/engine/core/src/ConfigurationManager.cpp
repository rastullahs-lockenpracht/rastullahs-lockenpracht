/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
 
#include "ConfigurationManager.h"
#include "ConfigurationManagerLinux.h"
#include "ConfigurationManagerWin32.h"
#include "ConfigurationManagerMac.h"

namespace rl
{
	ConfigurationManager* ConfigurationManager::getSingletonPtr()
	{
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		    return ConfigurationManagerLinux::getSingletonPtr();
#       elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    return ConfigurationManagerWin32::getSingletonPtr();
#       else
		    return ConfigurationManagerMac::getSingletonPtr();
#       endif
	}
	
	ConfigurationManager& ConfigurationManager::getSingleton()
	{
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		    return ConfigurationManagerLinux::getSingleton();
#       elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    return ConfigurationManagerWin32::getSingleton();
#       else
		    return ConfigurationManagerMac::getSingleton();
#       endif
	}
	
	ConfigurationManager::~ConfigurationManager()
	{
	}
	
	Ogre::String ConfigurationManager::getOgreLogPath()
	{
		return ogreLogPath;
	}
	
	Ogre::String ConfigurationManager::getRastullahCfgPath()
	{
		return rastullahCfgPath;
	}
	
	Ogre::String ConfigurationManager::getPluginCfgPath()
	{
		return pluginCfgPath;
	}
	
	Ogre::String ConfigurationManager::getRlCoreLogPath()
	{
		return rlCoreLogPath;
	}
	
	Ogre::String ConfigurationManager::getModulesCfgPath()
	{
		return modulesCfgPath;
	}
	
	Ogre::String ConfigurationManager::getModuleconfigCfgPath(const Ogre::String& module)
	{
		return (modulesRootDirectory + "/modules/" + module + moduleconfigCfgPath);
	}
	
	Ogre::String ConfigurationManager::getModulesRootDirectory()
	{
		return modulesRootDirectory;
	}
}
