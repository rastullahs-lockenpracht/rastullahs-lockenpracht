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

#ifndef __XmlResource_H__
#define __XmlResource_H__

#include "CommonPrerequisites.h"
#include "XmlProcessor.h"

namespace rl
{
    class _RlCommonExport XmlResource : public Ogre::Resource
    {
    public:
        XmlResource(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
            const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = NULL);

        virtual ~XmlResource();

        const char* getContent();

    protected:
        size_t calculateSize() const;

        void loadImpl();
        void unloadImpl();

    private:
        char* mCharBuffer;
    };

    using XmlPtr = std::shared_ptr<XmlResource>;
}

#endif
