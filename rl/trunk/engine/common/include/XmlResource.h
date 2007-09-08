/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#pragma warning (push)
#pragma warning (disable : 4244)
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#pragma warning (pop)

#include "CommonPrerequisites.h"
#include <OgreResource.h>
#include "XmlHelper.h"

namespace rl {

    class _RlCommonExport XmlResource : 
        public Ogre::Resource
    {
    public:
        XmlResource(
            Ogre::ResourceManager* creator, 
            const Ogre::String& name, 
            Ogre::ResourceHandle handle, 
            const Ogre::String& group,
            bool isManual = false,
            Ogre::ManualResourceLoader* loader = NULL);

        virtual ~XmlResource();

        bool parseBy(XERCES_CPP_NAMESPACE::XercesDOMParser* parser, XmlProcessor* const proc = NULL);
        bool parseBy(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, XmlProcessor* const proc = NULL);

    protected:
        size_t calculateSize() const;

        void loadImpl();
        void unloadImpl();

    private:
        XERCES_CPP_NAMESPACE::MemBufInputSource* mXmlBuffer;
        XMLByte* mCharBuffer;
    };

    class _RlCommonExport XmlPtr :
        public Ogre::SharedPtr<XmlResource>
    {
    public:
        XmlPtr() : Ogre::SharedPtr<XmlResource>() {}
        explicit XmlPtr(XmlResource* rep) : Ogre::SharedPtr<XmlResource>(rep) {}
        XmlPtr(const XmlPtr& res) : Ogre::SharedPtr<XmlResource>(res) {}
        XmlPtr(const Ogre::ResourcePtr& res);
        XmlPtr& operator=(const Ogre::ResourcePtr& res);
    protected:
        void destroy();
    };

}

#endif
