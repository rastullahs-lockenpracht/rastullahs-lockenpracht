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

#include "CorePrerequisites.h"
#include "SaveAble.h"

namespace rl
{

	class SaveAbleCollection
	{
	public:
		SaveAbleCollection(const CeGuiString &id);
		virtual void attachSaveAble(SaveAblePtr saveAble) = 0;
		virtual void deattachSaveAble(SaveAblePtr saveAble) = 0;
		virtual void deattachSaveAble(const CeGuiString &id) = 0;
		virtual void deattachAllSaveAbles() = 0;
		CeGuiString getId() const;
	protected:
		std::map<int, SaveAblePtr> mSaveAbles;
		CeGuiString mId;
	};

}