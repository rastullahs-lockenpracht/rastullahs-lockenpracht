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
#include "processors/SraiProcessor.h"
#include "AimlParser.h"
#include "DialogSubsystem.h"
#include "Nodemaster.h"
#include "XmlHelper.h"

namespace rl
{
	SraiProcessor::SraiProcessor(void)
	{
	}

	SraiProcessor::~SraiProcessor(void)
	{
	}

	CeGuiString SraiProcessor::process(DOMNode* node,Match* match, const CeGuiString& str, NaturalLanguageProcessor* nlp)
	{
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Srai");
		Match* newMatch = nlp->match( "*", XmlHelper::transcodeToString(
											node->getFirstChild()->
											getNodeValue()),
									  "*", "*");
		if(newMatch)
		{
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "found new match");
			//  get the <template> tag as DOMDocument node
			DOMDocument* doc=(DOMDocument *)newMatch->getNode()->getTemplateNode();
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "getTemplateNode");
			//  get the content of DOMDocument
			DOMNode* newNode=doc->getDocumentElement();
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "getDocumentElement");
			
			CeGuiString result= nlp->process(newNode,newMatch,str);
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Processed");
			doc->release();
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "doc release");
			delete newMatch;
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "match delete");
			return result;
		}
		return "";
	}
}
