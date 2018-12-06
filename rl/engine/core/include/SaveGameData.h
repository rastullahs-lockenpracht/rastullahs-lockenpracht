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
#ifndef __SaveGameData_H__
#define __SaveGameData_H__

#include "CorePrerequisites.h"

namespace rl
{
    class SaveGameFileWriter;
    class SaveGameFileReader;

    class _RlCoreExport SaveGameData
    {
    public:
        SaveGameData();
        virtual ~SaveGameData();
        virtual CeGuiString getXmlNodeIdentifier() const = 0;
        virtual void writeData(SaveGameFileWriter* writer) = 0;
        virtual void readData(SaveGameFileReader* reader) = 0;
        /// defines the loading/saving order higher priority are saved last and loaded first
        virtual int getPriority() const = 0;
    };
}

#endif // __SaveGameData_H__
