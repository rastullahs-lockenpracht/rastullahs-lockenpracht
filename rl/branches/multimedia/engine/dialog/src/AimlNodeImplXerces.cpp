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
//#include <xercesc/sax2/SAX2XMLReader.hpp>
//#include <xercesc/sax2/XMLReaderFactory.hpp>
//#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include "AimlNodeImplXerces.h"



namespace rl
{
	AimlNodeImplXerces::AimlNodeImplXerces(AimlNode* parent, DOMNode* node)
		: AimlNode(parent), mNode(node)
	{
		mNode = node;
		for(node = node->getFirstChild(); node != NULL; node->getNextSibling())
		{
			mChildNodes.push_back(new AimlNodeImplXerces(this, node));
		}
	}

	AimlNodeImplXerces::~AimlNodeImplXerces(void)
	{
/*		std::vector<AimlNode*>::iterator iter = mChilNodes.begin();
		for(; iter != mChildNodes.end(); ++iter)
		{
			delete (*iter);
		}
		mChildNodes.clear();
		*/
	}

	AimlNode* AimlNodeImplXerces::getFirstChild()
	{
		return mChildNodes[0];
	}

	AimlNode* AimlNodeImplXerces::getNextSibling()
	{
//		return (new AimlNodeImplXerces(mNode->getNextSibling()));
		return NULL;
	}

}
