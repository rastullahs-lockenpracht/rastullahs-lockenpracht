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

#include "AimlParser.h"
#include "processors/ConditionProcessor.h"
#include "Utils.h"
#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"
#include "XmlHelper.h"
#include <string>

#include "DialogSubsystem.h"

using namespace std;

namespace rl
{
	string ConditionProcessor::process(DOMNode* node,Match* m, const char* str, NaturalLanguageProcessor* nlp)
	{		
		string buffer="";
	//  Get name- and value-attribute. Name contains the function name, 
	//  value is optional (if there are no values in <li> 
		char* strName = XmlHelper::getAttributeValueAsString( (DOMElement*)node,XMLString::transcode("name") );
		int nValue = 0;//XmlHelper::getAttributeValueAsInteger( (DOMElement*)node,XMLString::transcode("value") );
	//  If function name is missing, return
		if(!((string)strName).empty())	
		{
		//  Get the NPC dialog control script object and call the named function
			ScriptObject* so=CoreSubsystem::getSingleton().getInterpreter()->getScriptObject("DialogMeister");  
			// @todo: ScriptObject name has to be NPC name
			int nReturn=so->callIntegerFunction(strName,0,0);
			XMLString::release(&strName);
		//  Search through all li-elements
			for (DOMNode* childNode=node->getFirstChild(); childNode != node->getLastChild(); childNode = childNode->getNextSibling() )
			{	
				if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) 
				{
					string nodeName=AimlParser::transcodeXmlCharToString(childNode->getNodeName());
					if(!nodeName.compare("li"))
					{
						nValue=XmlHelper::getAttributeValueAsInteger( (DOMElement*)childNode,XMLString::transcode("value") );
						char* test=XmlHelper::getAttributeValueAsString( (DOMElement*)childNode,XMLString::transcode("value") );
						char* id =XmlHelper::getAttributeValueAsString( (DOMElement*)childNode,XMLString::transcode("id") );
					//  Add elements in <li> Tag only if <li>-value = return value of the named function
						if(nValue==nReturn)
						{
							buffer+="<li id=\""+(string)id+"\" />";
							buffer+=nlp->process(childNode,m,str);	
						}
						XMLString::release(&id);
					} // end compare nodeName
				} // end compare nodeType
			} // end for loop
		} // end check empty name
		return buffer;
	}
}// Namespace rl end
