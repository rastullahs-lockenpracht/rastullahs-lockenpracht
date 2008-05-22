/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>

#include "Nodemaster.h"
#include "DialogSubsystem.h"
#include "Utils.h"
//--	Nodemaster.cpp



XERCES_CPP_NAMESPACE_USE

namespace rl
{
	Nodemaster* Nodemaster::cached;
	Nodemaster::Nodemaster() { }
	Nodemaster::Nodemaster(const CeGuiString& templateValue) 
	//	: mTemplate("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"+templateValue)
	    : mTemplate("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"+templateValue)
	{
	}

	Nodemaster::~Nodemaster() {}

	void Nodemaster::deleteAllNodes()
	{
		// DON'T LOG AFTER DESTRUCTING DIALOGSUBSYSTEM
//		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Delete all children");
		std::map<CeGuiString, Nodemaster *>::const_iterator itr = mChildren.begin();
		for(;itr!=mChildren.end();itr++)
		{
			if(itr->second)
			{
				itr->second->deleteAllNodes();
				delete itr->second; //child;
			}
		}
		mChildren.clear();
	}

	DOMNode* Nodemaster::getTemplateNode() const 
	{
		XercesDOMParser* parser=new XercesDOMParser();
		MemBufInputSource mTest(
		    reinterpret_cast<const XMLByte*>(mTemplate.data()),
		    static_cast<const unsigned int>(mTemplate.size()),
		    "test", false);
		parser->parse(mTest);
		DOMDocument* doc=parser->adoptDocument();
		if ( doc == NULL ) 
			return NULL;
		delete parser;
		return doc;
	}

	CeGuiString Nodemaster::getTemplate()
	{
		return mTemplate;
	}

	void Nodemaster::setTemplate(const CeGuiString &data) 
	{
		mTemplate=data;
	}

	void Nodemaster::addChild(const CeGuiString &key, Nodemaster *child) 
	{
		cached = child;
		mChildren[toUpper(key)] = cached;
	}

	Nodemaster* Nodemaster::getChild(const CeGuiString &key) 
	{
		if ( key.empty() ) return cached;
		std::map<CeGuiString, Nodemaster *>::iterator itr = mChildren.find(toUpper(key));
		if ( itr == mChildren.end() ) return NULL;
		cached = itr->second;
		return cached;
	}
}
