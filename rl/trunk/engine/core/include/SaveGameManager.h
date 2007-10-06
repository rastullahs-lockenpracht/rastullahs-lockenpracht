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
#ifndef _SaveGameManager_H_
#define _SaveGameManager_H_

#include "CorePrerequisites.h"
#include "SaveGameFileWriter.h"
#include "SaveGameFileReader.h"
#include "SaveGameFile.h"

#include <XmlPropertyWriter.h>
#include <XmlPropertyReader.h>

namespace rl
{
    class _RlCoreExport SaveGameData
    {
    public:
        SaveGameData();
        virtual ~SaveGameData();
        virtual CeGuiString getXmlNodeIdentifier() const = 0;
        virtual void writeData(SaveGameFileWriter* writer) = 0;
        virtual void readData(SaveGameFileReader* reader) = 0;
    };

    typedef std::set<SaveGameData*> SaveGameDataSet;

    typedef std::map<CeGuiString, SaveGameFile*> SaveGameEntryMap;

    class _RlCoreExport SaveGameManager : public Ogre::Singleton<SaveGameManager>, public Ogre::ScriptLoader
    {
    public:
        SaveGameManager(void);
        virtual ~SaveGameManager(void);    

        SaveGameEntryMap listSaveGames(void);
        void saveSaveGameFile(const CeGuiString &name);
        void loadSaveGameFile(const CeGuiString &name);
        void deleteSaveGameFile(const CeGuiString &name);
        bool SaveGameFileExists(const CeGuiString &name);

        virtual const Ogre::StringVector&  getScriptPatterns(void) const;
        virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
        virtual Ogre::Real getLoadingOrder(void) const;

        void registerSaveGameData(SaveGameData* data);
        void unregisterSaveGameData(SaveGameData* data);
    protected:
        void freeSaveGameMap();

        Ogre::StringVector mScriptPatterns;
        SaveGameEntryMap mSaveGames;

        SaveGameDataSet mSaveGameDataSet;
    };
}

#endif
