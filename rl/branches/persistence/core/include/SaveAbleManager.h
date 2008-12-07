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

#ifndef __SaveAbleManager_H__
#define __SaveAbleManager_H__

#include "CorePrerequisites.h"
#include "SaveAble.h"
#include "SaveAbleCollection.h"
#include "SaveAbleFactory.h"

namespace rl
{
	class _RlCoreExport SaveAbleManager : public Ogre::Singleton<SaveAbleManager>
	{
	protected:
		typedef std::map<CeGuiString, SaveAbleCollection*> SaveAbleCollectionMap;
	public:
		typedef std::map<CeGuiString, PropertyRecordPtr> SaveAbleStateMap;
		void saveState();
		void loadState();
		void addSaveAble(SaveAblePtr save, const CeGuiString &collectionId);
		void moveSaveAbleToCollection(SaveAblePtr save, const CeGuiString &collectionId);
		void moveSaveAbleToCollection(const CeGuiString &saveAbleId, const CeGuiString &collectionId);
		void removeSaveAble(SaveAblePtr save);
		void removeSaveAble(const CeGuiString &id);
		void removeAllSaveAbles();
		
		SaveAbleCollectionMap getCollections();
		SaveAbleStateMap getSaveAbleStates();

		SaveAbleFactory* getFactory(SaveAblePtr saveAble);
		SaveAbleFactory* getFactory(const CeGuiString &id);
	protected:
		SaveAbleStateMap mSaveAbleStates;
		//SaveAbleMap mSaveAbles;
		SaveAbleCollectionMap mSaveAbleCollections;

		void storeSaveAbleStates();
		void restoreSaveAblesFromStates();
		void restoreSaveAbleFromState(const CeGuiString &id);		
	};
}

#endif