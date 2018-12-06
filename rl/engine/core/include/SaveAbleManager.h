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

namespace rl
{
	class SaveAbleManager : public Ogre::Singleton<SaveAbleManager>
	{
	public:
		void saveState();
		void addSaveAble(SaveAblePtr save);
		void removeSaveAble(SaveAblePtr save);
		void removeSaveAble(int id);
		void removeAllSaveAbles();
		void attachSaveAbleToCollection(int id);
		void deattachSaveAbleFromColltection(int id);
		void restoreState();
	protected:
		std::map<int, PropertyRecordPtr> mSaveAbleStates;
		std::map<int, SaveAblePtr> mSaveAbles;
		std::map<Ogre::String,SaveAbleCollection> mSaveAbleCollections;
	};
}

#endif

