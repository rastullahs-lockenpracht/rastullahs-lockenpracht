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

#include "stdinc.h"

#include "SaveAbleManager.h"

template<> rl::SaveAbleManager* Ogre::Singleton<rl::SaveAbleManager>::ms_Singleton = 0;

namespace rl
{
	void SaveAbleManager::saveState()
	{
	}

	void SaveAbleManager::addSaveAble(SaveAblePtr save, const CeGuiString &collectionId)
	{
	}

	void SaveAbleManager::removeSaveAble(SaveAblePtr save)
	{
	}
		
	void SaveAbleManager::removeSaveAble(const CeGuiString &id)
	{
	}
		
	void SaveAbleManager::removeAllSaveAbles()
	{
	}
		
	/*void SaveAbleManager::attachSaveAbleToCollection(const CeGuiString &saveAbleId, const CeGuiString &collectionId)
	{
	}*/
		
	/*void SaveAbleManager::deattachSaveAbleFromColltection(const CeGuiString &saveAbleId, const CeGuiString &collectionId)
	{
	}*/
		
	void SaveAbleManager::restoreSaveAblesFromStates()
	{
	}

	void SaveAbleManager::restoreSaveAbleFromState(const CeGuiString &id)
	{
	}

	void SaveAbleManager::storeSaveAbleStates()
	{
		mSaveAbleStates.clear();

		SaveAbleMap::const_iterator it;
		for(it = mSaveAbles.begin(); it != mSaveAbles.end(); it++)
		{
			PropertyRecordPtr Properties;
			Properties->setProperty("Properties", it->second->getAllProperties()->toPropertyMap());

			// Ein dickes TODO ------
			Properties->setProperty("CreationProperties", Property());
			Properties->setProperty("Factory", Property("ID der Factory"));
			mSaveAbleStates[it->first] = Properties;
		}
	}
}
