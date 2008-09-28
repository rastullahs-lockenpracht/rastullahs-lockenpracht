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

#ifndef __GLOBALPROPERTIES_H__
#define __GLOBALPROPERTIES_H__

#include "RulesPrerequisites.h"

#include "Properties.h"
#include "SaveGameData.h"

namespace rl
{
    class _RlRulesExport GlobalProperties 
        : public PropertyHolder, 
        public Ogre::Singleton<GlobalProperties>, 
        public SaveGameData
    {
    public:
        /** Default Constructor */
        GlobalProperties();
        /** Default Deconstructor */
        virtual ~GlobalProperties();

        /// Overrides from PropertyHolder
        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;
        bool hasProperty(const Ogre::String& key) const;

        /// Overrides from SaveGameData
        /// Manages saving and loading from the SaveGameFile
        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual void writeData(SaveGameFileWriter* writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual int getPriority() const;
    protected:
        PropertyRecord mProperties;
    };
}

#endif

