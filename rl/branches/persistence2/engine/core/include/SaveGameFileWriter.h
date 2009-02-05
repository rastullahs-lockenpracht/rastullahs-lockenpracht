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

#ifndef _SaveGameFileWriter_H_
#define _SaveGameFileWriter_H_

#include "CorePrerequisites.h"

#include <XmlPropertyWriter.h>

#include "SaveGameFile.h"

namespace rl
{
    //class SaveGameData;
    //typedef std::multimap<int,SaveGameData*> SaveGameDataOrderMap;

    class _RlCoreExport SaveGameFileWriter 
        : public XmlPropertyWriter
    {
    public:
		void buildSaveGameFile(SaveGameFile* file);
        //void buildSaveGameFile(SaveGameFile *file, const SaveGameDataOrderMap &map);
        //void writeEachProperty(SaveGameData* data, const rl::PropertyMap &map);
    };
}

#endif

