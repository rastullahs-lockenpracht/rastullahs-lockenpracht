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
#include "MessagePump.h"

#include <XmlPropertyWriter.h>
#include <XmlPropertyReader.h>

//#include <multimap>

namespace rl
{
    enum SaveGameMessageTypeIds
    {
        //Mask
        RLMSG_SAVEGAME_EVENTS     = 0x0300000,
        //Events
        RLMSG_SAVEGAME_LOADED     = 0x0300001,
        RLMSG_SAVEGAME_SAVED      = 0x0300002,
        RLMSG_SAVEGAME_GOS_LOADED = 0x3000003
    };

    typedef MessageType<RLMSG_SAVEGAME_LOADED> MessageType_SaveGameLoaded;

    typedef MessageType<RLMSG_SAVEGAME_SAVED> MessageType_SaveGameSaved;

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

    typedef std::multimap<int,SaveGameData*> SaveGameDataOrderMap;

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

        SaveGameDataOrderMap mSaveGameDataOrderMap;
    };
}

#endif
