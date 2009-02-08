/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __SaveGameFile_H__
#define __SaveGameFile_H__



#include "CorePrerequisites.h"
#include <xercesc/framework/XMLFormatter.hpp>
#include <ctime>

#include "Property.h"
#include "Properties.h"
#include "WriteableDataStream.h"
#include "ScriptSerializer.h"


namespace rl
{
    class _RlCoreExport SaveGameFile : public PropertyHolder
    {
    public:
        static const Ogre::String PROPERTY_MODULEID;
        static const Ogre::String PROPERTY_TIME;
        static const Ogre::String PROPERTY_NAME;
        static const Ogre::String PROPERTY_MODULENAME;

        SaveGameFile(int id, Ogre::DataStreamPtr stream);
        ~SaveGameFile();
        CeGuiString getName() const;
		CeGuiString getModuleId() const;
		int getId() const;

        //access to file
        Ogre::DataStreamPtr getDataStream() const;
        XERCES_CPP_NAMESPACE::XMLFormatTarget* getFormatTarget();
        void deleteFileFromStorage(); //erwuenscht?

        //propertyholder
        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;
    protected:
        //properties
        CeGuiString mName;
        CeGuiString mModuleID;
        CeGuiString mModuleName;
        CeGuiString mLocalTime;

        //file access
        Ogre::StringVector mScriptPatterns;
        Ogre::DataStreamPtr mStream;
        int mSaveGameId;
    };
}

#endif
