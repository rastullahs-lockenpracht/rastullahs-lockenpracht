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
    String currentSection = StringUtil::BLANK;
    SettingsMultiMap* currentSettings = new SettingsMultiMap();
    mSettings[currentSection] = currentSettings;


}

//-----------------------------------------------------------------------
String ConfigFile::setSetting(const String& key, const String& section) const
{
    
    SettingsBySection::const_iterator seci = mSettings.find(section);
    if (seci == mSettings.end())
    {
        return StringUtil::BLANK;
    }
    else
    {
        SettingsMultiMap::const_iterator i = seci->second->find(key);
        if (i == seci->second->end())
        {
            return StringUtil::BLANK;
        }
        else
        {
            return i->second;
        }
    }
}


}
