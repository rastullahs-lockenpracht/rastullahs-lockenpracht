/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __XmlResource_H__
#define __XmlResource_H__

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>

#include "CommonPrerequisites.h"
#include <OgreResource.h>
#include <OgreSharedPtr.h>

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

		void parseBy(XERCES_CPP_NAMESPACE::XercesDOMParser* parser);
		void parseBy(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser);
	
	protected:
		size_t calculateSize() const;

		void loadImpl();
		void unloadImpl();

	private:
		XERCES_CPP_NAMESPACE::MemBufInputSource* mXmlBuffer;
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
