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

#ifndef SYSTEM_PROCESSOR_H
#define SYSTEM_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"
#include "../AimlParser.h"

#include <string>
using namespace std;

namespace rl
{
	class SystemProcessor : public AimlProcessor
	{
	public:
		SystemProcessor(){};
		~SystemProcessor() { }
	
		string process(DOMNode* node,Match* m, const char *str, NaturalLanguageProcessor* nlp)
		{
				DOMNamedNodeMap* attrbs=node->getAttributes();
				DOMNode* commandNode=attrbs->getNamedItem(XMLString::transcode("command"));	
				string commandStr=AimlParser::transcodeXmlCharToString(commandNode->getNodeValue());
				if(!commandStr.compare("exit"))
					nlp->mExit=true;
				return "";
		};
	};
}
#endif