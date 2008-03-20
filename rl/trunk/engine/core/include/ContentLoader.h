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

#ifndef __ContentLoader_H__
#define __ContentLoader_H__

#include "CorePrerequisites.h"
#include "Properties.h"

namespace rl
{
    class ContentModule;

    class _RlCoreExport ContentLoader : public PropertyHolder
    {
    public:
        ContentLoader(const Ogre::String& resourceGroup);
        virtual ~ContentLoader();

        virtual void loadContent() = 0;
        virtual void unloadContent() = 0;

        const Property getProperty(const CeGuiString& key) const;
        void setProperty(const CeGuiString& key, const Property& value);

        PropertyKeys getAllPropertyKeys() const;

        virtual const CeGuiString getClassName() const = 0;
    protected:
        ContentModule* mContentModule;
    };
}

#endif
