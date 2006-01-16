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

#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include "XmlResourceManager.h"
#include "OgreResourceGroupManager.h"

#include "AimlDocumentImplXerces.h"
#include "AimlNodeImplXerces.h"

#include "DialogSubsystem.h"

namespace rl
{
	XERCES_CPP_NAMESPACE_USE
	using namespace Ogre;

	AimlDocumentImplXerces::AimlDocumentImplXerces(const std::string& document, bool fromMemory)
		: mDocument(NULL), mRootNode(NULL)
	{
		if(fromMemory)
		{
			parseXmlFromFile(document);
		}
		else
		{
			parseXmlFromMemory(document);
		}
	}

	AimlDocumentImplXerces::~AimlDocumentImplXerces(void)
	{
		mDocument->release();
		mDocument = NULL;
	}

	
	void AimlDocumentImplXerces::parseXmlFromFile(const std::string& fileName)
	{	
		XercesDOMParser* parser = new XercesDOMParser();
		try
		{	
            XmlPtr res = DialogSubsystem::getSingleton().getXmlResource(fileName);
			res->parseBy(parser);
			mDocument = parser->adoptDocument();
			delete parser;
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
		{
			// get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			// cleanup
			if(parser)
			{
				delete parser; 
			}
		//	if(xmlHandler)delete xmlHandler;  TODO: xmlHandler!!!
			throw (exc);
		}
		mRootNode = new AimlNodeImplXerces(NULL, mDocument->getDocumentElement());
	}

	void AimlDocumentImplXerces::parseXmlFromMemory(const std::string& xmlContent)
	{

	}

	AimlNode* AimlDocumentImplXerces::getDocumentElement()
	{
		return mRootNode;
	}
}
