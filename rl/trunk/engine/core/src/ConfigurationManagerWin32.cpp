/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#include "ConfigurationManagerWin32.h"
using namespace std;

rl::ConfigurationManager* rl::ConfigurationManagerWin32::sInstance = 0;


namespace rl
{
	ConfigurationManager* ConfigurationManagerWin32::getSingletonPtr()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerWin32;
		}
		return sInstance;
	}
	
	ConfigurationManager& ConfigurationManagerWin32::getSingleton()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerWin32;
		}
		return *sInstance;
	}
	
	ConfigurationManagerWin32::ConfigurationManagerWin32()
	{
		mModulesRootDirectory = ".";
		mConfigModuleDirectory = mModulesRootDirectory + "/modules/config";
		mPluginCfgPath = mConfigModuleDirectory + "/plugins-win.cfg";
		mRastullahCfgPath = mConfigModuleDirectory + "/rastullah_ogre.cfg";
        mRastullahSystemCfgPath = mConfigModuleDirectory + "/rastullah_system.cfg";
		mSoundCfgPath = mConfigModuleDirectory + "/sound.cfg";
		mOgreLogPath = "logs/ogre.log";
		mRastullahLogPath = "logs/rastullah.log";
		mModulesCfgPath = mModulesRootDirectory + "/modules/modules.cfg";
	}
	
	ConfigurationManagerWin32::~ConfigurationManagerWin32()
	{
	    sInstance = NULL;
	}
	
}
