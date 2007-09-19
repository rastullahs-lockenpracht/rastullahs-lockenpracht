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
#include "stdinc.h" //precompiled header

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/TransService.hpp>
#include "AimlNodeImplRl.h"

#include "XmlProcessor.h"

namespace rl {

AimlNodeImplRl::AimlNodeImplRl(DOMNode* pNode)
{
    initializeXml();
	mNodeName  = transcodeToString(pNode->getLocalName());
	// #text or #comment have no local name, use NodeName for them
	if(mNodeName.empty())
	{
		mNodeName  = transcodeToString(pNode->getNodeName());
	}
	mNodeName.c_str();

	// remove whitespaces from textnodes
	if(mNodeName == "#text")
	{
		XMLCh* tmp = XMLString::replicate(pNode->getNodeValue());
		XMLString::collapseWS(tmp);
		mNodeValue = transcodeToString(tmp);
		XMLString::release(&tmp);
	}
	else
	{
		mNodeValue = transcodeToString(pNode->getNodeValue());
	}
	mNodeValue.c_str();
	// mpa attributes
	DOMNamedNodeMap* attributes = pNode->getAttributes();
	if(attributes != NULL)
	{
		for(unsigned int i = 0; i < attributes->getLength(); ++i)
		{
			CeGuiString attributeName  =
				transcodeToString(attributes->item(i)->getNodeName());
			CeGuiString attributeValue =
				transcodeToString(attributes->item(i)->getNodeValue());
			mAttributes.insert(Attributes::value_type(attributeName, attributeValue));
		}
	}
	//debug
	for(Attributes::iterator itr = mAttributes.begin(); itr != mAttributes.end() ;++itr)
	{
		itr->first.c_str();
		itr->second.c_str();
	}
	// map children
	DOMNode* child = pNode->getFirstChild();
	for(; child !=NULL; child = child->getNextSibling())
	{
		// don't add pure whitespaces as AimlNode
		XMLCh* cmp = XMLString::transcode("#text");
		if(XMLString::equals(child->getNodeName(), cmp))
		{
			if(XMLChar1_0::isAllSpaces(child->getNodeValue(), XMLString::stringLen(child->getNodeValue())))
			{
				continue;
			}
		}
		XMLString::release(&cmp);
		mChildren.push_back(new AimlNodeImplRl(child));
	}
	if(!mChildren.empty())
	{
		std::vector<XmlNode<CeGuiString>*>::iterator itr = mChildren.begin();
		std::vector<XmlNode<CeGuiString>*>::iterator itrnext;
		for(; itr != mChildren.end()-1; ++itr)
		{
			itrnext= itr+1;
			(*itr)->setNextSibling((*itrnext));
		}
		(*itr)->setNextSibling(NULL);
	}
}

AimlNodeImplRl::~AimlNodeImplRl(void)
{
    shutdownXml();
}

}
