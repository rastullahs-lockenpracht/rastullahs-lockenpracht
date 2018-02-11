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


/// Basis jedes Objektes im Spiel, welches veränderlich ist und einen abspeicherbaren Status haben soll

#ifndef __SaveAble_H__
#define __SaveAble_H__

#include "CorePrerequisites.h"
#include "Properties.h"

namespace rl
{
	class SaveAble;

	class SaveAblePtr : public Ogre::SharedPtr<SaveAble>
	{
	};

	class SaveAble : public PropertyHolder
	{
	public:
		SaveAble(int id, bool isSaveAble = true);
		void setSaveAble(bool enable);
		bool isSaveAble();

		virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;
		
		SaveAblePtr getParent() const { return mParentSaveAble; };

		int getId();
	protected:
		SaveAblePtr mParentSaveAble;
		int mId;
		bool mIsSaveAble;
	};	

	typedef std::pair<int, SaveAblePtr>  SaveAbleReference;
}


#endif

