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

#include "RulesPrerequisites.h"
#include "SaveGameFileWriter.h"
#include "SaveGameFile.h"

#include <XmlPropertyWriter.h>
#include <XmlPropertyReader.h>

namespace rl
{
    typedef std::map<CeGuiString, SaveGameFile*> SaveGameEntryMap;

    class SaveGameHeaderReader : public XmlPropertyReader
    {
    public:
        SaveGameHeaderReader();
        void parseHeader(Ogre::DataStreamPtr &stream, const Ogre::String &groupName, SaveGameFile* file);
    };

    class _RlRulesExport SaveGameManager : public Ogre::Singleton<SaveGameManager>, public Ogre::ScriptLoader
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
    protected:
        void freeSaveGameMap();

        Ogre::StringVector mScriptPatterns;
        SaveGameEntryMap mSaveGames;
    };
}

#endif
