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

#ifndef __RlScene_H__
#define __RlScene_H__

#include "ScriptPrerequisites.h"

#include <vector>

#include "Properties.h"
#include "SaveAbleCollection.h"

namespace rl {

    class _RlScriptExport Scene : public PropertyHolder, public SaveAbleCollection
    {
    public:
        static const Ogre::String PROPERTY_GAMEOBJECTS;

        Scene(const CeGuiString& name);
        ~Scene();

        void addMap(const Ogre::String& mapfile);
        void load(bool loadGameObjects);
        const CeGuiString& getName() const;

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;

		virtual void attachSaveAble(SaveAblePtr saveAble);
		virtual void deattachSaveAble(SaveAblePtr saveAble);
		virtual void deattachSaveAble(const CeGuiString &id);
		virtual void deattachAllSaveAbles();
    private:
        CeGuiString mName;
        std::vector<Ogre::String> mMaps;
    };

}

#endif // __RlScene_H__
