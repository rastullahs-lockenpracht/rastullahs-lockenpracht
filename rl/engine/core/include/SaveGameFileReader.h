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

#ifndef _SaveGameFileReader_H_
#define _SaveGameFileReader_H_

#include <tinyxml2.h>

#include "SaveGameFile.h"
#include "XmlPropertyReader.h"

namespace rl
{
    class SaveGameData;
    typedef std::multimap<int, SaveGameData*> SaveGameDataOrderMap;

    class _RlCoreExport SaveGameFileReader : public XmlPropertyReader
    {
    protected:
        tinyxml2::XMLDocument* mDocument;

    public:
        SaveGameFileReader();
        void parseSaveGameFile(SaveGameFile* file, const SaveGameDataOrderMap& map);
        void parseSaveGameFileHeader(Ogre::DataStreamPtr& stream, const Ogre::String& groupName, SaveGameFile* file);

        PropertyRecordPtr getAllPropertiesAsRecord(SaveGameData* data);

        tinyxml2::XMLDocument* getDocument()
        {
            return mDocument;
        }
    };
}

#endif //#ifndef _SaveGameFileReader_H_
