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

#include "ConfigurationManagerMac.h"

rl::ConfigurationManager* rl::ConfigurationManagerMac::instance = 0;

using namespace std;

namespace rl
{
	ConfigurationManager* ConfigurationManagerMac::getSingletonPtr()
	{
		if(instance == 0)
		{
			instance = new ConfigurationManagerMac;
		}
		return instance;
	}
	
	ConfigurationManager& ConfigurationManagerMac::getSingleton()
	{
		if(instance == 0)
		{
			instance = new ConfigurationManagerMac;
		}
		return *instance;
	}
	
	ConfigurationManagerMac::ConfigurationManagerMac()
	{
		string confdir("modules/common/conf/");
		pluginCfgPath = confdir + "plugins-mac.cfg";
		rastullahCfgPath = confdir + "rastullah.cfg";
		ogreLogPath = "logs/ogre.log";
		rlCoreLogPath = "logs/rlCore.log";
		modulesCfgPath = "./modules/modules.cfg";
		moduleconfigCfgPath = "/conf/moduleconfig.cfg";
		modulesRootDirectory = ".";
	}
	
	ConfigurationManagerMac::~ConfigurationManagerMac()
	{
	}
}
