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
 
#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include "CommonPrerequisites.h"
#include <OgreConfigFile.h>
#include <OgreDataStream.h>

namespace Ogre 
{
    template<class T> class SharedPtr;
    class DataStream;
    typedef class SharedPtr<DataStream> DataStreamPtr;
    
}

namespace rl
{

class _RlCommonExport ConfigFile : public Ogre::ConfigFile
{
public:
	ConfigFile();
    /// save to a filename (not using resource group locations)
    void save(const Ogre::String& filename, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);
    /// save to a filename (using resource group locations)
    void save(const Ogre::String& filename, const Ogre::String& resourceGroup, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);
    /// save to a data stream
    void save(std::ofstream& stream, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);
    /// save to a filename (not using resource group locations)
    void saveDirect(const Ogre::String& filename, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);
    /// save to a filename (using resource group locations)
    void saveToResourceSystem(const Ogre::String& filename, const Ogre::String& resourceGroup, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

    /** Sets a setting to the file with the named key. 
    @param key The name of the setting
    @param section The name of the section it must be in (if any)
    */
    Ogre::String setSetting(const Ogre::String& key, const Ogre::String& section = Ogre::StringUtil::BLANK) const;
    
};

}

#endif /*CONFIGFILE_H_*/
