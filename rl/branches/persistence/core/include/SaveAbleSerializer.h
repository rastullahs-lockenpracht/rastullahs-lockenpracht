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

#ifndef __SaveAbleSerializer_H__
#define __SaveAbleSerializer_H__

#include "CorePrerequisites.h"
#include "SaveAbleManager.h"
#include "SaveGameFile.h"
#include "ScriptSerializer.h"

#include "SaveGameFileReader.h"
#include "SaveGameFileWriter.h"

namespace rl
{
	class _RlCoreExport SaveAbleSerializer : public Ogre::Singleton<SaveAbleSerializer>, public ScriptSerializer
	{
	public:
		SaveAbleSerializer();

		typedef std::map<int, SaveGameFile*> SaveGameEntryMap;

		SaveGameEntryMap listSaveGames();
		SaveGameEntryMap listSaveGames(const CeGuiString &moduleId);
		bool SaveGameExists(const CeGuiString &name, const CeGuiString &moduleId);
		SaveGameFile* getSaveGame(const CeGuiString &name, const CeGuiString &moduleId);

		virtual const Ogre::StringVector&  getScriptPatterns(void) const;
        virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
        virtual Ogre::Real getLoadingOrder(void) const;

		void createNewSaveGame();
		void writeSaveGame(int fileId);
		void readSaveGame(int fileId);

		int getHighestSaveGameNumber();
	protected:
		void writeToSaveGameFile(SaveGameFile* file);
		void readFromSaveGameFile(SaveGameFile* file);

		SaveGameEntryMap mSaveGameFiles;
		SaveGameFileReader mReader;
		SaveGameFileWriter mWriter;

		Ogre::StringVector mScriptPatterns;
		int mHighestSaveGameNumber;
	};
}

#endif