/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include <OgreDataStream.h>

#include <Property.h>
#include <Properties.h>

#include <xercesc/framework/XMLFormatter.hpp>

#include <ctime>

namespace rl
{
    class _RlCoreExport SaveGameFile : public PropertyHolder, public Ogre::ScriptLoader
    {
    public:
        SaveGameFile(const CeGuiString &name);
        ~SaveGameFile();
        CeGuiString buildFilename();
        CeGuiString getName();
        bool saveGameExists();

        Ogre::DataStreamPtr &getDataStream();
        void closeDataStream();
        XERCES_CPP_NAMESPACE::XMLFormatTarget* getFormatTarget();
        void deleteFileFromStorage();

        virtual const Ogre::StringVector&  getScriptPatterns(void) const;
        virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
        virtual Ogre::Real getLoadingOrder(void) const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertyRecord* getAllProperties() const;
    protected:
        CeGuiString mName;
        CeGuiString mModuleID;
        CeGuiString mLocalTime;

        Ogre::StringVector mScriptPatterns;
        Ogre::DataStreamPtr mStream;
    };
}

#endif
