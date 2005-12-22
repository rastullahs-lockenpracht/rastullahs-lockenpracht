/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef SRAI_PROCESSOR_H
#define SRAI_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"
#include "../AimlParser.h"
#include "../DialogSubsystem.h"
#include "../Nodemaster.h"

#include <string>
using namespace std;

namespace rl
{
	class SraiProcessor : public AimlProcessor
	{
	public:
		SraiProcessor(){};
		~SraiProcessor() { }
	
		CeGuiString process(DOMNode* node,Match* m, const CeGuiString& str, NaturalLanguageProcessor* nlp)
		{
			DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "Srai");
			Match* newMatch = nlp->match( "*", XmlHelper::transcodeToString(
												node->getFirstChild()->
												getNodeValue()),
										  "*", "*");
			if(newMatch)
			{
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "found new match");
				//  get the <template> tag as DOMDocument node
				DOMDocument* doc=(DOMDocument *)newMatch->getNode()->getTemplateNode();
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "getTemplateNode");
				//  get the content of DOMDocument
				DOMNode* newNode=doc->getDocumentElement();
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "getDocumentElement");
				
				CeGuiString result= nlp->process(newNode,newMatch,str);
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "Processed");
				doc->release();
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "doc release");
				delete newMatch;
				DialogSubsystem::getSingletonPtr()->log(Ogre::LML_TRIVIAL, "match delete");
				return result;
			}
			return "";
		}
	};
}
#endif
