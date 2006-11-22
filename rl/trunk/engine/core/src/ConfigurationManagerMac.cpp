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

rl::ConfigurationManager* rl::ConfigurationManagerMac::sInstance = NULL;

using namespace std;

namespace rl
{
	ConfigurationManager* ConfigurationManagerMac::getSingletonPtr()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerMac;
		}
		return sInstance;
	}
	
	ConfigurationManager& ConfigurationManagerMac::getSingleton()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerMac;
		}
		return *sInstance;
	}
	
	ConfigurationManagerMac::ConfigurationManagerMac()
	{
		mModulesRootDirectory = ".";

        mConfigModuleDirectory = mModulesRootDirectory+"/modules/config";
       
		mPluginCfgPath = mConfigModuleDirectory + "/plugins-mac.cfg";
		mRastullahCfgPath = mConfigModuleDirectory + "/rastullah_ogre.cfg";
		mRastullahSystemCfgPath = mConfigModuleDirectory + "/rastullah_system.cfg";
		mSoundCfgPath = mConfigModuleDirectory + "sound.cfg";

		mOgreLogPath = mModulesRootDirectory + "/logs/ogre.log";
		mRastullahLogPath = mModulesRootDirectory + "/logs/rastullah.log";
		mModulesCfgPath = "./modules/modules.cfg";

	}
	
	ConfigurationManagerMac::~ConfigurationManagerMac()
	{
	}
}
