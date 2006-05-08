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
 
#include "ConfigFile.h"
#include <OgreString.h>
#include <OgreDataStream.h>

using namespace Ogre;
using namespace std;

namespace rl
{

ConfigFile::ConfigFile()
    : Ogre::ConfigFile()
{
}

//-----------------------------------------------------------------------
void ConfigFile::save(const String& filename, const String& separators, bool trimWhitespace)
{
    saveDirect(filename, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::save(const String& filename, const String& resourceGroup, 
    const String& separators, bool trimWhitespace)
{
    saveToResourceSystem(filename, resourceGroup, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::saveDirect(const String& filename, const String& separators, 
    bool trimWhitespace)
{
    /* Open the configuration file */
    ofstream fp;
    fp.open(filename.c_str());
    if(!fp)
        OGRE_EXCEPT(
        Exception::ERR_FILE_NOT_FOUND, "'" + filename + "' file not found!", "rl::ConfigFile::save" );

    save(fp, separators, trimWhitespace);

}
//-----------------------------------------------------------------------
void ConfigFile::saveToResourceSystem(const String& filename, 
    const String& resourceGroup, const String& separators, bool trimWhitespace)
{
// TODO ResourceGroup zum Dateiname erstellen.
    ofstream fp;
    fp.open(filename.c_str());
    if(!fp)
        OGRE_EXCEPT(
        Exception::ERR_FILE_NOT_FOUND, "'" + filename + "' file not found!", "rl::ConfigFile::save" );


    save(fp, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::save(ofstream& stream, const String& separators, 
    bool trimWhitespace)
{
	SettingsBySection::const_iterator section;
	for(section = mSettings.begin(); section != mSettings.end(); section++)
	{
		if (section->first != StringUtil::BLANK)
		{
			stream << "[" << section->first << "]" << endl;
		} else {
			/// Keine Sektion
		}
		SettingsMultiMap::const_iterator setting;
		SettingsMultiMap *settings = section->second;
		for(setting = settings->begin(); setting != settings->end(); setting++)
		{
			stream << setting->first << "=" << setting->second << endl;
		}
	}
}

//-----------------------------------------------------------------------
void ConfigFile::setSetting(const String& key, const String &value, const String& section)
{
	if (mSettings[section] == 0)
	{
		// Neue Sektion anlegen
		mSettings[section] = new SettingsMultiMap();
	}
	mSettings[section]->insert( multimap<String, String>::value_type(key, value));
}


}
