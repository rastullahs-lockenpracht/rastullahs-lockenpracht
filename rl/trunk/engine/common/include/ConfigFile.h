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
#include <OgreStringConverter.h>

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


    /**
	 * Setzt die Einstellung mit Namen und Abschnitt
     * @param name Der Name der Einstellung
	 * @param value Der Wert der Einstellung
     * @param section Der Name des Abschnitts
	 * @author JoSch
	 * @date 05-10-2006
    */
	template <class T>
	void setValue(const T &value, const Ogre::String &name,
		const Ogre::String &section = Ogre::StringUtil::BLANK)
	{
		if (mSettings[section] == 0)
		{
			// Neue Sektion anlegen
			mSettings[section] = new SettingsMultiMap();
		}
		Ogre::String _value = Ogre::StringConverter::toString(value);
		mSettings[section]->insert( std::multimap<Ogre::String, Ogre::String>::value_type(name, _value));
	}

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param _default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	int getValue(const int _default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	Ogre::String ConfigFile::getValue(const Ogre::String &_default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	Ogre::Real ConfigFile::getValue(const Ogre::Real &_default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	bool ConfigFile::getValue(const bool _default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	long int ConfigFile::getValue(const long int _default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	unsigned int ConfigFile::getValue(const unsigned int _default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

	/**
	 * Holt eine Einstellung mit Namen und Abschnitt
	 * @param name Der Name der Einstellung
	 * @param section Der Name des Abschnitts
	 * @param __default Ein Standardwert, falls die Einstellung nicht
	 * gespeichert wurde
	 * @author JoSch
	 * @date 05-10-2006
	 */
	unsigned long int ConfigFile::getValue(const unsigned long int _default, const Ogre::String &name, 
		const Ogre::String &section = Ogre::StringUtil::BLANK);

};

template<> inline
void ConfigFile::setValue<Ogre::String>(const Ogre::String &value, const Ogre::String &name,
        const Ogre::String &section)
{
        if (mSettings[section] == 0)
    {
        // Neue Sektion anlegen
        mSettings[section] = new SettingsMultiMap();
    }
    mSettings[section]->insert( std::multimap<Ogre::String, Ogre::String>::value_type(name, value));   
}

}

#endif /*CONFIGFILE_H_*/
