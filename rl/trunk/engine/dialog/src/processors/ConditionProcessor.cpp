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
		string strName = XmlHelper::getAttributeValueAsString( 
			static_cast<DOMElement*>(node), "name" ).c_str();
		int nValue = 0;//XmlHelper::getAttributeValueAsInteger( (DOMElement*)node,XMLString::transcode("value") );
	//  If function name is missing, return
		if(!strName.empty())	
		{
		//  Get the NPC dialog control script object and call the named function
			ScriptObject* so=CoreSubsystem::getSingleton().getInterpreter()->getScriptObject("DialogMeister");  
			// @todo: ScriptObject name has to be NPC name
			int nReturn=so->callIntegerFunction(strName,0,0);
		//  Search through all li-elements
			for (DOMNode* childNode=node->getFirstChild(); childNode != node->getLastChild(); childNode = childNode->getNextSibling() )
			{	
				if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) 
				{
					string nodeName=AimlParser::transcodeXmlCharToString(childNode->getNodeName());
					if(!nodeName.compare("li"))
					{
						nValue=XmlHelper::getAttributeValueAsInteger( (DOMElement*)childNode,"value" );
					//	char* test=XmlHelper::getAttributeValueAsString( (DOMElement*)childNode, "value" );
						std::string id =XmlHelper::getAttributeValueAsString( (DOMElement*)childNode, "id" ).c_str();
					//  Add elements in <li> Tag only if <li>-value = return value of the named function
						if(nValue==nReturn)
						{
							buffer+="<li id=\""+id+"\" />";
							buffer+=nlp->process(childNode,m,str);	
						}
					} // end compare nodeName
				} // end compare nodeType
			} // end for loop
		} // end check empty name
		return buffer;
	}
}// Namespace rl end
