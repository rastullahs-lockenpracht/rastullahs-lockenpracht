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

#include "AimlParser.h"
#include "processors/ConditionProcessor.h"
#include "Utils.h"
#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"
#include <string>

#include "DialogSubsystem.h"

using namespace std;

namespace rl
{
	string ConditionProcessor::process(DOMNode* node,Match* m, const char* str, NaturalLanguageProcessor* nlp)
	{		
		string buffer="";
		DOMNamedNodeMap* attrbs=node->getAttributes();
	//	DOMNode* scriptNode=attrbs->getNamedItem(XMLString::transcode("script"));
	//	DOMNode* classNode=attrbs->getNamedItem(XMLString::transcode("class"));
		DOMNode* nameNode=attrbs->getNamedItem(XMLString::transcode("name"));

		if(nameNode==NULL)	// If function name is missing, return
		{
			return "";
		}
		// Look if condition has a value (standard: no value)
		DOMNode* valueNode=attrbs->getNamedItem(XMLString::transcode("value"));
	//	if(scriptNode!=NULL && classNode!=NULL)
	//	{
	//		string scriptStr=AimlParser::transcodeXmlCharToString(scriptNode->getNodeValue());
	//		string classStr =AimlParser::transcodeXmlCharToString(classNode->getNodeValue());
			string nameStr=AimlParser::transcodeXmlCharToString(nameNode->getNodeValue());
			ScriptObject* so=CoreSubsystem::getSingleton().getInterpreter()->getScriptObject("DialogMeister"); //new ScriptObject("conditionTest");

			//		Ogre::String bla[]={"Popel"};
	//		so->setScript(scriptStr,classStr,1,bla);
	//		so->callFunction("OnPlaySound",0,0);
			int iReturn=so->callIntegerFunction(nameStr,0,0);	
	//		scriptStr+=classStr;
	//		if(iReturn==4){scriptStr="Hatgefunzt";}

			for (DOMNode* childNode=node->getFirstChild(); childNode != node->getLastChild(); childNode = childNode->getNextSibling() )
			{	// Search through all li-elements
				if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) 
				{
					string nodeName=AimlParser::transcodeXmlCharToString(childNode->getNodeName());
					if(!nodeName.compare("li"))
					{
						DOMNamedNodeMap* attrbs=childNode->getAttributes();
						int value=AimlParser::getAttributeValueAsInteger(attrbs,"value");
						string idString= AimlParser::transcodeXmlCharToString(attrbs->getNamedItem(XMLString::transcode("id"))->getNodeValue());
						if(value==iReturn)
						{
							buffer+="<li id=\""+idString+"\" />";
							buffer+=nlp->process(childNode,m,str);
						}
					}
				}
			}	
	//	}
		return buffer;
	}
}// Namespace rl end
