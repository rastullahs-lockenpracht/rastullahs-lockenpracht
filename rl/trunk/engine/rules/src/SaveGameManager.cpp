/* 
 * (C) 2003-2007. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */

#include "stdinc.h"

#include "SaveGameManager.h"

#include <CoreSubsystem.h>
#include <ContentModule.h>

template<> rl::SaveGameManager* Ogre::Singleton<rl::SaveGameManager>::ms_Singleton = 0;

namespace rl
{
    SaveGameIndexWriter::SaveGameIndexWriter()
    {
    }

    void SaveGameIndexWriter::buildIndexFile(const SaveGameEntryMap &map)
    {
    }

    SaveGameIndexReader::SaveGameIndexReader()
    {
    }

    SaveGameEntryMap &SaveGameIndexReader::parseIndexFile() const
    {
        SaveGameEntryMap entries;
        return entries;
    }

    SaveGameManager::SaveGameManager()
    {
    }

    SaveGameEntryMap SaveGameManager::listSaveGames()
    {
        return mReader->parseIndexFile();
    }

    void SaveGameManager::saveSaveGameFile(const CeGuiString &name)
    {
        time_t rawTime;
        tm* localTime; 
        time(&rawTime);
        localTime = localtime(&rawTime); 
        SaveGameFile file(name, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId(), localTime);

        SaveGameFileWriter writer;
        writer.buildSaveGameFile(&file);
    }

    void SaveGameManager::loadSaveGameFile(const CeGuiString &name)
    {
    }
}
