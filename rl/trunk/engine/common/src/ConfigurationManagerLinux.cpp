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

#include "ConfigurationManagerLinux.h"
#include <iostream>
#include <fstream>

rl::ConfigurationManager* rl::ConfigurationManagerLinux::instance = 0;

using namespace std;

namespace rl
{
	ConfigurationManager* ConfigurationManagerLinux::getSingletonPtr()
	{
		if(instance == 0)
		{
			instance = new ConfigurationManagerLinux;
		}
		return instance;
	}
	
	ConfigurationManager& ConfigurationManagerLinux::getSingleton()
	{
		if(instance == 0)
		{
			instance = new ConfigurationManagerLinux;
		}
		return *instance;
	}
	
	ConfigurationManagerLinux::ConfigurationManagerLinux()
	{
		string confdir("modules/common/conf/");
		pluginCfgPath = findConfRootDir() + "/" + confdir + 
			"plugins-linux.cfg";
		rastullahCfgPath = findRastullahConf();
		ogreLogPath = "logs/ogre.log";
		rlCoreLogPath = "logs/rlCore.log";
		modulesCfgPath = "./modules/modules.cfg";
		moduleconfigCfgPath = "/conf/moduleconfig.cfg";
	}
	
	ConfigurationManagerLinux::~ConfigurationManagerLinux()
	{
	}
	
	string ConfigurationManagerLinux::findConfRootDir()
    {
    	static string CURRENT_DIR = ".";
        char line[255];
        ifstream config;
        // Erstmal im Homeverzeichnis suchen.
        string path = ::getenv("HOME");
        path += "/.rl.conf";
        config.open(path.c_str());
        if (!config.is_open())
        {
            // Wir schliessen die Datei sicherheitshalber und
            // setzen den Zustand zurück (wichtig!)
            config.close();
            config.clear();
            // Jetzt schauen wir mal, ob in etc was zu finden ist.
            config.open("/etc/rl/rl.conf");
        } 
        // Haben  wir jetzt eine Datei?
        if (config.is_open())
        {
            while (!config.eof())
            {
                config.getline(line, 255);
                if (strlen(line) != 0)
                {
                    // Wir geben die erste nichtleere Zeile zurück.
                    return string(line);
                }
            }
        }
        // Klappt alles nicht.
        cerr<<"line "<<endl;
        return CURRENT_DIR;
    }

    string ConfigurationManagerLinux::findRastullahConf()
    {
        char line[255];
        ifstream config;
        // Erstmal im Homeverzeichnis suchen.
        string home = ::getenv("HOME");
        string path = home + "/.rastullah.cfg";
        config.open(path.c_str());
        if (!config.is_open())
        {
            // Wir schliessen die Datei sicherheitshalber und
            // setzen den Zustand zurück (wichtig!)
            config.close();
            config.clear();
            // Jetzt schauen wir mal, ob in etc was zu finden ist.
            path = "/etc/rl/rastullah.cfg";
            config.open(path.c_str());
        }
        // Haben  wir jetzt eine Datei?
        if (config.is_open())
        {
            config.close();
            return path;
        }
        return home + "/.rastullah.cfg";
    }
}
